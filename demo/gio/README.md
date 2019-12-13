# gio

### 命令说明

  - help     打印帮助
  - version  打印版本
  - cat      串接文件，写到标准输出
  - copy     复制文件
  - info     显示关于位置的信息
  - list     列出某位置的内容
  - mime     获取或设置某种 MIME 类型的处理程序
  - mkdir    创建目录
  - monitor  监视文件和目录更改
  - mount    挂载或卸载位置
  - move     移动文件
  - open     用默认应用打开文件
  - rename   重命名文件
  - remove   删除文件
  - save     从标准输入读取并保存
  - set      设置文件属性
  - trash    移动文件或目录到回收站
  - tree     在树中列出某位置的内容

### GIO

> GIO正在努力为桌面应用程序（如网络和D-Bus支持）提供一个现代的、易于使用的VFS API，它位于库堆栈的正确级别。目标是克服GnomeVFS的缺点，并提供一个非常好的API，使得开发人员更喜欢它而不是原始的POSIX调用。其中包括使用GObject。它还意味着不克隆POSIX API，而是提供更高级别的、以文档为中心的接口。 

1. GIO的抽象文件系统模型由许多I/O和文件的接口和基类组成： 

    - GFile  对文件的引用
    - GFileInfo  文件或文件系统相关的信息
    - GFileEnumerator 列出文件夹中的文件
    - GDrive 表示驱动器
    - GVolume 表示文件系统
    - GMount 表示已挂载的文件系统

2. 输入输出流

    - GInputStream 读取数据
    - GOutputStream 写数据
    - GIOStream 读/写数据
    - GSeekable 流定位

3. 应用程序及处理文件类型相关的接口

    - GAppinfo 有关已安装应用程序的信息
    - GIcon 文件和应用程序图标的抽象类型

4. 用于存储和检索应用程序设置的框架

    - GSetting 存储和检索应用程序设置

5. 网络编程、连接监控、名称解析、低级套接字API和高级客户端、高级帮助类

    - GSocket 底层平台无关的套接字
    - Gresolver 异步和可取消的DNS解析器
    - GSocketClient 高级别的网络客户端
    - GSocketService 高级别的网络服务端
    - GSocketConnection 网络连接流
    - GNetworkMonitor 网络连接监控

6. 连接到D-Bus，发送和接收信息，监视总线名称和使对象在总线上可用

    - GDBusConnection D-Bus连接
    - GDbusMethodInvocation 处理远程呼叫
    - GDBusServer 接受连接
    - GDBusProxy 远程对象接入D-Bus接口的代理



### GIOModule

> 提供用于加载和卸载模块的接口和默认函数。这在内部用于使GIO可扩展，但也可以被其他人用于实现模块加载。
