#ifndef UTILITY_H
#define UTILITY_H

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

// 内存检测相关宏定义
#define MEMCHECK_ENABLE 1  // 设置为0可禁用内存检测

#if MEMCHECK_ENABLE
#include "memcheck.h"
#define MEM_INIT() memcheck_init()
#define MEM_CLEANUP() memcheck_cleanup()
#define MEM_REPORT() memcheck_report()

// 重定义标准内存函数
#define malloc(size) memcheck_malloc(size, __FILE__, __LINE__)
#define realloc(ptr, size) memcheck_realloc(ptr, size, __FILE__, __LINE__)
#define calloc(nmemb, size) memcheck_calloc(nmemb, size, __FILE__, __LINE__)
#define free(ptr) memcheck_free(ptr, __FILE__, __LINE__)

#else
#define MEM_INIT()
#define MEM_CLEANUP()
#define MEM_REPORT()
#endif

#define MEM_CHECK(ptr, action) \
    do { \
        if ((ptr) == NULL) { \
            printf("%s[%d]:Memory allocation failed.\n", __func__, __LINE__); \
            action; \
        } \
    } while (0)

/**
 * @brief 判断一个整数是否为质数
 *
 * 判断一个整数是否为质数。质数是指除了1和它本身以外不再有其他因数的自然数。
 *
 * @param num 待判断的整数
 * @return 如果num是质数，则返回true；否则返回false
 */
bool is_prime(int num);

/**
 * @brief 查找给定数字范围内的最大质数
 *
 * 在给定的数字范围内查找最大的质数。
 *
 * @param num 给定的数字范围，用于查找最大质数
 * @return 返回范围内的最大质数，如果范围内没有质数则返回-1
 */
int find_largest_prime(int num);

#endif // UTILITY_H
