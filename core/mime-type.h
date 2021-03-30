//
// Created by dingjing on 2021/3/30.
//

#ifndef LIBCORE_DINGJING_MIMETYPE_H
#define LIBCORE_DINGJING_MIMETYPE_H

#include <glib.h>
#include <gio/gio.h>

#include <mutex>
#include <memory>
#include <vector>
#include <string>
#include <cstring>
#include <functional>
#include <forward_list>

#include "global.h"
#include "icon-info.h"
#include "thumbnailer.h"
#include "smart-ptr.hpp"

namespace dingjing
{
    class EXPORT_IMPORT_LIBRARY MimeType
    {
    public:
        friend class Thumbnailer;

        explicit MimeType(const char* typeName);

        MimeType() = delete;

        ~MimeType();

        std::shared_ptr<const Thumbnailer> firstThumbnailer() const
        {
            std::lock_guard<std::mutex> lock{mutex_};
            return thumbnailers_.empty() ? nullptr : thumbnailers_.front();
        }

        void forEachThumbnailer(std::function<bool(const std::shared_ptr<const Thumbnailer>&)> func) const
        {
            std::lock_guard<std::mutex> lock{mutex_};
            for(auto& thumbnailer: thumbnailers_) {
                if(func(thumbnailer)) {
                    break;
                }
            }
        }

        const std::shared_ptr<const IconInfo>& icon() const
        {
            return icon_;
        }

        const char* name() const
        {
            return name_.get();
        }

        const char* desc() const
        {
            if(!desc_) {
                desc_ = CStrPtr{g_content_type_get_description(name_.get())};
            }
            return desc_.get();
        }

        static std::shared_ptr<const MimeType> fromName(const char* typeName);

        static std::shared_ptr<const MimeType> guessFromFileName(const char* fileName);

        bool isUnknownType() const
        {
            return g_content_type_is_unknown(name_.get());
        }

        bool isDesktopEntry() const
        {
            return this == desktopEntry().get();
        }

        bool isText() const
        {
            return g_content_type_is_a(name_.get(), "text/plain");
        }

        bool isImage() const
        {
            return !std::strncmp("image/", name_.get(), 6);
        }

        bool isMountable() const
        {
            return this == inodeMountPoint().get();
        }

        bool isShortcut() const
        {
            return this == inodeShortcut().get();
        }

        bool isDir() const
        {
            return this == inodeDirectory().get();
        }

        bool canBeExecutable() const
        {
            return g_content_type_can_be_executable(name_.get());
        }

        static std::shared_ptr<const MimeType> inodeDirectory()
        {
            // inode/directory
            if(!inodeDirectory_)
                inodeDirectory_ = fromName("inode/directory");
            return inodeDirectory_;
        }

        static std::shared_ptr<const MimeType> inodeShortcut()
        {
            // inode/x-shortcut
            if(!inodeShortcut_)
                inodeShortcut_ = fromName("inode/x-shortcut");
            return inodeShortcut_;
        }

        static std::shared_ptr<const MimeType> inodeMountPoint()
        {
            // inode/mount-point
            if(!inodeMountPoint_)
                inodeMountPoint_ = fromName("inode/mount-point");
            return inodeMountPoint_;
        }

        static std::shared_ptr<const MimeType> desktopEntry()
        {
            // application/x-desktop
            if(!desktopEntry_)
                desktopEntry_ = fromName("application/x-desktop");
            return desktopEntry_;
        }

    private:
        void removeThumbnailer(std::shared_ptr<const Thumbnailer>& thumbnailer)
        {
            std::lock_guard<std::mutex> lock{mutex_};
            thumbnailers_.remove(thumbnailer);
        }

        void addThumbnailer(std::shared_ptr<const Thumbnailer> thumbnailer)
        {
            std::lock_guard<std::mutex> lock{mutex_};
            thumbnailers_.push_front(std::move(thumbnailer));
        }

    private:
        std::shared_ptr<const IconInfo> icon_;
        CStrPtr name_;
        mutable CStrPtr desc_;
        std::forward_list<std::shared_ptr<const Thumbnailer>> thumbnailers_;
        static std::unordered_map<const char*, std::shared_ptr<const MimeType>, CStrHash, CStrEqual> cache_;
        static std::mutex mutex_;

        static std::shared_ptr<const MimeType> inodeDirectory_;  // inode/directory
        static std::shared_ptr<const MimeType> inodeShortcut_;  // inode/x-shortcut
        static std::shared_ptr<const MimeType> inodeMountPoint_;  // inode/mount-point
        static std::shared_ptr<const MimeType> desktopEntry_; // application/x-desktop
    };
}




#endif //LIBCORE_DINGJING_MIMETYPE_H
