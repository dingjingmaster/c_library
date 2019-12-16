# gnome 全家桶

## GIO

#### 工具 —— gio-querymodules

#### 工具 —— gsettings

> 用来获取和修改键值，大多数命令需要 `SCHEMA` 和 `KEY` 来操作值。 当 `SCHEMA` 不在固定目录目录下时，`SCHEMA`需要一个`:PATH`后缀
> gsetting 需要一个D-Bus会话总线连接来将更改写入dconf数据库

```shell
    gsettings get SCHEMA [:PATH] KEY
    gsettings monitor SCHEMA [:PATH] [KEY]
    gsettings writable SCHEMA [:PATH] KEY
    gsettings range SCHEMA [:PATH] KEY
    gsettings describe SCHEMA [:PATH] KEY
    gsettings set SCHEMA [:PATH] KEY VALUE
    gsettings reset SCHEMA [:PATH] KEY
    gsettings reset-recursively SCHEMA [:PATH]
    gsettings list-schemas [--print-paths]
    gsettings list-relocatable-schemas
    gsettings list-keys SCHEMA [:PATH]
    gsettings list-children SCHEMA [:PATH]
    gsettings list-recursively [SCHEMA [:PATH]]
    gsettings help [COMMAND]
```

#### 工具 —— glib-compile-schemas

> 将目录中所有GSettings XML 模式文件编译成一个名为gshema.compile的二进制文件，GSettings可以使用这个文件。
> 要编译的 XML 文件必须由扩展名
> 该工具检查 `XDG_DATA_DIRS` 环境变量中指定的所有目录的 `glib-2.0/schemas`，一般路径是 `/usr/share/glib2.0/schemas`
> 扩展名`gshema.override`中的键值覆盖默认的键值，要覆盖的文件以`nn_`开头，数字越大优先级越高

```shell
    帮助选项：
    -h, --help                显示帮助选项

    应用程序选项：
    --version                 显示程序版本并退出
    --targetdir=DIRECTORY     Where to store the gschemas.compiled file
    --strict                  在方案出现任何错误时中止
    --dry-run                 不要对 gschema.compiled 进行写操作
    --allow-any-name          不要强制键名的限制
```

#### 工具 —— glib-compile-resources

> 从文件及其引用的文件中读取资源描述，并创建适合与GResource API 一起使用的二进制资源包。然后按原样或作为链接到应用程序的C源代码写出结果包。

```shell
    用法：
        glib-compile-resources [OPTION…] FILE

        将一份资源规格编译为资源文件。
        资源规格文件以 .gresource.xml 为扩展名，
        资源文件以 .gresource 为扩展名。

    帮助选项：
        -h, --help                   显示帮助选项

    应用程序选项：
        --version                    显示程序版本并退出
        --target=FILE                Name of the output file
        --sourcedir=DIRECTORY        The directories to load files referenced in FILE from (default: current directory)
        --generate                   以目标文件扩展名所选择的格式生成输出
        --generate-header            生成源码头文件
        --generate-source            Generate source code used to link in the resource file into your code
        --generate-dependencies      生成依赖关系列表
        --dependency-file=FILE       Name of the dependency file to generate
        --generate-phony-targets     Include phony targets in the generated dependency file
        --manual-register            Don’t automatically create and register resource
        --internal                   Don’t export functions; declare them G_GNUC_INTERNAL
        --external-data              Don’t embed resource data in the C file; assume it's linked externally instead
        --c-name                     用于生成的源代码的 C 标识符名称
```

#### 工具 —— gdbus

> 处理D-Bus对象的工具

```shell
    用法：
        gdbus COMMAND

    Commands:
        help         Shows this information
        introspect   Introspect a remote object
        monitor      Monitor a remote object
        call         Invoke a method on a remote object
        emit         Emit a signal
        wait         Wait for a bus name to appear
```

#### 工具 —— gdbus-codegen

> dbus 代码和文档生成器

#### 工具 —— gresource

> 提供了一个简单的命令行接口，允许列出并提取已编译到资源文件或包含在elf文件（二进制文件或共享库）中的资源
> 如果一个elf文件中包含多个带有资源的 section，那么可使用 `--section`查找可用的 `section`

```shell
    list        Lists resources. If SECTION is given, only list resources in this section. If PATH is given, only list matching resources.
    details     Lists resources with details. If SECTION is given, only list resources in this section. If PATH is given, only list matching resources. Details include the section, size and compression of each resource.
    extract     Extracts the resource named by PATH to stdout. Note that resources may contain binary data.
    sections    Lists sections containing resources. This is only interesting if FILE is an elf file.
    help        Prints help and exits.
```

#### 工具 —— gapplication

> 是`org.freedesktop.Application`接口的命令行实现，由 `freedesktop.org`桌面条目规范指定。
> gapplication可用于启动在`.desktop`文件中将`DBusActivatable`设置为`true`的应用程序，并可用于向其它应用程序已经运行的实例发送消息
> 程序可用在其`.desktop`文件的Exec中引用`gapplication`，来兼容不直接支持`DBusActivitable`的程序

```shell
    用法：

        gapplication COMMAND [ARGS…]

    命令：
        help           打印帮助
        version        打印版本
        list-apps      列出应用程序
        launch         启动一个应用程序
        action         激活一个动作
        list-actions   列出可用动作
```

#### 工具 —— gio

```shell
    help     打印帮助
    version  打印版本
    cat      串接文件，写到标准输出
    copy     复制文件
    info     显示关于位置的信息
    list     列出某位置的内容
    mime     获取或设置某种 MIME 类型的处理程序
    mkdir    创建目录
    monitor  监视文件和目录更改
    mount    挂载或卸载位置
    move     移动文件
    open     用默认应用打开文件
    rename   重命名文件
    remove   删除文件
    save     从标准输入读取并保存
    set      设置文件属性
    trash    移动文件或目录到回收站
    tree     在树中列出某位置的内容
```

#### GIO

> GIO正在努力为桌面应用程序（如网络和D-Bus支持）提供一个现代的、易于使用的VFS API，它位于库堆栈的正确级别。目标是克服GnomeVFS的缺点，并提供一个非常好的API，使得开发人员更喜欢它而不是原始的POSIX调用。其中包括使用GObject。它还意味着不克隆POSIX API，而是提供更高级别的、以文档为中心的接口。 

1. GIO的抽象文件系统模型由许多I/O和文件的接口和基类组成： 

    - GFile  对文件的引用 (文件、文件夹的检测、创建、删除、修改信息、打开、关闭、复制、路径操作、挂载相关、文件类型、byte load)
    - GFileInfo  文件或文件系统相关的信息（类型、是否隐藏、是否可读写执行等、是否可重命名、是否可挂载、是否unix设备、修改时间参数、uid、gid、归属用户和组...）
    - GFileAttribute key-value形式的文件属性（GFileInfo会返回这种类型）
    - GFileEnumerator 列出文件夹中的文件（对一组GFiles进行操作，为每个枚举的文件返回GFileInfo结构 -- 关联了 GFile 和 GFileInfo）
    - GDrive 表示驱动器 (来源于 GVolumeMonitor)
    - GVolume 表示文件系统 （来源于 GVolumeMonitor）
    - GMount 表示已挂载的文件系统（来源于 GVolumeMontor 名字、uuid、图标相关，由GFile得到）

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

![gnome-各库关系](pic/1.png)

#### GIOModule

> 提供用于加载和卸载模块的接口和默认函数。这在内部用于使GIO可扩展，但也可以被其他人用于实现模块加载。

#### POSIX和GIO对比

| POSIX | GIO |
| --- | --- |
| `POSIX` | `GIO` |
| `char *path` | `GFile *file` |
| `struct stat *buf` | `GFileInfo *info` |
| `struct statvfs *buf` | `GFileInfo *info` |
| `int fd` | `GInputStream *in` 与 `GOutputStream *out` |
| `DIR *` | `GFileEnumerator *enum` |
| `fstab entry` | `GUnixMountPoint *mount_point` |
| `mtab entry` | `GUnixMountEntry *mount_entry` |

#### GnomeVFS与GIO对比

| `Gnome_VFS` | `GIO` |
| --- | --- |
| `GnomeVFSUR` | `GFile` |
| `GnomeVFSFileInfo` | `GFileInfo` |
| `GnomeVFSResult` | `GError, with G_IO_ERROR values` |
| `GnomeVFSHandle & GnomeVFSAsyncHandle` | `GInputStream or GOutputStream` |
| `GnomeVFSDirectoryHandle` | `GFileEnumerator` |
| `mime type` | `content type` |
| `GnomeVFSMonitor` | `GFileMonitor` |
| `GnomeVFSVolumeMonitor` | `GVolumeMonitor` |
| `GnomeVFSVolume` | `GMount` |
| `GnomeVFSDrive` | `GVolume` |
| `-` | `GDrive` |
| `GnomeVFSContext` | `GCancellable` |
| `gnome_vfs_async_cancel` | `g_cancellable_cancel` |

## IPC

### dbus 例子

| 例子 | 说明 |
| --- | --- |
| `dbus-own-name.c` | 在DBus上注册一个名字 |
| `dbus-watch-name.c` | 在dbus的会话总线上检测指定名字的总线是否存在 |

#### 概念介绍

- dbus-daemon 一个dbus的后台守护程序，用于多应用之间消息转发
- libdbus.so dbus的功能接口
- 高层封装，如dbus-glib和QT D-bus
- dbus总线一般分为系统总线和会话总线，所以安全性很好
- dbus支持进程间一对一和一对多的对等通信
- 一个进程发送给另一个进程信息的时候，先是发给后台，由后台转发到另一个/多个进程

> 官网中的dubs安装值包含上述1和2

1. 运行一个dbus-daemon就是创建了一条通信的总线Bus，当一个application链接到这条Bus上时，就产生了一个Connection
2. 从dbus 的概念上说，通信的双方是Object，一个application可以包含多个Object
3. 一个Object中会有不同的Interface，Interface可以看作是要传输的数据
4. dbus的两种通信方式：
  
  1. signal: 也就是广播
  2. method: 是一对一通信。

5. dbus应用场景

  1. 函数调用：DBus可用实现进程间函数调用，进程A发送函数调用强求，经过总线转发至进程B。进程B将应答函数返回值或错误消息
  2. 消息广播：进程间消息广播不需要响应，接收方需要向总线注册感兴趣的消息类型，当总线接受到"Signal消息"类型的消息时，会将消息转发至希望接受的进程。

6. 程序中的各个元素

  1. address: 用来标识ibus-daemon，格式:`unix:path=/var/run/dbus/xxxx`
  2. bus name: 用来标识application的，格式：`com.xxx.xxx`
  3. path: 用于标识Object，格式：`/xxx/xxx/xxx`
  4. name: 每个interface都有自己的名字，通过interface的名字找到interface，格式：`xxx.xxx.xxx.xxx`
  5. signal 和 method也有自己的名字，随便取名就行

#### 术语

- connection: 用于打开到守护进程的连接的结构，可以通过指定`DBUS_BUS_SYSTEM`来打开系统总线守护进程，也可以使用`DBUS_BUS_SESSION`来打开会话总线守护进程。
- message: 是两个进程之间的一条消息，所有的DBus内部通信都是使用DBus消息完成的，这些消息可以有以下类型，方法调用，方法返回，信号，和错误。DBusMessage结构可以通过附加布尔整数、实数、字符串、数组、…到消息。
- path: 是远程对象的路径，如: /org/freedesktop/DBus.
- interface:是要与之对话的给定对象上的接口。
- signal:用来发出信号
- method call: 它是用于调用远程对象上的方法的DBus消息。
- Error: DBusError是通过调用DBus方法来保存错误代码的结构。

#### dbus守护进程

- dbus 可以代表其它应用程序启动程序（服务），应用程序要求dbus通过其名称启动服务，该名称必须是已知的，dbus查找的服务描述文件通常安装在`/usr/share/dbus-1/services/`扩展名位`.service`

#### 使用

- DBus主要概念为总线，连接到总线的进程可以通过总线接收或传递消息，总线收到消息时，根据不同的消息类型进行不同的处理。DBus消息分为以下四类：

    1. Methodcall消息：将触发一个函数调用
    2. Methodreturn消息：触发函数调用返回的结果
    3. Error消息：触发的函数调用返回一个异常
    4. Signal消息：通知，可用看作为事件消息

- 



