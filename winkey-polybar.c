#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <linux/input.h>
#include <stdlib.h>
#include <poll.h>
#include <errno.h>

#define EVENT_FP "/dev/input/event2"

int main(void){

	struct input_event event;
	struct pollfd pollfd;
	int event_fd, ret;

	// 打开 event 文件
	event_fd = open(EVENT_FP, O_RDONLY);
	if(event_fd < 0){
		perror(EVENT_FP);
		return -1;
	}

	// 初始化 pollfd
	pollfd.fd = event_fd;
	// 循环读取 event 文件
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
		ret = read(event_fd, &event, sizeof(event));
		if(ret != sizeof(event)){
			perror(EVENT_FP);
		}
		// 解析 event 事件
		if(event.type == EV_KEY){
			if(event.value == 0 || event.value == 1){
				//printf("key %d %s\n", event.code, event.value?"Pressed":"Released");
				if(event.code == 125 && event.value) // 按下 Win 键
					system("xdotool search --class polybar | while read w; do xdotool windowmap $w; done");
				else if(event.code == 125) // 抬起 Win 键
					system("xdotool search --class polybar | while read w; do xdotool windowunmap $w; done");
			}
		}/*else{
			//printf("type=%x %d %d\n", event.type, event.code, event.value);
		}*/
	}

	close(event_fd);
	return 0;
}

