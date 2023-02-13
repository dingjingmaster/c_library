#ifndef LOG_H
#define LOG_H
#include <glib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG(...) \
    g_log_structured(G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,       \
                    "FILE", __FILE__,                       \
                    "LINE", __LINE__,                       \
                    "FUNC", __FUNCTION__,                   \
                    "MESSAGE", __VA_ARGS__);


#define LOG_DEBUG(...) \
    g_log_structured(G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,       \
                    "FILE", __FILE__,                       \
                    "LINE", __LINE__,                       \
                    "FUNC", __FUNCTION__,                   \
                    "MESSAGE", __VA_ARGS__);


#define INFO(...) \
    g_log_structured(G_LOG_DOMAIN, G_LOG_LEVEL_INFO,        \
                    "FILE", __FILE__,                       \
                    "LINE", __LINE__,                       \
                    "FUNC", __FUNCTION__,                   \
                    "MESSAGE", __VA_ARGS__);


#define LOG_INFO(...) \
    g_log_structured(G_LOG_DOMAIN, G_LOG_LEVEL_INFO,        \
                    "FILE", __FILE__,                       \
                    "LINE", __LINE__,                       \
                    "FUNC", __FUNCTION__,                   \
                    "MESSAGE", __VA_ARGS__);


#define WARNING(...) \
    g_log_structured(G_LOG_DOMAIN, G_LOG_LEVEL_WARNING,     \
                    "FILE", __FILE__,                       \
                    "LINE", __LINE__,                       \
                    "FUNC", __FUNCTION__,                   \
                    "MESSAGE", __VA_ARGS__);


#define LOG_WARNING(...) \
    g_log_structured(G_LOG_DOMAIN, G_LOG_LEVEL_WARNING,     \
                    "FILE", __FILE__,                       \
                    "LINE", __LINE__,                       \
                    "FUNC", __FUNCTION__,                   \
                    "MESSAGE", __VA_ARGS__);


#define ERROR(...) \
    g_log_structured(G_LOG_DOMAIN, G_LOG_LEVEL_ERROR,       \
                    "FILE", __FILE__,                       \
                    "LINE", __LINE__,                       \
                    "FUNC", __FUNCTION__,                   \
                    "MESSAGE", __VA_ARGS__);


#define LOG_ERROR(...) \
    g_log_structured(G_LOG_DOMAIN, G_LOG_LEVEL_ERROR,       \
                    "FILE", __FILE__,                       \
                    "LINE", __LINE__,                       \
                    "FUNC", __FUNCTION__,                   \
                    "MESSAGE", __VA_ARGS__);


#define DIE(...) \
{                                                           \
    g_log_structured(G_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL,    \
                    "FILE", __FILE__,                       \
                    "LINE", __LINE__,                       \
                    "FUNC", __FUNCTION__,                   \
                    "MESSAGE", __VA_ARGS__);                \
    exit(1);                                                \
}


#define LOG_RAW(level, file, line, func, msg) \
    g_log_structured(G_LOG_DOMAIN, level,                   \
                    "FILE", file,                           \
                    "LINE", line,                           \
                    "FUNC", func,                           \
                    "MESSAGE", msg)


#define DIE_BEFORE_LOG_INIT(...) \
{                                                           \
    fprintf (stderr, __VA_ARGS__);                          \
    exit(-1);                                               \
}

void signal_handler (int sig, siginfo_t* siginfo, void* context);

GLogWriterOutput log_handler(GLogLevelFlags level, const GLogField *fields, gsize nFields, gpointer udata);

#ifdef __cplusplus
}
#endif
#endif // LOG_H
