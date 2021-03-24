//
// Created by dingjing on 2021/3/24.
//

#ifndef LIBCORE_DINGJING_CSTRPTR_H
#define LIBCORE_DINGJING_CSTRPTR_H
#include <glib.h>
#include <memory.h>

/**
 * @brief unique_ptr 是 C++11 提供的智能指针实现，防止内存泄漏，其包含一个原始指针，并负责其生命周期，对象销毁时候，释放指针内存
 * unique_ptr 重载了 '->' 和 '*' 运算符，使用类似普通指针。
 * unique_ptr对象始终是关联的原始指针的唯一所有者。 我们不能复制一个unique_ptr对象，它只能移动。
 * 由于每个unique_ptr对象是原始指针的唯一拥有者，因此在其析构函数中直接删除关联的指针。不需要任何参考计数，因此它非常轻量。
 * 传递所有权 std::unique_ptr<Task> taskPtr4 = std::move(taskPtr2);
 */

namespace dingjing
{
    struct CStrDeleter {
        void operator()(char* ptr) const {
            g_free (ptr);
        }
    };

    typedef std::unique_ptr<char[], CStrDeleter> CStrPtr;

    struct CStrHash {
        std::size_t operator()(const char* str) const {
            return g_str_hash(str);
        }
    };

    struct CStrEqual {
        bool operator()(const char* str1, const char* str2) const {
            return g_str_equal(str1, str2);
        }
    };

    struct CStrVDeleter {
        void operator()(char** ptr) const {
            g_strfreev(ptr);
        }
    };

    typedef std::unique_ptr<char*[], CStrVDeleter> CStrArrayPtr;
}


#endif //LIBCORE_DINGJING_CSTRPTR_H
