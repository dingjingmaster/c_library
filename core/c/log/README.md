### c 日志库

### `clib_log.h clib_log.c`

> 注意：
>
> ​	本日志库支持多线程，已经确保多线程中只会有一个线程完成日志库的初始化，日志输出要么是控制台，要么是文件，暂不支持其它选项。

1. 日志初始化

   ```c
   int log_init(log_type_t type, log_level_t level, log_rotate_t rotate, unsigned long long log_size, const char* dir, const char* prefix, const char* suffix);
   ```

   参数：

   	- type：log 输出位置，参见 `log_type_t`
   	- level：日志输出级别，参见 `log_level_t`
   	- rotate：是否切分文件，参见 `log_rotate_t`
   	- log_size: 每个文件字节数
   	- dir：日志文件夹路径
   	- prefix：日志文件名
   	- suffix：日志文件扩展名

2. 日志销毁

   ```c
   void log_destroy(void);
   ```

3. 日志打印

   ```c
   loge(); // 错误
   logw(); // 警告
   logi(); // 信息
   logd(); // 调试
   logv(); // 详细信息
   ```

   