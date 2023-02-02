#ifndef LOG_H
#define LOG_H
#include <glib.h>

#define DEBUG(...) \
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

#define WARNING(...) \
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

GLogWriterOutput log_handler (GLogLevelFlags level, const GLogField* fields, gsize nFields, gpointer udata);

#endif // LOG_H
