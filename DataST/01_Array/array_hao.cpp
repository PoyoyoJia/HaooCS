//
// Created by hao on 2024/7/23.
//

#include "array_hao.h"

Array *arrayCreate() {
    struct Array *array = nullptr;
    array = static_cast<Array *>(malloc(sizeof(*array)));

    // 检查分配内存是否成功
    if (array == nullptr) {
        return nullptr;
    }

    /* 初始化 */
    array->p = nullptr;

    array->size = 0;
    array->typeSize = 0;
    array->len = 0;

    array->dup = nullptr;
    array->free = nullptr;
    array->match = nullptr;

    return array;
}

void arrayInit(Array *array, int size, int typeSize) {
    // 外部数据校验
    if (nullptr == array || size <= 0 || typeSize <= 0) {
        // 外部数据异常
//        throw std::exception();
        return;
    }

    void *data = calloc(1, size * typeSize);
    if (nullptr == data) {
        // 内存分配异常
//        throw std::exception();
        return;
    }

    array->size = size;
    array->p = data;
    array->len = 0;
    array->typeSize = typeSize;
}
