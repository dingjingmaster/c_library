## dbus

### 概念介绍

- dbus-daemon 一个dbus的后台守护程序，用于多应用之间消息转发
- libdbus.so dbus的功能接口
- 高层封装，如dbus-glib和QT D-bus
- dbus总线一般分为系统总线和会话总线，所以安全性很好

> 官网中的dubs安装值包含上述1和2

1. 运行一个dbus-daemon就是创建了一条通信的总线Bus，当一个application链接到这条Bus上时，就产生了一个Connection
2. 从dbus 的概念上说，通信的双方是Object，一个application可以包含多个Object
3. 一个Object中会有不同的Interface，Interface可以看作是要传输的数据
4. dbus的两种通信方式：
  
  1. signal: 也就是广播
  2. method: 是一对一通信。

5. 程序中的各个元素

  1. address: 用来标识ibus-daemon，格式:`unix:path=/var/run/dbus/xxxx`
  2. bus name: 用来标识application的，格式：`com.xxx.xxx`
  3. path: 用于标识Object，格式：`/xxx/xxx/xxx`
  4. name: 每个interface都有自己的名字，通过interface的名字找到interface，格式：`xxx.xxx.xxx.xxx`
  5. signal 和 method也有自己的名字，随便取名就行

### 术语

- connection: 用于打开到守护进程的连接的结构，可以通过指定`DBUS_BUS_SYSTEM`来打开系统总线守护进程，也可以使用`DBUS_BUS_SESSION`来打开会话总线守护进程。
- message: 是两个进程之间的一条消息，所有的DBus内部通信都是使用DBus消息完成的，这些消息可以有以下类型，方法调用，方法返回，信号，和错误。DBusMessage结构可以通过附加布尔整数、实数、字符串、数组、…到消息。
- path: 是远程对象的路径，如: /org/freedesktop/DBus.
- interface:是要与之对话的给定对象上的接口。
- signal:用来发出信号
- method call: 它是用于调用远程对象上的方法的DBus消息。
- Error: DBusError是通过调用DBus方法来保存错误代码的结构。

### dbus守护进程需要配置


