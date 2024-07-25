//
// Created by hao on 2024/7/23.
//

#ifndef DATAST_ARRAY_HAO_H
#define DATAST_ARRAY_HAO_H

#include <exception>
#include <cstdio>
#include <cstdlib>

/**
 * 数组定义
 */
template <class T>
struct Array
{
    // p指针的空间大小
    size_t size;

    // p指针已经使用的空间大小
    size_t len;

    // 数据类型的大小
    size_t typeSize;

    // 值复制函数
    void(*dup)(void *ptr, void *key);

    // 值释放函数
    void(*free)(void *ptr);

    // 值比较函数
    int(*match)(void *ptr, void *key);

    // 存放数据的指针
    void *p;
};

/**
 * 创建数组
 * @return 创建数组的指针，如果内存分配失败则返回 nullptr
 */
Array* arrayCreate();

/**
 * 初始化数组
 * @param array 数组指针
 * @param size 数组的大小
 * @param typeSize 类型大小
 */
void arrayInit(Array *array, int size, int typeSize);

int arrayInsert(Array *array, size_t pos, void *const value);
size_t arraySearchValue(Array *array, void* const value);
void* arrayIndex(Array *array, size_t index);
int arrayModify(Array *array, size_t pos, void *const value);

size_t arrayLen(Array *array);
size_t arraySize(Array *array);

void arrayEmpty(Array *array);
void arrayDelValue(Array *array, void *value);
void arrayDelIndex(Array *array, size_t pos);

#endif //DATAST_ARRAY_HAO_H
