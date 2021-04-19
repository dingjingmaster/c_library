# 常用C库整理 <a href="https://www.travis-ci.org/dingjingmaster/c_library"><img src="https://www.travis-ci.org/dingjingmaster/c_library.svg?branch=master" alt="status"/></a>

### 说明

本仓库主要是我工作中遇到的一些常用库，以及对一些开源库的简单单元测试，目的主要为了学习这些库的使用

### 项目结构

- `test/`       是测试用例
- `package/`    方便使用会产生 `.a` 静态库以及必须的头文件
- `gtest/`      google test
- `其它目录/`   每个目录都是一个C库，不会相互依赖

### 规范

> 仅按我的喜好做到代码统一
> 仅列出必须遵守的，且不断完善和慢慢修改

- [ ] 函数实现地方的花括号`{}`必须换行,程序内部的不换行,便于vim折叠代码
- [ ] c变量、函数命名不能出现大写字母，大写字母仅适用于常量和枚举
- [ ] 自定义类型必须以`_t`结尾，以便于区分
- [ ] gobject 类每个首字母大写,没有`_t`
- [ ] 函数声明地方必须注明输入与输出以及返回值大致情况
- [ ] 每个`.h`和`.c`必须对应一个`_demo.c`文件，这是一个例子,并不是测试
- [ ] 以后要加测试

### gnome目录下依赖的开源库

- [x] glibtop-2.0
- [x] gobject-2.0

