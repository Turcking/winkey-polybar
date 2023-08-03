#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <regex.h>

#include "file_read.h"
#include "xwindow.h"

#ifndef KEYCODE
#define KEYCODE 125
#endif

#ifndef REGEX_EXPRESSION
#define REGEX_EXPRESSION "Polybar"
#endif

regex_t regex;

// 清理函数
void clean_up(int sig){
	xwindow_free();
	file_read_free();
	regfree(&regex);
	exit(sig);
}

// 显示，隐藏窗口处理程序
bool unmap_xwindow_handler(Display *display, Window window){
	XUnmapWindow(display, window);
	return false;
}
bool map_xwindow_handler(Display *display, Window window){
	XRaiseWindow(display, window);
	XMapWindow(display, window);
	return false;
}

int main(void){

	int _ret, regex_error_code;
	bool is_down;

	/*
	  if we receive a signal, we want to exit nicely, in
	  order not to leave the keyboard in an unusable mode
	*/
	signal(SIGALRM, clean_up);
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
	signal(SIGCHLD, clean_up);
	signal(SIGCONT, clean_up);
	signal(SIGSTOP, clean_up);
	signal(SIGTSTP, clean_up);
	signal(SIGTTIN, clean_up);
	signal(SIGTTOU, clean_up);

	// 编译正则表达式
	if((regex_error_code = regcomp(&regex, REGEX_EXPRESSION, REG_EXTENDED | REG_ICASE))){
		char regerror_str[regerror(regex_error_code, &regex, NULL, 0)];
		regerror(regex_error_code, &regex, regerror_str, sizeof(regerror_str));
		fputs(regerror_str, stderr);
		return EXIT_FAILURE;
	}

	// 初始化 file_read
	if((_ret = file_read_init()) < 0){
		regfree(&regex);
		fprintf(stderr, "file_read_init():%s", strerror(-_ret));
		return EXIT_FAILURE;
	}
	
	// 初始化 xwindow
	if(xwindow_init(NULL) == EXIT_FAILURE){
		regfree(&regex);
		file_read_free();
		fputs("xwindow init failed", stderr);
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
				xwindow_search(&regex, unmap_xwindow_handler);  // 键抬起
				is_down = false;
			}else if(!is_down){
				xwindow_search(&regex, map_xwindow_handler);  // 键按下
				is_down = true;
			}
		}
	}

	xwindow_free();
	file_read_free();
	regfree(&regex);
	return EXIT_SUCCESS;
}

