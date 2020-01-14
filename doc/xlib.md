## xlib开发

### 简介

X窗口系统的开发主要目标是灵活性。想法是，事物的外观是一回事，而事物的工作方式则是另一回事。因此，较低的级别提供了绘制窗口，处理用户输入，允许使用彩色（或黑白屏幕）绘制图形等所需的工具。至此，决定将系统分为两部分。一个客户端决定要做什么，一个服务器实际在屏幕上绘制并读取用户输入，以便将其发送给客户端进行处理。

服务器控制屏幕，鼠标和键盘。客户端可以连接到服务器，请求它绘制一个窗口（或多个窗口），然后请求服务将用户发送到这些窗口的任何输入发送给服务器。因此，几个客户端可能会连接到一个X服务器-一个可能正在运行电子邮件软件，一个可能正在运行WWW浏览器，等等。
当用户将其发送到某个窗口时，服务器会将消息发送给控制该窗口的客户端进行处理。客户端决定如何处理此输入，然后在窗口中向服务器发送绘图请求。

整个会话使用X消息协议进行。该协议最初是通过TCP/IP协议套件承载的，允许客户端在连接到服务器所在网络的任何计算机上运行。后来在X服务器上进行了扩展，
以允许在本地计算机上运行的客户端对服务器进行更优化的访问（请注意，X协议消息的大小可能为数百KB），例如：使用共享内存或使用Unix域套接字（在两个进程之间的Unix系统上创建逻辑通道的方法）。

> 那么XServer的服务器其实是本地机器，客户端是远程和本地的程序。程序控制服务端绘图

1. GUI编程

与具有某些串行特性的常规计算机程序不同，GUI程序通常使用异步编程模型，也称为“事件驱动的编程”。这意味着该程序通常处于空闲状态，等待X服务器发送的事件，然后对这些事件进行操作。一个事件可能会说“用户在点x，y中按下了第一个按钮鼠标”，或者“您需要重绘您控制的窗口”。为了使程序能够响应用户输入以及刷新请求，它需要在相当短的时间内（例如，根据经验，少于200毫秒）处理每个事件。
这也意味着程序在处理事件时可能不会执行可能花费很长时间的操作（例如，打开与某个远程服务器的网络连接，或连接到数据库服务器，甚至执行长文件复制操作）。相反，它需要以异步方式执行所有这些操作。这可以通过使用各种异步模型执行冗长的操作，或者通过在不同的进程或线程中执行它们来完成。
因此，GUI程序的外观如下所示：
		- 执行初始化例程
		- 连接到X服务器
		- 执行X相关的初始化
		- 未完成时：
				- 从X服务器接收下一个事件
				- 处理事件，可能会向X服务器发送各种绘图请求
				- 如果事件是退出消息，请退出循环
		- 关闭与X服务器的连接
		- 执行清理操作

> 为了消除程序实际实现X协议层的需要，创建了一个名为“ Xlib”的库。该库使程序可以对任何X服务器进行非常低级的访问。由于该协议是标准化的，因此使用Xlib的任何实现的客户端都可以与任何X服务器通信。

2. 相关概念

	- X Display：代表我们与给定X服务器打开的连接的结构。它隐藏了来自服务器的消息队列，以及我们的客户端打算发送给服务器的未决请求队列。
	- GC-图形上下文：当我们执行各种绘制操作（图形，文本等）时，我们可以指定各种选项来控制如何绘制数据-使用哪种前景色和背景色，如何连接线边缘，绘制某些字体时使用什么字体文字等）。为了避免为每个绘图功能提供大量的参数，使用了“ GC”类型的图形上下文结构。我们在此结构中设置各种绘图选项，然后将指向该结构的指针传递给任何绘图例程。这非常方便，因为我们经常需要使用相同的选项执行多个绘图请求。因此，我们将初始化图形上下文，设置所需的选项，然后将此GC结构传递给所有绘图函数。
	- 对象句柄：当X服务器为我们创建各种对象（例如窗口，绘图区域和光标）时，相关函数将返回一个句柄。这是实际驻留在X服务器内存中而不是我们应用程序内存中的对象的一些标识符。服务器在这些句柄与其管理的实际对象之间保持映射。Xlib为这些对象（Window，Cursor，Colormap等）提供了各种类型定义，它们最终都被映射为简单的整数。
	- Xlib结构体的内存分配：xlib很灵活，所有变量都让用户分配和初始化很麻烦，所以大多数变量都有默认值。
	- 释放内存：用户申请的内存使用`free`，使用X库申请的用`XFree`
	- 事件：类型为“XEvent”的结构用于传递从X服务器接收到的事件。Xlib支持大量的事件类型。XEvent结构包含接收到的事件类型，以及与事件相关的数据(例如，生成事件的屏幕位置、与事件相关的鼠标按钮、与“重绘”事件相关的屏幕区域等)。读取事件数据的方式取决于事件类型。因此，XEvent结构包含所有可能事件类型的C语言联合
	- 退出：XCloseDisplay(display);只是清空了服务端保存的数据和释放了服务端的内存，客户端方面还没有退出。

> 关于GC：图形上下文定义了与各种绘图函数一起使用的几个属性。为此，我们定义了一个图形上下文。我们可以在单个窗口中使用多个图形上下文，以绘制多种样式(不同的颜色、不同的线宽等)。分配新GC是使用XCreateGC()函数完成的

3. 编译

```shell
cc prog.c -o prog -lX11
```
	
### 最基本的Xlib程序剖析

1. 包含头文件

```c
#include <X11/Xlib.h> 													// xlib程序必须包含
#include <assert.h>   													// 测试返回值
```

2. 打开与XServer的连接

```c
Display* dsp = XOpenDisplay(NULL); 	//失败返回NULL
assert(dpy);
```
此时，创建了一个窗口，接下来需要先获取窗口的背景色。由于要适应很多机器，所以X的颜色模型比较复杂。每个颜色由一个整形值在同一进程中代替。
X中唯而的获取两种颜色“白”和“黑”的函数如下:
```c
int blackColor = BlackPixel(dsp, DefaultScreen(dsp));
int whiteColor = WhitePixel(dsp, DefaultScreen(dsp));
```

3. 创建新的窗口

- XCreateSimpleWindow
- XCreateWindow

```c
/* 参数：显示上下文，父窗体，位置x，位置y，长度，高度，边框宽度，边框颜色 */
Window w = XCreateSimpleWindow(dsp, DefaultRootWindow(dsp), 0, 0, 200, 100, 0, blackColor, blackColor);
```
- DefaultRootWindow(dsp); 正在创建窗口的父窗口。当创建的窗口出现在父窗口中，并受到它的限制，称为对父窗口的裁剪。
- 窗口管理器负责装饰窗口。

4. 关注事件

> X是基于“客户端-服务端”架构的。X服务端向客户端发送事件，使其了解服务端中的修改，客户端需要告诉服务器它感兴趣的修改。
> Map事件就是将窗口显示到屏幕上

```c
XSelectInput(dsp, w, StructureNotifyMask);
```
5. 显示窗口

> map/显示的请求是异步的，执行这条指令的时间由XServer决定，执行的时候服务器会发送MapNotify事件。

```c
XMapWindow(dsp, w);
```
6. GC(Graphics Context对象)

> X图形模型是无状态的，XService不记得诸如：绘图颜色、线条粗细等属性。因此我们需要在每个绘图请求中向服务端提供所有的这些参数。为了避免每次传递所有参数，X使用了一个名为GC的对象，用来存储图形向下文中所有需要的参数

```c
GC gc = XCreateGC(dsp, w, 0, 0);

// 设置前景色
XSetForeground(dsp, gc, whiteColor);
```
7. 等待服务端渲染窗口

> 死循环等待结果，成功渲染则退出等待。

```c
for (;;) {
				XEvent e;
				XNextEvent (dsp, &e);
				if (e.type == MAPNotify) {
								break;				
				}
}
```
- 我们获取到窗口渲染事件后退出等待，另外注意：必须获取到事件后`XNextEvent(d, &e);`XService才会渲染窗口。
- 有些事件需要我们手动设置接收，有些事件就算没有设置也会被接收，这些事件称为“不可屏蔽事件”，窗口大小改变就是这样一个事件。
- 不可屏蔽事件是在响应一些程序请求的时候会发送，因此不太可能在我们上下文中发生。
- `XNextEvent()`过程是阻塞的，因此如果没有要读取的事件，程序就只是在`XNextEvent()`中等待(读取事件之前执行`XFlush(dsp)`)。

- 划线：XDrawLine(dsp, w, gc, 10, 60, 180, 200);这条线在(10, 60)到(180,20)之间，此时程序会在此处休眠等待，需要手动执行`XFlush()`请求传到服务器才行`XFlush(dsp)`。

### 检查显示器相关信息

```c
/* this variable will be used to store the "default" screen of the  */
/* X server. usually an X server has only one screen, so we're only */
/* interested in that screen.                                       */
int screen_num;

/* these variables will store the size of the screen, in pixels.    */
int screen_width;
int screen_height;

/* this variable will be used to store the ID of the root window of our */
/* screen. Each screen always has a root window that covers the whole   */
/* screen, and always exists.                                           */
Window root_window;

/* these variables will be used to store the IDs of the black and white */
/* colors of the given screen. More on this will be explained later.    */
unsigned long white_pixel;
unsigned long black_pixel;

/* check the number of the default screen for our X server. */
screen_num = DefaultScreen(display);

/* find the width of the default screen of our X server, in pixels. */
screen_width = DisplayWidth(display, screen_num);

/* find the height of the default screen of our X server, in pixels. */
screen_height = DisplayHeight(display, screen_num);

/* find the ID of the root window of the screen. */
root_window = RootWindow(display, screen_num);

/* find the value of a white pixel on this screen. */
white_pixel = WhitePixel(display, screen_num);

/* find the value of a black pixel on this screen. */
black_pixel = BlackPixel(display, screen_num);

```

### GC 相关

- Value 和 ValueMask：ValueMask表示我们希望设置的值，Value表示希望设置值对应的值。

> GC实例创建之后就可以使用其它函数修改其值。

### 绘制相关

1. 绘制点

```c
/* draw a pixel at position '5,60' (line 5, column 60) of the given window. */
XDrawPoint(display, win, gc, 5, 5);
```
2. 绘制线

```c
/* draw a line between point '20,20' and point '40,100' of the window. */
XDrawLine(display, win, gc, 20, 20, 40, 100);

/* the XDrawLines() function draws a set of consecutive lines, whose     */
/* edges are given in an array of XPoint structures.                     */
/* The following block will draw a triangle. We use a block here, since  */
/* the C language allows defining new variables only in the beginning of */
/* a block.                                                              */
  {
    /* this array contains the pixels to be used as the line's end-points. */
    XPoint points[] = {
      {0, 0},
      {15, 15},
      {0, 15},
      {0, 0}
    };
    /* and this is the number of pixels in the array. The number of drawn */
    /* lines will be 'npoints - 1'.                                       */
    int npoints = sizeof(points)/sizeof(XPoint);

    /* draw a small triangle at the top-left corner of the window. */
    /* the triangle is made of a set of consecutive lines, whose   */
    /* end-point pixels are specified in the 'points' array.       */
    XDrawLines(display, win, gc, points, npoints, CoordModeOrigin);
  }
```
3. 绘制角度

> XFillArc()

```c
/* draw an arc whose center is at position 'x,y', its width (if it was a     */
/* full ellipse) is 'w', and height is 'h'. Start the arc at angle 'angle1'  */
/* (angle 0 is the hour '3' on a clock, and positive numbers go              */
/* counter-clockwise. the angles are in units of 1/64 of a degree (so 360*64 */
/* is 360 degrees).                                                          */
int x = 30, y = 40;
int h = 15, w = 45;
int angle1 = 0, angle2 = 2.109;
XDrawArc(display, win, gc, x-(w/2), y-(h/2), w, h, angle1, angle2);

/* now use the XDrawArc() function to draw a circle whose diameter */
/* is 15 pixels, and whose center is at location '50,100'.         */
XDrawArc(display, win, gc, 50-(15/2), 100-(15/2), 15, 15, 0, 360*64);
```
4. 绘制矩形

> 填充多边形 XFillPolygon()如果数组中的最后一个点与数组中的第一个点的位置不同，则XFillPolygon()函数会自动添加另一个“虚拟”行，将这两个点连接起来

```c
/* draw a rectangle whose top-left corner is at '120,150', its width is */
/* 50 pixels, and height is 60 pixels.                                  */
XDrawRectangle(display, win, gc, 120, 150, 50, 60);

/* draw a filled rectangle of the same size as above, to the left of the  */
/* previous rectangle. note that this rectangle is one pixel smaller than */
/* the previous line, since 'XFillRectangle()' assumes it is filling up   */
/* an already drawn rectangle. This may be used to draw a rectangle using */
/* one color, and later to fill it using another color.                   */
XFillRectangle(display, win, gc, 60, 150, 50, 60);
```

### X事件

> 在Xlib程序中，一切都是由事件驱动的。屏幕上的事件绘制有时是作为对事件(“expose”事件)的响应而完成的。如果程序窗口中被隐藏的部分被暴露(例如，窗口被提升到其他窗口之上)，X服务器将发送一个“暴露”事件，让程序知道它应该重新绘制窗口的这部分。用户输入(按键、鼠标移动等)也作为一组事件接收。

1. 注册事件

> 在程序创建一个窗口(或多个窗口)之后，它应该告诉X服务器它希望为这个窗口接收什么类型的事件。默认情况下，不会向程序发送任何事件。它可以注册各种鼠标(也称为“指针”)事件、键盘事件、公开事件等等。这样做是为了优化服务器到客户机的连接(例如，为什么要发送一个它不感兴趣的程序(它甚至可能在地球的另一端运行)?
> 在Xlib中，我们使用XSelectInput()函数来注册事件。该函数接受3个参数——Display结构体、窗口的ID和它希望获得的事件类型的掩码。窗口ID参数允许我们注册接收不同窗口的不同类型的事件。下面是我们如何注册一个ID为'win'的窗口的"expose"事件:

```c
XSelectInput(display, win, ExposureMask);
XSelectInput(display, win, ExposureMask | ButtonPressMask);
```

2. 接收事件——使用事件循环

```c
/* this structure will contain the event's data, once received. */
XEvent an_event;

/* enter an "endless" loop of handling events. */
while (1) {
    XNextEvent(display, &an_event);
    switch (an_event.type) {
      case Expose:
        /* handle this event type... */
        break;
      default: /* unknown event type - ignore it. */
        break;
    }
}
```

> XNextEvent() 函数的作用是:获取来自X服务器的下一个事件。如果没有事件在等待，它将阻塞直到接收到一个事件。当它返回时，事件的数据被放在作为第二个参数提供给函数的XEvent变量中。之后，该变量的“type”字段指定我们得到的事件类型。Expose是事件类型，它告诉我们窗口中有一部分需要重新绘制。处理此事件后，我们返回并等待下一个事件处理。显然，我们需要给用户一些终止程序的方法。这通常是通过处理一个特殊的“退出”事件来完成的

3. expose 事件

“expose”事件是应用程序可能接收的最基本事件之一。它将在以下几种情况之一发给我们:

	- 覆盖部分窗户的窗户已经移动，露出部分(或全部)窗户
	- 我们的窗户高于其他窗户
	- 我们的窗口第一次展示
	- 我们的窗户被去图标化

> 您应该注意这里隐藏的隐含假设——当我们的窗口被其他窗口隐藏时，它的内容将丢失。有人可能想知道为什么X服务器不保存这些内容。答案是——为了节省内存。毕竟，在给定时间显示的窗口数量可能非常大，存储所有这些窗口的内容可能需要大量内存(例如，一个覆盖400×400像素的256色位图需要160KB的内存)。现在想想20个窗口，有些比这个大很多)。实际上，有一种方法可以告诉X服务器在特殊情况下存储窗口的内容，我们将在后面看到。

"expose"事件字段:

	- count:服务器事件队列中正在等待的其他公开事件的数量。如果我们连续获得多个暴露事件，这可能会很有用-通常，我们将避免重绘窗口，直到获得最后一个事件
	- window:发送此"expose"事件的窗口的ID（以防我们的应用程序在多个窗口上注册了事件）
 	- int x, y:需要绘制的窗口区域的x和y坐标（以像素为单位）
 	- int width,height: 需要重绘的窗口区域的宽度和高度（以像素为单位）。 

绘制一条线 	
```c
   case Expose:
    /* if we have several other expose events waiting, don't redraw. */
    /* we will do the redrawing when we receive the last of them.    */
    if (an_event.xexpose.count > 0)
        break;
    /* ok, now draw the line... */
    XDrawLine(display, win, gc, 0, 100, 400, 100);
    break;
 ```

4. 获取用户输入

> 传统上，用户输入有两个来源——鼠标和键盘。存在各种事件类型来通知我们用户的输入——键盘上的键被按下，键盘上的键被释放，鼠标在窗口上移动，鼠标进入(或离开)窗口，等等。

- 按键按下

```c
  case ButtonPress:
    /* store the mouse button coordinates in 'int' variables. */
    /* also store the ID of the window on which the mouse was */
    /* pressed.                                               */
    x = an_event.xbutton.x;
    y = an_event.xbutton.y;
    the_win = an_event.xbutton.window;

    /* check which mouse button was pressed, and act accordingly. */
    switch (an_event.xbutton.button) {
        case Button1:
            /* draw a pixel at the mouse position. */
            XDrawPoint(display, the_win, gc_draw, x, y);
            break;
        case Button2:
            /* erase a pixel at the mouse position. */
            XDrawPoint(display, the_win, gc_erase, x, y);
            break;
        default: /* probably 3rd button - just ignore this event. */
            break;
    }
    break;
```

- 鼠标移动按钮，一种是没有按下按钮时的鼠标指针移动，另一种是按下一个(或多个)按钮时的鼠标指针移动(有时称为“鼠标拖动操作”，或简称“拖动”)。以下事件掩码可以添加到XSelectInput()调用中，以便我们的应用程序得到此类事件的通知:

		- pointerMotionMask:指针事件在我们的应用程序控制的窗口之一中移动，而没有按住任何鼠标按钮。
		- ButtonMotionMask:按住一个（或多个）鼠标按钮时指针移动的事件。
		- Button1MotionMask:与ButtonMotionMask相同，但是仅在按住鼠标1st的情况下
		- Button2MotionMask, Button3MotionMask, Button4MotionMask, Button5MotionMask:同样，对于第二个鼠标按钮，或第三，第四或第五个按钮。

在事件循环开关中要检查的事件类型为以下任意一种：
		
		- MotionNotify:鼠标指针移到了我们要求收到此类事件的窗口之一中。这些事件类型的事件结构可以通过“ an_event.xbutton”进行访问，并且包含以下字段：
		
				- window: 发送此鼠标移动事件的窗口的ID（以防我们的应用程序在多个窗口上注册了事件的窗口）
				- int x,y:
				- unsigned int state:按键Mask：Button1Mask、Button2Mask、Button3Mask、Button4Mask、Button5Mask、ShiftMask、LockMask、ControlMask、Mod1Mask、Mod2Mask、Mod3Mask、Mod4Mask、Mod5Mask，前5个是指被按下的鼠标按钮，而其余的是指被按下的各种“特殊键”（Mod1通常是“ ALT”键或“ META”键）
- 鼠标进入、离开事件
- 键盘焦点：多个窗口打开，但只有一个键盘与之连接，XServer根据焦点发送特定的键盘输入事件
- 键盘按下和释放事件：

5. 处理文字和字体

		除了在窗口上绘制图形外，我们通常还想绘制文本。文本字符串有两个主要属性—要绘制的字符和用于绘制它们的字体。为了绘制文本，我们需要首先请求X服务器加载字体。然后将字体分配给GC，最后使用GC在窗口中绘制文本。

```c
/* assume that win_width contains the width of our window, win_height        */
/* contains the height of our window, and 'win' is the handle of our window. */

/* some temporary variables used for the drawing. */
int x, y;

/* draw a "hello world" string on the top-left side of our window. */
x = 0;
y = 0;
XDrawString(display, win, gc, x, y, "hello world", strlen("hello world"));

/* draw a "middle of the road" string in the middle of our window. */
char* text_string = "middle of the road";
/* find the width, in pixels, of the text that will be drawn using */
/* the given font.                                                 */
int string_width = XTextWidth(font_info, text_string, strlen(text_string));
/* find the height of the characters drawn using this font.        */
int fond_height = font_info->ascent + font_info->descent;
x = (win_width - string_width) / 2;
y = (win_width - font_height) / 2;
XDrawString(display, win, gc, x, y, "hello world", strlen("hello world"));
```

未完 ...... 
