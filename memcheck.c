#include "memcheck.h"
#include <stdlib.h>
#include <stdio.h>

// 统计信息
static size_t total_allocations = 0;    // 总分配次数
static size_t total_frees = 0;         // 总释放次数
static bool is_initialized = false;    // 是否已初始化

// 初始化内存检测系统
void memcheck_init(void) {
    if (!is_initialized) {
        total_allocations = 0;
        total_frees = 0;
        is_initialized = true;
        printf("[MemCheck] 内存检测系统已初始化\n");
    }
}

// 清理内存检测系统
void memcheck_cleanup(void) {
    if (is_initialized) {
        memcheck_report();
        is_initialized = false;
        printf("[MemCheck] 内存检测系统已清理\n");
    }
}

// 分配内存并打印地址
void *memcheck_malloc(size_t size, const char *file, int line) {
    if (!is_initialized) {
        memcheck_init();
    }
    
    // 直接调用标准库函数，避免递归
    void *ptr = (void *)malloc(size);
    
    if (ptr != NULL) {
        total_allocations++;
        printf("[MemCheck] 分配内存: 地址 %p, 大小 %zu 字节, 位置 %s:%d\n", 
               ptr, size, file, line);
    } else {
        fprintf(stderr, "[MemCheck] 错误: 内存分配失败, 大小 %zu 字节, 位置 %s:%d\n", 
                size, file, line);
    }
    
    return ptr;
}

// 重新分配内存并打印地址
void *memcheck_realloc(void *ptr, size_t size, const char *file, int line) {
    if (!is_initialized) {
        memcheck_init();
    }
    
    // 如果ptr为NULL，相当于malloc
    if (ptr == NULL) {
        return memcheck_malloc(size, file, line);
    }
    
    // 如果size为0，相当于free
    if (size == 0) {
        memcheck_free(ptr, file, line);
        return NULL;
    }
    
    printf("[MemCheck] 重新分配内存: 原地址 %p, 新大小 %zu 字节, 位置 %s:%d\n", 
           ptr, size, file, line);
    
    // 重新分配内存
    void *new_ptr = realloc(ptr, size);
    
    if (new_ptr != NULL) {
        printf("[MemCheck] 重新分配完成: 新地址 %p\n", new_ptr);
    } else {
        fprintf(stderr, "[MemCheck] 错误: 内存重新分配失败, 大小 %zu 字节, 位置 %s:%d\n", 
                size, file, line);
    }
    
    return new_ptr;
}

// 分配内存并初始化为0
void *memcheck_calloc(size_t nmemb, size_t size, const char *file, int line) {
    if (!is_initialized) {
        memcheck_init();
    }
    
    void *ptr = calloc(nmemb, size);
    
    if (ptr != NULL) {
        total_allocations++;
        printf("[MemCheck] 分配内存(calloc): 地址 %p, 数量 %zu, 单元大小 %zu 字节, 总大小 %zu 字节, 位置 %s:%d\n", 
               ptr, nmemb, size, nmemb * size, file, line);
    } else {
        fprintf(stderr, "[MemCheck] 错误: 内存分配(calloc)失败, 数量 %zu, 单元大小 %zu 字节, 位置 %s:%d\n", 
                nmemb, size, file, line);
    }
    
    return ptr;
}

// 释放内存并打印地址
void memcheck_free(void *ptr, const char *file, int line) {
    if (!is_initialized) {
        memcheck_init();
    }
    
    if (ptr == NULL) {
        printf("[MemCheck] 警告: 尝试释放NULL指针, 位置 %s:%d\n", file, line);
        return; // 释放NULL指针是合法的，不做任何操作
    }
    
    printf("[MemCheck] 释放内存: 地址 %p, 位置 %s:%d\n", ptr, file, line);
    total_frees++;
    
    // 释放实际内存
    free(ptr);
}

// 生成简单报告
void memcheck_report(void) {
    if (!is_initialized) {
        printf("[MemCheck] 内存检测系统未初始化\n");
        return;
    }
    
    printf("\n===== 内存操作统计 =====\n");
    printf("总分配次数: %zu\n", total_allocations);
    printf("总释放次数: %zu\n", total_frees);
    
    if (total_allocations > total_frees) {
        printf("警告: 可能存在内存泄漏, 有 %zu 次分配未释放\n", 
               total_allocations - total_frees);
    } else if (total_allocations < total_frees) {
        printf("警告: 释放次数多于分配次数, 可能存在释放未分配内存的情况\n");
    } else {
        printf("所有分配的内存都已释放\n");
    }
    
    printf("===== 报告结束 =====\n\n");
}

// 这些函数在简化版中不再需要实际功能，但为了保持API兼容性，保留空实现
size_t memcheck_get_allocated_memory(void) {
    return 0; // 简化版不再跟踪已分配内存总量
}

bool memcheck_is_valid_pointer(void *ptr) {
    (void)ptr; // 避免未使用变量的警告(
    return false; // 简化版不再跟踪指针有效性
}
