#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"

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
    // 创建哈希表
    HashMapChaining *hashMap = newHashMapChaining(10, freeIntPtr);
    if (hashMap == NULL) {
        printf("创建哈希表失败\n");
        return 1;
    }
    
    // 添加键值对
    printf("添加键值对到哈希表...\n");
    int *value;
    
    value = createIntPtr(100);
    put(hashMap, 1, value);
    
    value = createIntPtr(200);
    put(hashMap, 2, value);
    
    value = createIntPtr(300);
    put(hashMap, 3, value);
    
    value = createIntPtr(400);
    put(hashMap, 4, value);
    
    // 打印哈希表
    printf("\n哈希表初始内容:\n");
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
    
    // 测试在迭代过程中删除元素
    printf("\n测试在迭代过程中删除元素:\n");
    printf("使用removeCurrent删除键为2和3的元素:\n");
    
    iterator = initIterator(hashMap);
    while (hasNext(&iterator)) {
        int key = getKey(&iterator);
        int *value = (int *)getValue(&iterator);
        printf("当前键: %d, 值: %d", key, *value);
        
        // 删除键为2和3的元素
        if (key == 2 || key == 3) {
            printf(" -> 删除\n");
            removeCurrent(&iterator);
        } else {
            printf(" -> 保留\n");
            next(&iterator);
        }
    }
    
    // 验证删除结果
    printf("\n删除后的哈希表内容:\n");
    print(hashMap);
    
    // 再次遍历验证
    printf("再次遍历验证:\n");
    iterator = initIterator(hashMap);
    while (hasNext(&iterator)) {
        int key = getKey(&iterator);
        int *value = (int *)getValue(&iterator);
        printf("键: %d, 值: %d\n", key, *value);
        next(&iterator);
    }
    
    // 释放哈希表
    delHashMapChaining(hashMap);
    
    return 0;
}
