### 说明

本仓库主要是我工作中遇到的一些常用库，以及对一些开源库的简单单元测试，目的主要为了学习这些库的使用

### 项目结构

- `test/`       是测试用例
- `package/`    方便使用会产生 `.so` 静态库以及必须的头文件
- `gtest/`      google test
- `其它目录/`   每个目录都是一个C库，不会相互依赖

### make 命令

- `make test` 生成可执行测试程序

```shell
make test
```

- `make 或 make all` 生成 `静态库`、`头文件`、`代码示例` 都会放到 `package` 文件夹下

```shell
make
```
- `make debug` 生成带有调试信息的程序

```shell
make debug
```

- `make clean` 清空非源码文件

- `make` 或 `make all` 生成demo及静态库文件

- `make install` 安装静态库

