//
// Created by hao on 2024/7/23.
//

#pragma once

#include <iostream>
#include <exception>
#include <cstdio>
#include <cstdlib>

namespace Haoo {
    /**
     * @brief 数组类型定义
     * @tparam T 数组的数据类型
     */
    template <class Type>
    class Array {
    private:
        /**
         * @brief 存放数据的地址
         */
        void* dataPtr;

        /**
         * @brief 存放数据的指针
         */
        Type* data;

        /**
         * @brief 数据指针的空间大小
         */
        size_t maxLen;

        /**
         * @brief 已经使用的空间大小
         */
        size_t count;

        /**
         * @brief 数据类型的长度
         */
        size_t typeSize;

    public:
        /**
         * @brief 值复制函数
         */
        void(*dup)(void* ptr, void* key);

        /**
         * @brief 值释放函数
         */
        void(*free)(void* ptr);

        /**
         * @brief 值比较函数
         */
        int(*match)(void* ptr, void* key);

    public:
        /**
         * @brief 默认构造函数，设置最大长度为0，数据类型大小为0，后续使用 Init 函数进行初始化
         */
        Array() :
            dataPtr(nullptr),
            data(nullptr),
            maxLen(0),
            count(0),
            typeSize(0) {
        }

        /**
         * @brief 初始化构造函数
         * @param maxLen 数组的最大长度
         * @param typeSize 数组的数据类型长度
         * @param initVal 数组的初始化值
         */
        Array(size_t maxLen, const Type* const initVal) {
            this->Init(maxLen, initVal);
        }

        /**
         * @brief 析构函数，释放请求的内存（如果有）
         */
        ~Array() {
            if (this->dataPtr) {
                //free(this->dataPtr); // 出错：0xC0000005: 执行位置 0x0000000000000000 时发生访问冲突。
                delete this->dataPtr; // 通过
                
                /*this->dataPtr = nullptr;
                this->data = nullptr;*/
            }
        }

        /**
         * @brief 获取数组目前已使用的大小
         * @return 已使用的大小
         */
        size_t Count() {
            return this->count;
        }

        /**
         * @brief 获取数组的数据类型的长度
         * @return 数据类型长度
         */
        size_t TypeSize() {
            return this->typeSize;
        }

        /**
         * @brief 返回数组的大小
         * @return 数组的大小
         */
        size_t Size() {
            return this->maxLen;
        }

        /**
         * @brief 数值化数组，当且仅当数组未被初始化（即使用默认构造）时使用
         * @param maxLen 数组的最大长度
         * @param typeSize 数组的数据类型长度
         * @param initVal 数组的初始化值
         */
        void Init(size_t maxLen, const Type* const initVal) {
            if (IsInit()) {
                throw std::exception("数组不能重复初始化，请使用ReSet函数");
            }

            if (maxLen <= 1) {
                throw std::exception("数组最大长度设置不合法");
            }

            this->maxLen = maxLen;
            this->count = 0;
            this->typeSize = sizeof(Type);
            this->dataPtr = malloc(sizeof(Type) * maxLen);

            if (this->dataPtr == nullptr) {
                throw std::exception("数组内存分配失败");
            }

            this->data = static_cast<Type*>(this->dataPtr);

            // 设置初始值
            if (initVal) {
                for (int idx = 0; idx < this->maxLen; ++idx) {
                    Type* ptr = this->data + idx;
                    *ptr = *initVal;
                }
            }
        }

        /**
         * @brief 指定位置插入数据，当数组未初始化或下标设置不合理时抛出异常
         * @param pos 要插入数据的下标
         * @param value 要插入的数据的引用
         * @return 插入成功与否
         */
        bool Insert(size_t pos, const Type& value, bool throwEx = false) {
            if (!IsInit()) {
                if (throwEx) {
                    throw std::exception("未初始化，无法操作");
                }

                return false;
            }

            try {
                CheckInsert(pos);
            } catch (std::exception& ex) {
                if (throwEx) {
                    throw ex;
                }

                return false;
            }

            Type* posPtr = this->data + pos;
            *posPtr = value;

            return true;
        }

        /**
         * @brief 获取指定位置的数据的引用，当数组未初始化或下标设置不合理时抛出异常
         * @param pos 要获取数据的下标
         * @return 获取数据的引用
         */
        Type& Get(size_t pos) {
            if (!IsInit()) {
                throw std::exception("未初始化，无法操作");
            }

            CheckInsert(pos);

            Type* posPtr = this->data + pos;
            return *posPtr;
        }

        /**
         * @brief 重载索引运算符
         * @param pos 索引的位置
         * @return 获取数据的引用
         */
        Type& operator[] (size_t pos) {
            return Get(pos);
        }

        /**
         * @brief 指定位置插入数据
         * @param pos 要插入数据的下标
         * @param value 要插入的数据的指针
         * @return 插入成功与否
         */
        bool Insert(size_t pos, Type* const value) {
            if (!CheckInsert(pos)) {
                return false;
            }

            Type* posPtr = this->data + pos;
            if (NULL != this->dup)
            {
                this->dup(posPtr, value);
            }
            else
            {
                memcpy(posPtr, value, this->typeSize);
            }
        }

    private:
        /**
         * @brief 判断数组是否已经初始化
         * @return 是否初始化
         */
        inline bool IsInit() {
            return this->dataPtr != nullptr;
        }

        /**
         * @brief 判断数组的下标是否合法
         * @param pos 要判断的下标
         * @param throwEx 是否抛出异常，默认为是
         * @return 是否合法
         */
        inline bool CheckInsert(size_t pos, bool throwEx = true) {
            if (pos >= maxLen) {
                if (throwEx) {
                    throw std::exception("索引位置超出数组的范围");
                }

                return false;
            }

            if (pos < 0) {
                if (throwEx) {
                    throw std::exception("索引位置不能小于0");
                }

                return false;
            }

            return true;
        }

    public:
        friend std::ostream& operator<< (std::ostream& os, const Array<Type>* array) {
            os << "array {\n\tmax size: " << array->maxLen << "\n\tcount: " << array->count << "\n\tdata:[";
            for (int idx = 0; idx < array->maxLen; ++idx) {
                Type* ptr = array->data;

                if (idx) {
                    os << ", ";
                }

                if (ptr == nullptr) {
                    os << "NULL";
                }
                else {
                    os << *ptr;
                }
            }

            os << "]\n}";

            return os;
        }

        friend std::ostream& operator<< (std::ostream& os, const Array<Type>& array) {
            os << "array {\n\tmax size: " << array.maxLen << "\n\tcount: " << array.count << "\n\tdata: [";
            for (int idx = 0; idx < array.maxLen; ++idx) {
                Type* ptr = array.data + idx;

                if (idx) {
                    os << ", ";
                }

                if (ptr == nullptr) {
                    os << "NULL";
                }
                else {
                    os << *ptr;
                }
            }

            os << "]\n}";

            return os;
        }
    };
}
