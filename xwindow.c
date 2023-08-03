#include <X11/Xlib.h>
#include <stdlib.h>
#include <regex.h>
#include <stdbool.h>
#include <X11/Xutil.h>

#include "xwindow.h"

// 检查窗口是否匹配
#define CHECK_WINDOW_MATCH(display, window, regex) ({ \
			Display *_display = display; \
			const Window _window = window; \
			const regex_t *_regex = regex; \
			XClassHint classhint; \
			bool is_match = False; \
			/* 获取窗口类名 */ \
			if(XGetClassHint(_display, _window, &classhint)){ \
				if(classhint.res_class != NULL) \
					is_match = !regexec(_regex, classhint.res_class, 0, NULL, 0); \
				XFree(classhint.res_name); \
				XFree(classhint.res_class); \
			}else{ \
				is_match = !regexec(_regex, "", 0, NULL, 0); \
			} \
			is_match; \
		})

static Display *display = NULL;
static int (*old_error_handler)(Display *, XErrorEvent *) = NULL;

// 新的错误 X Window 处理函数，掠过 BadWindow
static int xwindow_error_handler(Display *display, XErrorEvent *event){

	// 遇到 Bad Window，掠过
	if(event->error_code == BadWindow)
		return EXIT_SUCCESS;

	// 其余错误使用原错误处理函数处理
	if(old_error_handler != NULL)
		return old_error_handler(display, event);

	return EXIT_FAILURE;
}

// xwindow 的初始化函数
// display_name 是显示名称，填 NULL 会从环境变量获取
// 成功返回 EXIT_SUCCESS, 失败返回 EXIT_FAILURE
int xwindow_init(_Xconst char *display_name){

	// 连接 X Window Server
	if(display == NULL){
		display = XOpenDisplay(display_name);
		if(display == NULL)
			return EXIT_FAILURE;
	}

	// 备份并设置新的错误处理函数
	old_error_handler = XSetErrorHandler(xwindow_error_handler);

	return EXIT_SUCCESS;
}

// 回收函数
void xwindow_free(void){

	// 归还旧的错误处理函数
	if(old_error_handler != NULL)
		XSetErrorHandler(old_error_handler), old_error_handler = NULL;

	// 关闭连接
	if(display != NULL)
		XCloseDisplay(display), display = NULL;

	return;
}

// 递归搜索窗口
// 成功：调用 handler，display 是 X Window 的连接，window 是匹配到的窗口，handler 返回值为真时停止遍历并返回 EXIT_FAILURE。
// 失败：返回 EXIT_FAILURE
// 搜索完毕且 handler 返回值全为假时返回 EXIT_SUCCESS
static int _xwindow_search(const regex_t *regex, bool (*handler)(Display *display, Window window), Window parent_window){
	Window *children = NULL, none;
	unsigned int nchildren;

	// 遍历子窗口
	if(XQueryTree(display, parent_window, &none, &none, &children, &nchildren)){
		for(unsigned int i=0; i<nchildren; ++i){

			// 窗口匹配且处理函数返回真时退出
			if(CHECK_WINDOW_MATCH(display, children[i], regex))
				if(handler(display, children[i])){
					XFree(children);
					return EXIT_FAILURE;
				}

			// 遍历下一层自窗口
			if(_xwindow_search(regex, handler, children[i]) == EXIT_FAILURE){
				XFree(children);
				return EXIT_FAILURE;
			}
		}
	}else{

		// 获取失败，处理错误
		if(children != NULL)
			XFree(children);
	}

	return EXIT_SUCCESS;
}

// 搜索窗口
// 成功：调用 handler，display 是 X Window 的连接，window 是匹配到的窗口，handler 返回值为真时停止遍历并返回 EXIT_FAILURE。
// 失败：返回 EXIT_FAILURE
// 搜索完毕且 handler 返回值全为假时返回 EXIT_SUCCESS
int xwindow_search(const regex_t *regex, bool (*handler)(Display *display, Window window_id)){

	// 遍历桌面
	const int screen_count = ScreenCount(display);
	for(int i=0; i<screen_count; ++i){

		// 获取根窗口并比较
		Window root = RootWindow(display, i);
		if(CHECK_WINDOW_MATCH(display, root, regex))
			if(handler(display, root))
				break;
		// 遍历根窗口的下一层
		if(_xwindow_search(regex, handler, root) == EXIT_FAILURE)
			break;
	}

	XFlush(display);
	return EXIT_SUCCESS;
}

