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

### dbus守护进程需要配置


