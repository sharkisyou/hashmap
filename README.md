# C语言哈希表实现

这是一个用C语言实现的高效哈希表库，使用链式地址法解决哈希冲突，并支持自动扩容功能。

## 特性

- 使用链式地址法（拉链法）解决哈希冲突
- 支持自动扩容，默认负载因子为0.75
- 提供完整的哈希表操作API
- 支持迭代器遍历
- 内存管理安全，支持自定义值释放函数
- 严格的编译选项，确保代码质量

## 主要API

```c
// 创建哈希表
HashMapChaining *newHashMapChaining(size_t capacity, void (*freeVal)(void*));

// 删除哈希表
void delHashMapChaining(HashMapChaining *hashMap);

// 插入键值对
bool put(HashMapChaining *hashMap, int key, void *val);

// 获取值
void *get(HashMapChaining *hashMap, int key);

// 删除键值对
bool removeItem(HashMapChaining *hashMap, int key);

// 获取哈希表大小
size_t size(HashMapChaining *hashMap);

// 判断哈希表是否为空
bool isEmpty(HashMapChaining *hashMap);

// 清空哈希表
void clear(HashMapChaining *hashMap);

// 打印哈希表
void print(HashMapChaining *hashMap);

// 迭代器相关函数
HashMapIterator newIterator(HashMapChaining *hashMap);
bool hasNext(HashMapIterator *iterator);
Pair next(HashMapIterator *iterator);
```

## 编译和使用

本项目使用CMake构建系统：

```bash
# 创建构建目录
mkdir -p build
cd build

# 配置CMake项目
cmake ..

# 编译
cmake --build .

# 运行测试
./hash_table_test
./iterator_test
./memcheck_test
```

## 示例

```c
#include "hash_table.h"
#include <stdio.h>

int main(void) {
    // 创建哈希表，容量为10，不使用自定义释放函数
    HashMapChaining *hashMap = newHashMapChaining(10, NULL);
    
    // 插入键值对
    put(hashMap, 1, "value1");
    put(hashMap, 2, "value2");
    
    // 获取值
    printf("key 1: %s\n", (char *)get(hashMap, 1));
    
    // 打印哈希表内容
    print(hashMap);
    
    // 删除哈希表
    delHashMapChaining(hashMap);
    
    return 0;
}
```

## 许可证

MIT