# GDK3 API 大纲

### gdk 函数

- General 初始化和其它函数

- GdkDisplayManager 维护所有打开的GdkDisplays的列表 (单例)

- GdkDisplay 控制一组GdkScreens和其相关的输入设备(GdkDisplay对象可以描述为键盘、鼠标、一个或多个屏幕组成的工作站)

- GdkScreen 物理屏幕对象

- GdkSeat 表示用户输入设备集合的对象

- GdkMonitor 表示和GdkDisplay关联的各个输出

- GdkDevice 表示单个输入设备，如：键盘、鼠标、触摸板等

- GdkDevicePad 表示pad设备接口

- Points and Rectangles 简单的图形数据类型(点和矩形)

- Pixbufs 客户端图像

- RGBA Colors 颜色

- Visuals 低级别的硬件信息显示

- Cursors 创建和销毁光标，有标准光标和从pixmap创建的光标，需要绑定到一个窗口，供用户查看`gdk_window_set_cursor`

- Windows 目标视窗系统中的显示区域

- Frame clock GdkFClock告诉应用程序何时更新和重绘窗口

- Frame timings 保存单个帧的计时信息的对象

- GdkDrawingContext gdk窗口绘制的上下文

- GdkGLContext

- Events 处理来自系统窗口的事件

- Event Structures

- Key Values 操作键盘代码的功能

- Selections 用于在程序之间传递数据的函数，其本质是一个命名的粘贴板

- Drag And Drop 拖放功能

- Properties and Atoms 窗口属性

- Threads GDK 多线程编程

- Pango GDK和GTK+使用的文本布局系统

- Cairo 是一个支持矢量图形和图像合成的图形库，可以与GDK一起使用，GTK+使用cairo完成所有绘图。

- XWindow System Interaction X 窗口系统交互

- Wayland Interaction Wayland系统交互

- Application launching 应用程序启动通知

- Testing 测试



