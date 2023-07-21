#ifndef __FILE_READ__
#define __FILE_READ__

// file_read 的初始化函数
extern int file_read_init(void);

// 获取一个 key
// 成功返回 key 并设置 is_release，失败返回 -errno
extern int next_key(bool *is_release);

// 回收函数
extern void file_read_free(void);

#endif

