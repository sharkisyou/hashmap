#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdbool.h>
#include <stddef.h>

#define HASH_TABLE_AUTO_EXPAND  // 哈希表自动扩容
#ifdef HASH_TABLE_AUTO_EXPAND
#define HASH_TABLE_LOAD_FACTOR 0.75  // 负载因子
#define HASH_TABLE_EXPAND_RATIO 2 // 扩容倍数
#endif

/* 链式地址哈希表 */
typedef struct HashMapChaining HashMapChaining;

/* 哈希表迭代器 */
typedef struct {
    HashMapChaining *hashMap;  // 迭代器所属的哈希表
    size_t bucketIndex;           // 当前桶索引tIndex;           // 当前桶索引 bucketIndex;           // 当前桶索引
    void *currentNode;         // 当前节点指针
    bool hasNext;              // 是否有下一个元素
} HashMapIterator;

/**
 * @brief 创建一个新的 HashMapChaining 对象
 *
 * 该函数用于创建一个新的 HashMapChaining 对象，初始化其容量、自由值释放函数等属性，并为其分配存储空间。
 *
 * @param capacity 哈希表的容量，即桶的数量。必须大于0。
 * @param freeVal val 值释放函数指针，用于释放存储在哈希表中的值。如果不需要释放，可以传递 NULL。
 *
 * @return 成功时返回新创建的 HashMapChaining 对象指针，失败时返回 NULL。
 */
 HashMapChaining *newHashMapChaining(size_t capacity, void (*freeVal)(void*));

/**
 * @brief 删除哈希表（链表法）
 *
 * 删除哈希表中的所有键值对，并释放哈希表所占用的内存。
 *
 * @param hashMap 哈希表的指针
 */
void delHashMapChaining(HashMapChaining *hashMap);

/**
 * @brief 哈希函数，计算给定键的哈希值
 *
 * 根据给定的哈希表（使用链表法处理冲突）和键，计算该键的哈希值。
 *
 * @param hashMap 哈希表指针，类型为HashMapChaining*
 * @param key 需要计算哈希值的键，类型为int
 *
 * @return 返回计算得到的哈希值，类型为int
 */
 size_t hashFunc(HashMapChaining *hashMap, int key);

/**
 * @brief 计算哈希表的加载因子
 *
 * 该函数计算哈希表的加载因子，即哈希表中存储的元素数量与哈希表容量之间的比值。
 *
 * @param hashMap 哈希表对象指针
 * @return 返回哈希表的加载因子，类型为 float
 */
float loadFactor(HashMapChaining *hashMap);

/**
 * @brief 根据键从哈希表中获取值
 *
 * 从哈希表中获取与指定键对应的值。如果键存在，则返回对应的值；如果键不存在，则返回NULL。
 *
 * @param hashMap 哈希表的指针
 * @param key 要查找的键
 *
 * @return 返回与键对应的值，如果键不存在则返回NULL
 */
void *get(HashMapChaining *hashMap, int key);

/**
 * @brief 添加键值对到哈希表
 *
 * 向哈希表中添加一个键值对。如果键已存在，则更新对应的值。
 *
 * @param hashMap 哈希表的指针
 * @param key 要添加的键
 * @param val 要添加的值
 */
void put(HashMapChaining *hashMap, int key, const void *val);

/**
 * @brief 从哈希表中删除键值对
 *
 * 从哈希表中删除指定键的键值对。
 *
 * @param hashMap 哈希表的指针
 * @param key 要删除的键
 */
void removeItem(HashMapChaining *hashMap, int key);

/**
 * @brief 打印哈希表
 *
 * 打印哈希表中的所有键值对。
 *
 * @param hashMap 哈希表的指针
 */
void print(HashMapChaining *hashMap);

/**
 * @brief 初始化哈希表迭代器
 *
 * 初始化一个哈希表迭代器，用于遍历哈希表中的所有键值对。
 *
 * @param hashMap 哈希表的指针
 * @return 返回初始化后的迭代器
 */
HashMapIterator initIterator(HashMapChaining *hashMap);

/**
 * @brief 判断迭代器是否有下一个元素
 *
 * 判断迭代器是否还有下一个元素可以访问。
 *
 * @param iterator 迭代器的指针
 * @return 如果有下一个元素，则返回true；否则返回false
 */
bool hasNext(HashMapIterator *iterator);

/**
 * @brief 获取迭代器当前元素的键
 *
 * 获取迭代器当前指向的键值对中的键。
 *
 * @param iterator 迭代器的指针
 * @return 返回当前键值对的键
 */
int getKey(HashMapIterator *iterator);

/**
 * @brief 获取迭代器当前元素的值
 *
 * 获取迭代器当前指向的键值对中的值。
 *
 * @param iterator 迭代器的指针
 * @return 返回当前键值对的值
 */
void *getValue(HashMapIterator *iterator);

/**
 * @brief 将迭代器移动到下一个元素
 *
 * 将迭代器移动到哈希表中的下一个键值对。
 *
 * @param iterator 迭代器的指针
 */
void next(HashMapIterator *iterator);

#endif // HASH_TABLE_H
