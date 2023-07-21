#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#include "file_read.h"

#define KEYCODE 125
#define KEYDOWN_TODO (system("xdotool search --class polybar | while read w; do xdotool windowmap $w; done"))
#define KEYUP_TODO (system("xdotool search --class polybar | while read w; do xdotool windowunmap $w; done"))

// 清理
void clean_up(int sig){
	file_read_free();
	exit(sig);
}

int main(void){

	int _ret;
	bool is_down;

	/*
	  if we receive a signal, we want to exit nicely, in
	  order not to leave the keyboard in an unusable mode
	*/
	signal(SIGHUP, clean_up);
	signal(SIGINT, clean_up);
	signal(SIGQUIT, clean_up);
	signal(SIGILL, clean_up);
	signal(SIGTRAP, clean_up);
	signal(SIGABRT, clean_up);
	signal(SIGIOT, clean_up);
	signal(SIGFPE, clean_up);
	signal(SIGKILL, clean_up);
	signal(SIGUSR1, clean_up);
	signal(SIGSEGV, clean_up);
	signal(SIGUSR2, clean_up);
	signal(SIGPIPE, clean_up);
	signal(SIGTERM, clean_up);
#ifdef SIGSTKFLT
	signal(SIGSTKFLT, clean_up);
#endif
	//signal(SIGCHLD, clean_up);
	signal(SIGCONT, clean_up);
	signal(SIGSTOP, clean_up);
	signal(SIGTSTP, clean_up);
	signal(SIGTTIN, clean_up);
	signal(SIGTTOU, clean_up);

	// 初始化 file_read
	if((_ret = file_read_init()) < 0){
		fprintf(stderr, "file_read_init():%s", strerror(-_ret));
		return EXIT_FAILURE;
	}

	// 获取键盘输入
	is_down = false; // 防止多次显示
	while(true){
		bool is_release;
		int key;

		// 获取键盘输入 
		key = next_key(&is_release);
		// 错误处理
		if(key < 0){
			fprintf(stderr, "next_key():%s", strerror(-key));
			return EXIT_FAILURE;
		}
		// 解析键盘
		if(key == KEYCODE){
			if(is_release){
				KEYUP_TODO; // 键抬起
				is_down = false;
			}else if(!is_down){
				KEYDOWN_TODO; // 键按下
				is_down = true;
			}
		}
	}

	file_read_free();
	return EXIT_SUCCESS;
}

