//
// Created by dingjing on 2021/3/24.
//

#ifndef LIBCORE_DINGJING_GLOBAL_H
#define LIBCORE_DINGJING_GLOBAL_H

#ifdef signals
#undef signals
#endif

#ifdef COMPILE_LIBRARY
#define EXPORT_IMPORT_LIBRARY Q_DECL_EXPORT
#else
#define EXPORT_IMPORT_LIBRARY Q_DECL_IMPORT
#endif

#endif //LIBCORE_DINGJING_GLOBAL_H
