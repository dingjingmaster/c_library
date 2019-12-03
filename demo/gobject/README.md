# gobject

## gobject学习

#### gobject好处

 - [x] 基于引用计数的内存管理
 - [x] 对象的构造函数与析构函数
 - [x] 可设置对象属性的 set/get 函数
 - [x] 易于使用的信号机制

#### 怎样用C实现一个类（以双向链表声明为例demo1）

1. 定义节点数据类型，定义链表数据类型
2. 定义链表类数据类型 (注意链表类和链表数据类型第一个参数都是 GObject 类型,意味着继承自GObject)
3. 声明 `GType xxx_get_type(void);` 定义 `p_t_get_type` 宏,即:`#define xxx_TYPE_xxx (xxx_get_type(void))`
4. 在`.c`文件中生成`p_t_get_type`函数的具体实现，即`G_DEFINE_TYPE(类型, 成员函数前缀, 父类:G_TYPE_OBJECT)`
5. 实现类型初始化和类的初始化函数`xxx_init(类型指针 self)...` 和 `xxx_class_init(xxxClass* self)...`
6. 使用：在`main`函数中1. GObject库的类型管理系统初始化`g_type_init()`；2. 类实例化；3.类销毁

#### gobject 实现类型隐藏 (demo2)

1. 在`.c`文件中声明并定义私有类型结构体
2. 在`xxx_class_init`中添加私有属性结构体
3. 定义私有属性获取的宏`xxx_GET_PRIVATE(obj)（这里调用G_TYPE_INSTANCE_GET_PRIVATE((obj), 数据类型, 私有属性类型)）`

#### gobject 私有属性外部访问 (demo3)

> set/get相对直接指针处理的好处：将数据的变化和程序的功能隔开，数据的变化不影响程序的功能(给使用库的程序员一致的使用方式，变化都在底层)。

1. 实现`xxx_set`和`xxx_get`
2. 在`xxx_class_init`中安装
    1. `GObjectClass *base_class = G_OBJECT_CLASS (klass);`
    2. `base_class->set_property = pm_dlist_set_property;`
    3. `base_class->get_property = pm_dlist_get_property;`

#### GObject中安装属性(demo3)

> 1. GParamSpec 类型对一个键值对打包成一个数据结构，然后将之安装到相应的 GObject 子类中。
> 2. `g_param_spec_pointer` 函数，可以将“属性名：属性值”参数打包为`GParamSpec`类型的变量，该函数的第一个参数用于设定键名，第二个参数是键名的昵称，第三个参数是对这个键值对的详细描述，第四个参数用于表示键值的访问权限，`G_PARAM_READABLE | G_PARAM_WRITABLE` 是指定属性即可读又可写，`G_PARAM_CONSTRUCT` 是设定属性可以在对象示例化之时被设置
> 3. `g_object_class_install_property` 函数用于将 `GParamSpec` 类型变量所包含的数据插入到 `GObject` 子类中，其中的细节可以忽略，只需要知道该函数的第一个参数为 `GObject` 子类的类结构体，第二个参数是 `GParamSpec` 对应的属性 `ID`。`GObject` 子类的属性 `ID` 在前文已经提及，它是 GObject 子类设计者定义的宏或枚举类型。第三个参数是要安装值向 `GObject` 子类中的 `GParamSpec` 类型的变量指针。
> 4. 一定要注意，`g_object_class_install_property` 函数的第二个参数值不能为 0

#### GObject中添加方法(demo4)

1. 直接实现就行，注意要传入数据类型


## 工具之 GOB2

> The gobject builder
> GOB2 是一个简单的预处理器，简化了GObject类的创建。
> GOB2 主要用于C或C++中创建GObject。

#### gob2 [选项]... 文件

|               |     |
| ---           | --- |
| -? -h --help  | 显示帮助 |
| --version     | 显示版本 |
| -w --exit-on-warn | 警告时返回错误码并退出 |
| --no-exit-on-warn | 仅在错误时退出(默认) |
| --for-cpp     | 产生c++代码 |
| --no-extern-c | 不在头文件中加入 `extern c` |
| --no-gnu      | 不用GNU C 扩展生成任何代码，更严格 |
| --no-touch    | 不 touch 文件，除非真的改变了 |
| --no-touch-headers | 要禁用 |
| --always-private-header | 创建<basename>-private.h 即使文件可能时空的 |
| --ondemand-private-header | 当有私有数据、方法的时候，创建私有头文件(默认) |
| --no-private-header | 没有私有成员和数据 |
| --m4          | 用m4处理源码(多语言支持) |
| --m4-dir      | 将搜索m4文件的打印目录 |
| -n --no-write | 不要写任何输出文件，只检查输入文件的语法 |
| --no-lines    | 不要把 '#line' 语句打印到输出中，用于调试自动生成的代码 |
| --no-self-alias | 不要创建 self 和 selfclass 类型的别名及 Self、IS-Self、Self-CLASS宏 |
| --no-kill-underscores | 不要从方法名中删除初始下划线 |
| --always-private-struct | 始终在公共头文件中包含私有指针。这对于作为库的一部分的文件非常有用，并且你可以在不破坏为二进制兼容的情况下添加某些私有数据成员 |
| -o --output-dir | 将输出放入的目录 |
| --file-sep[=c] | 替换默认的'-'文件名分隔符 |
| --gtk3         | 使用gtk3 |

#### 类型名

- 类型声明以`:`分割
- 第一个一般为 `namespace` 
- 例如:

  `Gtk:New:Button` 宏将变为 `GTK_IS_NEW_BUTTON` 和 `gtk_TYPE_NEW_BUTTON`

#### 输出文件

- 文件名就是上边提到的类型名，以 `-` 分割，且为小写；上述示例结果为 `gtk-new-button.h` `gtk-new-button.c`，如果使用C++，则扩展名为`.cc` 如果有任何私有成员或私有变量，则文件名为: `<basename>-private.h`

#### 在输出文件中包含正常的C代码

- 要在输出C文件中直接包含某些代码，在空行以`%{`开头，在空行以`%}`结尾。这些部分将按给定的顺序显示在输出文件中。
- 也可以写在另一个`.h`文件中，在现在这个文件中包含进来。
- 也可以写在私有文件中`.hp`，这将使代码进入私有头文件。
- 如果希望某些代码出现在外部`c`和保护定义之前，可以将他们放在`headertop(或ht)`部分
- 如果要包含注释可以将他们放入`all(或a)`部分
- 如果希望出现在所有文件顶部的代码进入`all top(或at)`部分

```c
%alltop{
    //
%}

%ctop{
    /* this will be at the very top of the C file */
    /* Requires 2.0.18 */
%}

%headertop{
    /* this will be on top of the public header */
%}

%privateheader{
    /* this will go into the private header file */
%}

%h{
    /* will be included in the header */
    void somefunc(int i);
%}

%a{
    /* will be included in all files */
%}

%afterdecls{
    /* between the declarations and the method implementations */
    /* Requires gob version 2.0.16 */
%}

%{
    /* will be included in the C file */
    void somefunc(int i)
    {
        /* some code */
    }
%}

```
