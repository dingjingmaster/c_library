//
// Created by dingjing on 2021/3/30.
//

#include "file-path.h"
#include <utility>

namespace dingjing
{

FilePath FilePath::homeDir_;

const FilePath &FilePath::homeDir()
{
    if(!homeDir_) {
        const char* home = getenv("HOME");
        if(!home) {
            home = g_get_home_dir();
        }
        homeDir_ = FilePath::fromLocalPath(home);
    }
    return homeDir_;
}
}