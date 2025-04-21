#include <stdio.h>
#include <stdbool.h>
#include <math.h>

/**
 * @brief 判断一个整数是否为质数
 *
 * 判断一个整数是否为质数。质数是指除了1和它本身以外不再有其他因数的自然数。
 *
 * @param num 待判断的整数
 * @return 如果num是质数，则返回true；否则返回false
 */
bool is_prime(int num) {
    if (num <= 1) return false;
    if (num == 2) return true;
    if (num % 2 == 0) return false;
    
    int sqrt_num = (int)(sqrt(num)) + 1;
    for (int i = 3; i < sqrt_num; i += 2) {
        if (num % i == 0) {
            return false;
        }
    }
    return true;
}

/**
 * @brief 查找给定数字范围内的最大质数
 *
 * 在给定的数字范围内查找最大的质数。
 *
 * @param num 给定的数字范围，用于查找最大质数
 * @return 返回范围内的最大质数，如果范围内没有质数则返回-1
 */
int find_largest_prime(int num) {
    if (num < 2) return -1; // 没有质数小于2
    
    for (int i = num; i >= 2; i--) {
        if (is_prime(i)) {
            return i;
        }
    }
    return -1; // 理论上不会执行到这里
}

