#ifndef __XWINDOW_H__
#define __XWINDOW_H__

#include <X11/Xlib.h>
#include <regex.h>
#include <stdbool.h>

// xwindow 的初始化函数
// display_name 是显示名称，填 NULL 会从环境变量获取
// 成功返回 EXIT_SUCCESS, 失败返回 EXIT_FAILURE
extern int xwindow_init(_Xconst char *display_name);

// 回收函数
extern void xwindow_free(void);

// 搜索窗口
// 成功：调用 handler，display 是 X Window 的连接，window 是匹配到的窗口，handler 返回值为真时停止遍历并返回 EXIT_FAILURE。
// 失败：返回 EXIT_FAILURE
// 搜索完毕且 handler 返回值全为假时返回 EXIT_SUCCESS
extern int xwindow_search(const regex_t *regex, bool (*handler)(Display *display, Window window_id));

#endif

