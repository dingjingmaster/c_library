//
// Created by dingjing on 2021/3/24.
//

#ifndef LIBCORE_DINGJING_SMART_PTR_HPP
#define LIBCORE_DINGJING_SMART_PTR_HPP
#include "global.h"

#include <glib.h>
#include <memory>
#include <QString>
#include <gio/gio.h>
#include <glib-object.h>

/**
 * @brief unique_ptr 是 C++11 提供的智能指针实现，防止内存泄漏，其包含一个原始指针，并负责其生命周期，对象销毁时候，释放指针内存
 * unique_ptr 重载了 '->' 和 '*' 运算符，使用类似普通指针。
 * unique_ptr对象始终是关联的原始指针的唯一所有者。 我们不能复制一个unique_ptr对象，它只能移动。
 * 由于每个unique_ptr对象是原始指针的唯一拥有者，因此在其析构函数中直接删除关联的指针。不需要任何参考计数，因此它非常轻量。
 * 传递所有权 std::unique_ptr<Task> taskPtr4 = std::move(taskPtr2);
 */

namespace dingjing
{
struct CStrDeleter
{
    void operator()(char* ptr) const {
        g_free (ptr);
    }
};

typedef std::unique_ptr<char[], CStrDeleter> CStrPtr;

struct CStrHash
{
    std::size_t operator()(const char* str) const {
        return g_str_hash(str);
    }
};

struct CStrEqual
{
    bool operator()(const char* str1, const char* str2) const {
        return g_str_equal(str1, str2);
    }
};

struct CStrVDeleter
{
    void operator()(char** ptr) const {
        g_strfreev(ptr);
    }
};

typedef std::unique_ptr<char*[], CStrVDeleter> CStrArrayPtr;

template <typename T>
class GObjectPtr
{
public:
    explicit GObjectPtr(): gobj_{nullptr} {}
    explicit GObjectPtr(T* gobj, bool addRef = true) : gobj_ {gobj}
    {
        if(gobj_ != nullptr && addRef) {
            g_object_ref(gobj_);
        }
    }

    GObjectPtr(const GObjectPtr& other): gobj_{other.gobj_ ? reinterpret_cast<T*> (g_object_ref(other.gobj_)) : nullptr} {}
    GObjectPtr(GObjectPtr&& other) noexcept: gobj_{other.release()} {}

    ~GObjectPtr()
    {
        if(gobj_ != nullptr)
            g_object_unref(gobj_);
    }

    T* get() const
    {
        return gobj_;
    }

    T* release()
    {
        T* tmp = gobj_;
        gobj_ = nullptr;
        return tmp;
    }

    void reset()
    {
        if(gobj_ != nullptr) {
            g_object_unref(gobj_);
        }
        gobj_ = nullptr;
    }

    GObjectPtr& operator = (const GObjectPtr& other)
    {
        if (*this == other)
            return *this;

        if(gobj_ != nullptr)
            g_object_unref(gobj_);
        gobj_ = other.gobj_ ? reinterpret_cast<T*>(g_object_ref(other.gobj_)) : nullptr;
        return *this;
    }

    GObjectPtr& operator = (GObjectPtr&& other) noexcept
    {
        if (this == &other)
            return *this;

        if(gobj_ != nullptr)
            g_object_unref(gobj_);
        gobj_ = other.release();

        return *this;
    }

    GObjectPtr& operator = (T* gobj)
    {
        if (*this == gobj)
            return *this;

        if(gobj_ != nullptr)
            g_object_unref(gobj_);
        gobj_ = gobj ? reinterpret_cast<T*>(g_object_ref(gobj_)) : nullptr;
        return *this;
    }

    bool operator == (const GObjectPtr& other) const
    {
        return gobj_ == other.gobj_;
    }

    bool operator == (T* gobj) const
    {
        return gobj_ == gobj;
    }

    bool operator != (std::nullptr_t) const
    {
        return gobj_ != nullptr;
    }

    operator bool() const
    {
        return gobj_ != nullptr;
    }

private:
    mutable T* gobj_;
};

typedef GObjectPtr<GFile> GFilePtr;
typedef GObjectPtr<GFileInfo> GFileInfoPtr;
typedef GObjectPtr<GFileMonitor> GFileMonitorPtr;
typedef GObjectPtr<GCancellable> GCancellablePtr;
typedef GObjectPtr<GFileEnumerator> GFileEnumeratorPtr;

typedef GObjectPtr<GInputStream> GInputStreamPtr;
typedef GObjectPtr<GFileInputStream> GFileInputStreamPtr;
typedef GObjectPtr<GOutputStream> GOutputStreamPtr;
typedef GObjectPtr<GFileOutputStream> GFileOutputStreamPtr;

typedef GObjectPtr<GIcon> GIconPtr;

typedef GObjectPtr<GVolumeMonitor> GVolumeMonitorPtr;
typedef GObjectPtr<GVolume> GVolumePtr;
typedef GObjectPtr<GMount> GMountPtr;

typedef GObjectPtr<GAppInfo> GAppInfoPtr;

class GErrorPtr
{
public:
    GErrorPtr(): err_{nullptr} {}

    GErrorPtr(GError*&& err) noexcept: err_{err}
    {
        err = nullptr;
    }

    GErrorPtr(const GErrorPtr& other) = delete;

    GErrorPtr(GErrorPtr&& other) noexcept: err_{other.err_}
    {
        other.err_ = nullptr;
    }

    GErrorPtr(std::uint32_t domain, unsigned int code, const char* msg):
            GErrorPtr{g_error_new_literal(domain, code, msg)} {}

    GErrorPtr(std::uint32_t domain, unsigned int code, const QString& msg):
            GErrorPtr{domain, code, msg.toUtf8().constData()} {}

    ~GErrorPtr()
    {
        reset();
    }

    std::uint32_t domain() const
    {
        if(err_ != nullptr) {
            return err_->domain;
        }
        return 0;
    }

    unsigned int code() const
    {
        if(err_ != nullptr) {
            return err_->code;
        }
        return 0;
    }

    QString message() const
    {
        if(err_ != nullptr) {
            return QString::fromUtf8(err_->message);
        }
        return QString();
    }

    void reset()
    {
        if(err_) {
            g_error_free(err_);
        }
        err_ = nullptr;
    }

    GError* get() const
    {
        return err_;
    }

    GErrorPtr& operator = (const GErrorPtr& other) = delete;

    GErrorPtr& operator = (GErrorPtr&& other) noexcept {
        reset();
        err_ = other.err_;
        other.err_ = nullptr;
        return *this;
    }

    GErrorPtr& operator = (GError*&& err) {
        reset();
        err_ = err;
        return *this;
    }

    GError** operator&() {
        return &err_;
    }

    GError* operator->() {
        return err_;
    }

    const GError* operator->() const {
        return err_;
    }

    bool operator == (const GErrorPtr& other) const {
        return err_ == other.err_;
    }

    bool operator == (GError* err) const {
        return err_ == err;
    }

    bool operator != (std::nullptr_t) const {
        return err_ != nullptr;
    }

    operator bool() const {
        return err_ != nullptr;
    }

private:
    GError* err_;
};
}


#endif // LIBCORE_DINGJING_SMART_PTR_HPP
