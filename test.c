#include "hash_table.h"
#include <stdio.h>
#include <string.h>

int main(void) {
    // 创建哈希表
    HashMapChaining *hashMap = newHashMapChaining(10, NULL);
    if (hashMap == NULL) {
        printf("创建哈希表失败\n");
        return 1;
    }
    
    // 测试数据
    char *value1 = "value1";
    char *value2 = "value2";
    
    // 测试插入功能
    put(hashMap, 1, value1);
    put(hashMap, 2, value2);
    put(hashMap, 11, "value3");
    
    // 测试查找功能
    printf("查找key1: %s\n", (char *)get(hashMap, 1));
    printf("查找key2: %s\n", (char *)get(hashMap, 2));
    printf("查找key3: %s\n", (char *)get(hashMap, 3) ? (char *)get(hashMap, 3) : "未找到");
    
    // 打印哈希表
    printf("\n哈希表内容:\n");
    print(hashMap);
    
    // 测试删除功能
    // removeItem(hashMap, 1);
    // printf("\n删除key1后:\n");
    // printf("查找key1: %s\n", (char *)get(hashMap, 1) ? (char *)get(hashMap, 1) : "未找到");
    
    // // 释放哈希表
    // delHashMapChaining(hashMap);
    return 0;
}
