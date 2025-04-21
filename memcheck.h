#ifndef MEMCHECK_H
#define MEMCHECK_H

#include <string.h>
#include <stdbool.h>

// 初始化内存检测系统
void memcheck_init(void);

// 清理内存检测系统
void memcheck_cleanup(void);

// 分配内存并打印地址
void *memcheck_malloc(size_t size, const char *file, int line);

// 重新分配内存并打印地址
void *memcheck_realloc(void *ptr, size_t size, const char *file, int line);

// 分配内存并初始化为0
void *memcheck_calloc(size_t nmemb, size_t size, const char *file, int line);

// 释放内存并打印地址
void memcheck_free(void *ptr, const char *file, int line);

// 生成简单报告
void memcheck_report(void);

// 为保持API兼容性而保留的函数
size_t memcheck_get_allocated_memory(void);
bool memcheck_is_valid_pointer(void *ptr);


#endif // MEMCHECK_H
