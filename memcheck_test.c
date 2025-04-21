#include <stdio.h>
#include "memcheck.h"
#include "utility.h"
// 故意泄漏内存的函数
void leak_memory(void) {
    int *ptr = malloc(sizeof(int) * 10);
    (void)ptr; // 避免未使用的变量警告 (unused variable warning suppression)
    printf("分配了10个整数的内存，但不会释放它\n");
    // 故意不释放ptr，造成内存泄漏
}

// 正确使用内存的函数
void use_memory_correctly(void) {
    char *str = malloc(100);
    if (str != NULL) {
        strcpy(str, "这是一个正确使用内存的例子");
        printf("%s\n", str);
        free(str); // 正确释放内存
    }
}

// 重复释放内存的函数
void double_free(void) {
    int *ptr = malloc(sizeof(int));
    if (ptr != NULL) {
        *ptr = 42;
        printf("分配了一个整数内存并赋值: %d\n", *ptr);
        free(ptr); // 第一次释放
        printf("第一次释放内存\n");
        // // 下面这行会导致重复释放错误
        // free(ptr); // 第二次释放同一块内存
        // printf("第二次释放内存 - 这是一个错误操作\n");
    }
}

// 使用未初始化内存的函数
void use_uninitialized_memory(void) {
    int *ptr = malloc(sizeof(int) * 5);
    if (ptr != NULL) {
        // 没有初始化ptr指向的内存就直接使用
        printf("未初始化的内存值: %d\n", ptr[3]); // 使用未初始化的内存
        free(ptr);
    }
}

// 内存越界访问的函数
void memory_out_of_bounds(void) {
    int *array = malloc(sizeof(int) * 5);
    if (array != NULL) {
        for (int i = 0; i < 5; i++) {
            array[i] = i;
        }
        // 越界访问
        array[5] = 100; // 这是越界访问，数组只有5个元素，索引应为0-4
        printf("进行了越界访问，写入了array[5]\n");
        free(array);
    }
}

int main(void) {
    // 初始化内存检测系统
    memcheck_init();
    
    printf("\n=== 测试1: 正确使用内存 ===\n");
    use_memory_correctly();
    
    printf("\n=== 测试2: 内存泄漏 ===\n");
    // leak_memory();
    
    printf("\n=== 测试3: 重复释放内存 ===\n");
    double_free();
    
    printf("\n=== 测试4: 使用未初始化内存 ===\n");
    use_uninitialized_memory();
    
    printf("\n=== 测试5: 内存越界访问 ===\n");
    memory_out_of_bounds();
    
    // 生成内存泄漏报告
    printf("\n=== 生成内存泄漏报告 ===\n");
    memcheck_report();
    
    // 清理内存检测系统
    memcheck_cleanup();
    
    return 0;
}
