#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/kd.h>
#include <string.h>
#include <poll.h>

#include "file_read.h"

static const char *conspath[] = {
	"/proc/self/fd/0",
	"/dev/tty",
	"/dev/tty0",
	"/dev/vc/0",
	"/dev/systty",
	"/dev/console",
	NULL
};

// 判断 fd 是否为一个 console
#define IS_A_CONSOLE(fd) ({ \
		char arg; \
		(isatty(fd) && ioctl(fd, KDGKBTYPE, &arg) == 0 && ((arg == KB_101) || (arg == KB_84))); \
	})

static int fd = -1;
static int old_keyboard_mode = -1;
static int n = 0, i = 0;
static unsigned char buf[18];
static struct pollfd pollfd = {.fd = -1};

// 获取 console 的 fd
// 成功返回 fd，失败返回 -errno
static int getfd(void){
	int fd;
	for(int i = 0; conspath[i] != NULL; i++){
		if((fd = open(conspath[i], O_RDONLY)) >= 0){
			if(IS_A_CONSOLE(fd))
				return fd;
			close(fd);
		}
	}
	return -errno;
}

// file_read 的初始化函数
int file_read_init(void){
	memset(buf, 0, sizeof(buf));
	if((fd = getfd()) < 0)
		return fd;
	if(ioctl(fd, KDGKBMODE, &old_keyboard_mode)){
		int _errno = -errno;
		old_keyboard_mode = -1;
		close(fd);
		fd = -1;
		return _errno;
	}
	if(ioctl(fd, KDSKBMODE, K_MEDIUMRAW)){
		int _errno = -errno;
		ioctl(fd, KDSKBMODE, old_keyboard_mode);
		old_keyboard_mode = -1;
		close(fd);
		fd = -1;
		return _errno;
	}
	pollfd.fd = fd;
	return EXIT_SUCCESS;
}

// 获取一个 key
// 成功返回 key 并设置 is_release，失败返回 -errno
int next_key(bool *is_release){
	int key;
	if(i == n || n == 0){
		i = 0;
		pollfd.events = POLLIN;
		while(poll(&pollfd, 1, -1) == -1){
			int _errno = errno;
			if(_errno != EAGAIN && _errno != EINTR)
				return -_errno;
		}
		n = read(fd, buf, sizeof(buf));
	}
	*is_release = (buf[i] & 0x80)?true:false;
	if(i + 2 < n && (buf[i] & 0x7f) == 0 && (buf[i + 1] & 0x80) != 0 && (buf[i + 2] & 0x80) != 0){
		key = ((buf[i + 1] & 0x7f) << 7) | (buf[i + 2] & 0x7f);
		i += 3;
	} else {
		key = (buf[i] & 0x7f);
		i++;
	}

	return key;
}

// 回收函数
void file_read_free(void){
	pollfd.fd = -1;
	if(old_keyboard_mode != -1)
		ioctl(fd, KDSKBMODE, old_keyboard_mode), old_keyboard_mode = -1;
	if(fd >= 0)
		close(fd), fd = -1;
	return;
}

