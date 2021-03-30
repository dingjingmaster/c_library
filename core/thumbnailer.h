//
// Created by dingjing on 2021/3/30.
//

#ifndef LIBCORE_DINGJING_THUMBNAILER_H
#define LIBCORE_DINGJING_THUMBNAILER_H

#include <mutex>
#include <vector>
#include <memory>
#include <unordered_map>

#include "global.h"
#include "smart-ptr.hpp"

namespace dingjing
{
    class MimeType;

    class Thumbnailer
    {
    public:
        Thumbnailer(const char *id, GKeyFile *kf);

        CStrPtr commandForUri(const char* uri, const char* output_file, guint size) const;

        bool run(const char* uri, const char* output_file, int size) const;

        static void loadAll();

    private:
        CStrPtr id_;
        CStrPtr try_exec_; /* FIXME: is this useful? */
        CStrPtr exec_;
        //std::vector<std::shared_ptr<const MimeType>> mimeTypes_;

        static std::mutex mutex_;
        static std::vector<std::shared_ptr<Thumbnailer>> allThumbnailers_;
    };
}

#endif //LIBCORE_DINGJING_THUMBNAILER_H
