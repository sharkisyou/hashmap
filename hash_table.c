#include <stdio.h>
#include <string.h>
#include "hash_table.h"
#include "utility.h"

/* 键值对 int->void */
typedef struct {
    int key;
    void *val;
} Pair;

/* 链表节点 */
typedef struct HashNode {
    Pair pair;
    struct HashNode *next;
} HashNode;

/* 链式地址哈希表 */
typedef struct HashMapChaining{
    size_t size;         // 键值对数量
    size_t capacity;     // 哈希表容量

#ifdef HASH_TABLE_AUTO_EXPAND
    float loadThres; // 触发扩容的负载因子阈值
    int extendRatio;  // 扩容倍数
#endif // HASH_TABLE_AUTO_EXPAND

    HashNode **buckets;   // 桶数组
    void (*freeVal)(void*); // 释放val的回调函数，如果为NULL则不释放

} HashMapChaining;

static void extend(HashMapChaining *hashMap);

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
HashMapChaining *newHashMapChaining(size_t capacity, void (*freeVal)(void*))
{
    if (capacity <= 0) {
        return NULL; 
    }
    HashMapChaining *hashMap = (HashMapChaining *)malloc(sizeof(HashMapChaining));
    if (hashMap == NULL) {
        return NULL;
    }

    hashMap->size = 0;
    hashMap->capacity = capacity;
    hashMap->freeVal = freeVal;
#ifdef HASH_TABLE_AUTO_EXPAND
    hashMap->loadThres = HASH_TABLE_LOAD_FACTOR;
    hashMap->extendRatio = HASH_TABLE_EXPAND_RATIO;
#endif // HASH_TABLE_AUTO_EXPAND
    hashMap->buckets = (HashNode **)malloc((size_t)hashMap->capacity * sizeof(HashNode *));
    if (hashMap->buckets == NULL) {
        free(hashMap);
        return NULL;
    }

    for (size_t i = 0; i < hashMap->capacity; i++) {
        hashMap->buckets[i] = NULL;
    }
    return hashMap;
}

/**
 * @brief 删除哈希表（链表法）
 *
 * 删除哈希表中的所有键值对，并释放哈希表所占用的内存。
 *
 * @param hashMap 哈希表的指针
 */
void delHashMapChaining(HashMapChaining *hashMap) {
    if (hashMap == NULL) {
        return;
    }
    
    for (size_t i = 0; i < hashMap->capacity; i++) {
        HashNode *cur = hashMap->buckets[i];
        while (cur) {
            HashNode *tmp = cur;
            cur = cur->next;
            // 如果设置了释放回调函数，则释放val指向的内存
            if (hashMap->freeVal != NULL && tmp->pair.val != NULL) {
                hashMap->freeVal(tmp->pair.val);
            }
            free(tmp);
        }
    }
    free(hashMap->buckets);
    free(hashMap);
}

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
 size_t hashFunc(HashMapChaining *hashMap, int key) {
    return (size_t)key % hashMap->capacity;
}

/**
 * @brief 计算哈希表的加载因子
 *
 * 该函数计算哈希表的加载因子，即哈希表中存储的元素数量与哈希表容量之间的比值。
 *
 * @param hashMap 哈希表对象指针
 * @return 返回哈希表的加载因子，类型为 float
 */
float loadFactor(HashMapChaining *hashMap) {
    return (float)hashMap->size / (float)hashMap->capacity;
}


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
void *get(HashMapChaining *hashMap, int key) {
    if (hashMap == NULL) {
        return NULL;
    }
    
    size_t index = hashFunc(hashMap, key);
    // 遍历桶，若找到 key ，则返回对应 val
    HashNode *cur = hashMap->buckets[index];
    while (cur) {
        if (cur->pair.key == key) {
            return cur->pair.val;
        }
        cur = cur->next;
    }
    return NULL; 
}

/* 添加操作 */
void put(HashMapChaining *hashMap, int key, const void *val) {
    if (hashMap == NULL || val == NULL) {
        return;
    }
    
#ifdef HASH_TABLE_AUTO_EXPAND
    // 当负载因子超过阈值时，执行扩容
    if (loadFactor(hashMap) > hashMap->loadThres) {
        extend(hashMap);
    }
#endif
    
    size_t index = hashFunc(hashMap, key);
    // 遍历桶，若遇到指定 key ，则更新对应 val 并返回
    HashNode *cur = hashMap->buckets[index];
    while (cur) {
        if (cur->pair.key == key) {
            // 更新现有键的值，使用memcpy而不是strcpy
            // 注意：这里假设调用者已经正确管理了val指向的内存
            // 如果需要深拷贝，调用者应该在传入前处理
            cur->pair.val = (void*)val;
            return;
        }
        cur = cur->next;
    }
    
    HashNode *newNode = (HashNode *)malloc(sizeof(HashNode));
    if (newNode == NULL) {
        return; // 内存分配失败
    }
    newNode->pair.key = key;
    newNode->pair.val = (void*)val;
    newNode->next = hashMap->buckets[index];
    hashMap->buckets[index] = newNode;
    hashMap->size++;
}

/* 扩容哈希表 */
static void extend(HashMapChaining *hashMap)
{
    if (hashMap == NULL) {
        return;
    }
    
    // 暂存原哈希表
    size_t oldCapacity = (size_t)hashMap->capacity;
    HashNode **oldBuckets = hashMap->buckets;
    
    // 初始化扩容后的新哈希表
    hashMap->capacity *= (size_t)hashMap->extendRatio;
    hashMap->buckets = (HashNode **)malloc((size_t)hashMap->capacity * sizeof(HashNode *));
    if (hashMap->buckets == NULL) {
        // 恢复原始容量，扩容失败
        hashMap->capacity = (size_t)oldCapacity;
        hashMap->buckets = oldBuckets;
        return;
    }
    
    for (size_t i = 0; i < hashMap->capacity; i++) {
        hashMap->buckets[i] = NULL;
    }
    hashMap->size = 0;
    
    // 将键值对从原哈希表搬运至新哈希表
    for (size_t i = 0; i < oldCapacity; i++) {
        HashNode *cur = oldBuckets[i];
        while (cur) {
            put(hashMap, cur->pair.key, cur->pair.val);
            HashNode *temp = cur;
            cur = cur->next;
            // 只释放节点内存，不释放val指向的内存
            free(temp);
        }
    }

    free(oldBuckets);
}

/* 删除操作 */
void removeItem(HashMapChaining *hashMap, int key) {
    if (hashMap == NULL) {
        return;
    }
    
    size_t index = hashFunc(hashMap, key);
    HashNode *cur = hashMap->buckets[index];
    HashNode *pre = NULL;
    while (cur) {
        if (cur->pair.key == key) {
            // 从中删除键值对
            if (pre) {
                pre->next = cur->next;
            } else {
                hashMap->buckets[index] = cur->next;
            }
            // 如果设置了释放回调函数，则释放val指向的内存
            if (hashMap->freeVal != NULL && cur->pair.val != NULL) {
                hashMap->freeVal(cur->pair.val);
            }

            free(cur);
            hashMap->size--;
            return;
        }
        pre = cur;
        cur = cur->next;
    }
}

/* 打印哈希表 */
void print(HashMapChaining *hashMap) {
    if (hashMap == NULL) {
        return;
    }
    
    for (size_t i = 0; i < hashMap->capacity; i++) {
        HashNode *cur = hashMap->buckets[i];
        printf("[");
        while (cur) {
            printf("%d -> %p, ", cur->pair.key, cur->pair.val);
            cur = cur->next;
        }
        printf("]\n");
    }
}

/* 初始化哈希表迭代器 */
HashMapIterator initIterator(HashMapChaining *hashMap) {
    HashMapIterator iterator;
    iterator.hashMap = hashMap;
    iterator.bucketIndex = 0;
    iterator.currentNode = NULL;
    iterator.hasNext = false;
    
    if (hashMap != NULL && hashMap->size > 0) {
        // 找到第一个非空桶
        for (size_t i = 0; i < hashMap->capacity; i++) {
            if (hashMap->buckets[i] != NULL) {
                iterator.bucketIndex = i;
                iterator.currentNode = hashMap->buckets[i];
                iterator.hasNext = true;
                break;
            }
        }
    }
    
    return iterator;
}

/* 判断迭代器是否有下一个元素 */
bool hasNext(HashMapIterator *iterator) {
    if (iterator == NULL) {
        return false;
    }
    return iterator->hasNext;
}

/* 获取迭代器当前元素的键 */
int getKey(HashMapIterator *iterator) {
    if (iterator == NULL || !iterator->hasNext || iterator->currentNode == NULL) {
        // 返回一个错误值，实际应用中可能需要更好的错误处理
        return -1;
    }
    
    HashNode *node = (HashNode *)iterator->currentNode;
    return node->pair.key;
}

/* 获取迭代器当前元素的值 */
void *getValue(HashMapIterator *iterator) {
    if (iterator == NULL || !iterator->hasNext || iterator->currentNode == NULL) {
        return NULL;
    }
    
    HashNode *node = (HashNode *)iterator->currentNode;
    return node->pair.val;
}

/* 将迭代器移动到下一个元素 */
void next(HashMapIterator *iterator) {
    if (iterator == NULL || !iterator->hasNext || iterator->currentNode == NULL) {
        return;
    }
    
    HashNode *currentNode = (HashNode *)iterator->currentNode;
    
    // 如果当前节点有下一个节点，直接移动到下一个节点
    if (currentNode->next != NULL) {
        iterator->currentNode = currentNode->next;
        return;
    }
    
    // 当前链表已经遍历完，需要找下一个非空桶
    for (size_t i = iterator->bucketIndex + 1; i < iterator->hashMap->capacity; i++) {
        if (iterator->hashMap->buckets[i] != NULL) {
            iterator->bucketIndex = i;
            iterator->currentNode = iterator->hashMap->buckets[i];
            return;
        }
    }
    
    // 所有桶都已经遍历完
    iterator->hasNext = false;
    iterator->currentNode = NULL;
}

