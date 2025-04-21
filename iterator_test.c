#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"
#include "utility.h"

// 释放整数指针的回调函数
void freeIntPtr(void *ptr) {
    free(ptr);
}

// 创建整数指针
int *createIntPtr(int value) {
    int *ptr = (int *)malloc(sizeof(int));
    if (ptr != NULL) {
        *ptr = value;
    }
    return ptr;
}

int main(void) {
    // 初始化内存检测
    MEM_INIT();
    
    // 创建哈希表
    HashMapChaining *hashMap = newHashMapChaining(10, freeIntPtr);
    if (hashMap == NULL) {
        printf("创建哈希表失败\n");
        return 1;
    }
    
    // 添加键值对
    printf("添加键值对到哈希表...\n");
    for (int i = 0; i < 20; i++) {
        int *value = createIntPtr(i * 100);
        if (value == NULL) {
            printf("内存分配失败\n");
            delHashMapChaining(hashMap);
            return 1;
        }
        put(hashMap, i, value);
    }
    
    // 打印哈希表
    printf("\n哈希表内容:\n");
    print(hashMap);
    
    // 使用迭代器遍历哈希表
    printf("\n使用迭代器遍历哈希表:\n");
    HashMapIterator iterator = initIterator(hashMap);
    
    while (hasNext(&iterator)) {
        int key = getKey(&iterator);
        int *value = (int *)getValue(&iterator);
        printf("键: %d, 值: %d\n", key, *value);
        next(&iterator);
    }
    
    // 测试删除操作后的迭代
    printf("\n删除键 5 和 15 后:\n");
    removeItem(hashMap, 5);
    removeItem(hashMap, 15);
    
    // 重新初始化迭代器
    iterator = initIterator(hashMap);
    while (hasNext(&iterator)) {
        int key = getKey(&iterator);
        int *value = (int *)getValue(&iterator);
        printf("键: %d, 值: %d\n", key, *value);
        next(&iterator);
    }
    
    // 释放哈希表
    delHashMapChaining(hashMap);
    
    // 内存检测报告
    MEM_REPORT();
    MEM_CLEANUP();
    
    return 0;
}
