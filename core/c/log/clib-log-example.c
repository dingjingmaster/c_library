#include <stdio.h>
#include <string.h>
#include "clib-log.h"

int main() {
    int i = 0;
    /* 控制台输出 */
    log_init(LOG_TYPE_CONSOLE, LOG_VERB, LOG_ROTATE_TRUE, 2 << 20,
                 NULL, NULL, NULL);
    loge("错误示例1");
    loge("错误示例2");
    loge("错误示例3");
    loge("错误示例4");
    logw("警告示例1");
    logw("警告示例2");
    logw("警告示例3");
    logw("警告示例4");
    logi("info示例1");
    logi("info示例2");
    logi("info示例3");
    logi("info示例4");
    logd("debug示例1");
    logd("debug示例2");
    logd("debug示例3");
    logd("debug示例4");
    logv("详细信息示例1");
    logv("详细信息示例2");
    logv("详细信息示例3");
    logv("详细信息示例4");
    log_destroy();


    /* 文件输出 */
    log_init(LOG_TYPE_FILE, LOG_VERB, LOG_ROTATE_TRUE, 1 << 10,
                 "日志文件夹", "日志", NULL);

    for(i = 0; i < 2; ++i) {
            loge("./测试:%d", i);
    }
    for(i = 0; i < 10000; ++i) {
        loge("测试:%d", i);
    }
    for(i = 0; i < 10000; ++i) {
        logw("测试:%d", i);
    }
    for(i = 0; i < 10000; ++i) {
        logi("测试:%d", i);
    }
    for(i = 0; i < 10000; ++i) {
        logd("测试:%d", i);
    }
    for(i = 0; i < 10000; ++i) {
        logv("测试:%d", i);
    }
    return 0;
}
