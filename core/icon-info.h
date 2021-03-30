//
// Created by dingjing on 2021/3/30.
//

#ifndef LIBCORE_DINGJING_ICON_INFO_H
#define LIBCORE_DINGJING_ICON_INFO_H

#include <QIcon>
#include <mutex>
#include <memory>
#include <gio/gio.h>
#include <forward_list>
#include <unordered_map>

#include "global.h"
#include "smart-ptr.hpp"

/**
 * @brief
 *
 * @brief class EXPORT_IMPORT_LIBRARY IconInfo : public std::enable_shared_from_this<IconInfo>
 * enable_shared_from_this是一个模板类，定义于头文件<memory>
 * 说明: std::enable_shared_from_this 能让一个对象（假设其名为 t ，且已被一个 std::shared_ptr 对象 pt 管理）
 *        安全地生成其他额外的 std::shared_ptr 实例（假设名为 pt1, pt2, ... ） ，它们与 pt 共享对象 t 的所有权。
 * 使用场合: 当类A被share_ptr管理，且在类A的成员函数里需要把当前类对象作为参数传给其他函数时，就需要传递一个指向自身的share_ptr。
 */

namespace dingjing
{
class EXPORT_IMPORT_LIBRARY IconInfo : public std::enable_shared_from_this<IconInfo>
{
public:
    friend class IconEngine;

    explicit IconInfo() {}

    explicit IconInfo(const char* name);

    explicit IconInfo(const GIconPtr gicon);

    ~IconInfo();

    static std::shared_ptr<const IconInfo> fromName(const char* name);

    static std::shared_ptr<const IconInfo> fromGIcon(GIconPtr gicon);

    static std::shared_ptr<const IconInfo> fromGIcon(GIcon* gicon) {
        return fromGIcon(GIconPtr{gicon, true});
    }

    static void updateQIcons();

    GIconPtr gicon() const {
        return gicon_;
    }

    QIcon qicon() const;

    bool hasEmblems() const {
        return G_IS_EMBLEMED_ICON(gicon_.get());
    }

    std::forward_list<std::shared_ptr<const IconInfo>> emblems() const;

    bool isValid() const {
        return gicon_ != nullptr;
    }

private:

    static QList<QIcon> qiconsFromNames(const char* const* names);

    // actual QIcon loaded by QIcon::fromTheme
    QIcon internalQicon() const;

    struct GIconHash {
        std::size_t operator()(GIcon* gicon) const {
            return g_icon_hash(gicon);
        }
    };

    struct GIconEqual {
        bool operator()(GIcon* gicon1, GIcon* gicon2) const {
            return g_icon_equal(gicon1, gicon2);
        }
    };

private:
    GIconPtr gicon_;
    mutable QIcon qicon_;
    mutable QList<QIcon> internalQicons_;

    static std::unordered_map<GIcon*, std::shared_ptr<IconInfo>, GIconHash, GIconEqual> cache_;
    static std::mutex mutex_;
    static QList<QIcon> fallbackQicons_;
};
};

Q_DECLARE_METATYPE(std::shared_ptr<const dingjing::IconInfo>)


#endif //LIBCORE_DINGJING_ICON_INFO_H
