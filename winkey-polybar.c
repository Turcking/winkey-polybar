#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <linux/input.h>
#include <stdlib.h>
#include <poll.h>
#include <errno.h>

#define EVENT_FP "/dev/input/event2"
#define KEYCODE 125
#define KEYDOWN_TODO (system("xdotool search --class polybar | while read w; do xdotool windowmap $w; done"))
#define KEYUP_TODO (system("xdotool search --class polybar | while read w; do xdotool windowunmap $w; done"))

int main(void){

	struct input_event event;
	struct pollfd pollfd;
	int event_fd, ret, event_fd_nu;

	// 打开 event 文件
	event_fd = open(EVENT_FP, O_RDONLY);
	if(event_fd < 0){
		perror(EVENT_FP);
		return -1;
	}

	// 初始化 pollfd
	pollfd.fd = event_fd;
	// 循环读取 event 文件
	event_fd_nu = 0; // 读取到的字节数
	while(true){
		// 等待 event 文件可读 
		pollfd.events = POLLIN;
		if(poll(&pollfd, 1, -1) == -1){
			if(errno == EAGAIN || errno == EINTR) // 忽略跳过伪异常（EAGAIN: 资源暂时不可用，EINTR: 信号中断了系统调用）
				continue;
			perror("poll()");
			break;
		}
		// 读取 event 文件
		ret = read(event_fd, ((char *)(&event))+event_fd_nu, sizeof(event)-event_fd_nu);
		if(ret < 0){
			perror(EVENT_FP);
			return EXIT_FAILURE;
		}
		if(event_fd_nu + ret < sizeof(event)){
			// printf("Get %d data\n", ret);
			continue;
		}else{
			// 解析 event 事件
			if(event.type == EV_KEY){
				if(event.value == 0 || event.value == 1){
					//printf("key %d %s\n", event.code, event.value?"Pressed":"Released");
					if(event.code == KEYCODE && event.value){ // 按下 Win 键
						KEYDOWN_TODO;
					}else if(event.code == KEYCODE){ // 抬起 Win 键
						KEYUP_TODO;
					}
				}
			}/*else{
				//printf("type=%x %d %d\n", event.type, event.code, event.value);
			}*/
			event_fd_nu = 0;
		}
	}

	close(event_fd);
	return EXIT_SUCCESS;
}

