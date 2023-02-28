//
// Created by dingjing on 23-2-28.
//

#ifndef ESAFE_DSM_KERNEL_LOG_H
#define ESAFE_DSM_KERNEL_LOG_H
#include <linux/printk.h>

#define LOG_FMT(fmt)    fmt
#define LOG_DEBUG(fmt,...)          \
    printk(KERN_DEBUG   "DSM [DEBUG] [%s:%d] " LOG_FMT(fmt) "\n", __FILE__, __LINE__, ##__VA_ARGS__);

#define LOG_INFO(fmt,...)           \
    printk(KERN_INFO    "DSM [INFO] [%s:%d] " LOG_FMT(fmt) "\n", __FILE__, __LINE__, ##__VA_ARGS__);

#define LOG_ERROR(fmt,...)          \
    printk(KERN_ERR     "DSM [ERROR] [%s:%d] " LOG_FMT(fmt) "\n", __FILE__, __LINE__, ##__VA_ARGS__);

#define LOG_WARNING(fmt,...)        \
    printk(KERN_WARNING "DSM [ERROR] [%s:%d] " LOG_FMT(fmt) "\n", __FILE__, __LINE__, ##__VA_ARGS__);

#endif //ESAFE_DSM_KERNEL_LOG_H
