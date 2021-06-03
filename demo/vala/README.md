## vala
> vala 是GNOME使用现代语言特性，提供了对GLib和GObject特性的使用

### vala 是什么

1. 静态类型编程语言
2. 应用程序在运行时候唯一支持的是标准的 GLib 和 GObject 库（通过VAPI文件来描述接口并使用Vala中的系统库）
3. vala 提供了与 DBus 轻松集成

### vala 源码

- vala 源码分为两类：
 1. 以 `.vala` 结尾的源代码，这里包含了完整的 `.vala` 源码
 2. 以 `.vapi` 结尾的文件，这里记录对库的描述，既不可以用 `vala` 也不可以用 `c` 来写,它是不可编译的，仅仅在编译`vala` 时候有用

- vala 文件命名
  正常来说并没有严格要求`vala`源文件的命名。`VAPI` 文件的命名通常与它们所关联的库的 `pkg-config` 名称匹配，约定如下：
  1. vala 源文件通常包含一个主要的公共类，名字为 `命名空间` + `-` + `类名小写`
  2. 命名空间 - `NameSpaceName`
  3. 类名 - `ClassName`
  4. 方法名 - `method_name`
  5. 静态变量、枚举 - `CONSTANT_NAME`

### vala 语法
  vala 语法类似 `C#`
  合法标识符必须是：`数字` `字母` `下划线`，数字不打头。也可使用语言关键字为标识符，必须以`@`打头。

### GType 和 GObject
  vala 使用  GType 的运行时类型系统，GType为vala提供了一个名为GObject的强大对象模型。

### 内存管理
  vala 自动使用 GLib 中的内存管理系统，这是一个引用计数系统。因为没有垃圾回收器，循环引用可能导致内存泄漏。
  vala 有指针类型

### vala 编译
  vala 程序和库在编译成机器码之前被翻译成 `c` 语言。这一阶段是完全透明的，一个vala类转为一个 c 的头文件和源文件

### 程序入口
  程序入口一定是 `main` 函数
  ```vala
  void main () { ... }
  int main () { ... }
  void main (string[] args) { ... }
  int main (string[] args) { ... }
  ```
  入口点可以是隐式的，因为您可以直接在 main 函数外部的头文件中编写主代码块

## 概念

### 变量、字段、参数
  - 类型的实例以不同的方式创建，具体取决于类型。例如，基本类型是用文字表达式实例化的，而类类型是用new操作符实例化的。
  - 为了访问数据，必须以某种方式(如通过名称)标识实例。在Vala中，大致有三种方法可以实现，语义相似但不相同。

#### 变量
  在方法的可执行代码中，可以将实例赋值给变量。变量有一个名称，并且被声明为引用特定数据类型的实例。一个典型的变量声明是:
  ```vala
  int a;
  ```
  如果一个类型在变量声明语句中直接实例化，那么该变量将拥有该新实例而被创建，例如:
  ```vala
  string s = "hello,world";
  ```

#### 字段
  字段类似于变量，只是定义它的范围不同。字段可以在名称空间、类和结构中定义。对于类和结构，它们可以在类或结构的作用域内，也可以在类或结构的每个实例的作用域内。
  只要一个字段的作用域仍然存在，它就有效——对于非实例字段，这是应用程序的整个生命周期;例如实例字段，这是实例的生命周期。
  如果一个类型在字段的声明中直接实例化，那么该字段将创建为拥有该新实例的字段。

#### 参数
  传递给方法的实例可以通过方法参数列表中给出的名称在该方法中访问。
  它们的行为就像变量一样，除非默认情况下，它们不能获得分配给它们的第一个实例的所有权，即传递给方法的实例。这种行为可以通过明确的所有权转移来改变。当对参数重新赋值时，结果取决于参数的方向。假设参数没有方向修饰符，它随后将完全充当变量。

### 面向对象编程
  - Vala主要是一种面向对象的语言。本文档不打算详细描述面向对象编程，但为了使其他部分更有意义，需要解释一些事情。

  - Vala中的类是潜在多态类型的定义。多态类型是一种可以被视为多个类型的类型。其基本方法是继承，因此可以将一个类型定义为另一个类型的专门化版本。子类型的实例来自于特定的超类型，具有该超类型的所有属性，可以在需要该超类型实例的任何地方使用。

  - 无论何时需要SuperType实例，都可以使用SubType实例。这是压缩类所允许的继承范围，但是完整类的特性更强。所有非紧凑类型的类都可以有虚方法，也可以实现接口。

    ```vala
      class SuperType {
      		public int act() {
      			return 1;
            }
      }
      class SubType : SuperType {
      }
    
      // 调用
      SubType sub = new SubType();
      SuperType super = sub;
    
      sub.act();
      super.act();
    ```

    

  这里，当SubType .act()被调用时，被调用的方法将是SubType的“act”方法。调用super.act()将调用SuperType的“act”。如果act方法是虚的，则SubType 在这两种情况下都将调用Act方法。

### 引用与所有权
  - Vala中的类型实例在很大程度上是自动管理的。这意味着分配内存来存储数据，然后在不再需要数据时释放内存。然而，Vala没有运行时垃圾收集器，相反，它在编译时应用规则，在运行时可预见地释放内存。
  - Vala的内存管理系统的核心概念是所有权。实例被认为是仍在使用,只要至少有一个的访问方式,即有一些领域,指的是实例变量或参数——这样的一个标识符将被视为实例的所有者,因此实例的内存将不会被收回。当不再有任何方式访问数据实例时，它被认为是无主的，其内存将被释放。

#### 引用与所有权
  - 使用引用类型，多个标识符可以引用相同的数据实例。并非所有引用类型实例的标识符都能够拥有该实例，原因将在后面解释。因此，在编写Vala代码时，经常需要考虑实例所有权。
  - 大多数引用类型都支持引用计数。这意味着实例在内部维护当前存在多少对它的引用的计数。此计数用于决定实例是否仍在使用中，或者是否可以释放其内存。因此，以这种方式计算的每个引用都是实例的潜在所有者，因为它确保实例继续存在。有些情况下不希望这样，因此可以将字段或变量定义为“weak”。在这种情况下，引用不会被计数，因此引用存在的事实不会阻止实例被释放，也就是说，这种引用不能获得实例的所有权。
  - 当使用引用计数类型时，弱引用的主要用途是防止引用循环。当数据实例内部包含对另一个实例的引用时，就会存在这些引用，而另一个实例又包含对第一个实例的引用。在这种情况下，不可能重新分配实例，因为每个实例都可能拥有另一个实例。通过确保其中一个引用是弱引用，其中一个实例可以成为无主实例并被释放，在此过程中另一个实例将被解除引用，也可能成为无主实例并被释放。
  - 它也可以有引用类型没有引用计数;这方面的一个例子是基本字符串类型，其他是紧凑的类类型。如果Vala允许多个引用拥有这些实例，它将无法跟踪它们何时全部停止存在，因此将无法知道何时释放实例。相反，只有一个或零个标识符将拥有实例——当它为零时，实例将被释放。这意味着，对一个已经拥有的实例的所有引用要么是弱引用，要么必须使用所有权转移操作符明确地将所有权传递给新引用

#### 指针类型
  指针类型非常重要。指针类型是值类型，其实例是对其他一些数据实例的引用。因此，它们不是实际的引用，并且永远不会拥有它们间接引用的实例。

### 类型

### 表达式

### 语句

### 命名空间

### 方法

### 代理(委托)

### 错误

### 类

### 接口

### 泛型

### 结构体

### 枚举类型

### 属性

## 预处理器

  Vala预处理器是Vala的一个特殊部分，它只在语法级别起作用，允许您根据特定的编译时条件有条件地编写软件的各个部分。预处理器指令永远不会在结果代码中生成。

### 指令语法
  所有预处理指令都以(#)开头，除了文件的第一行的#!(用于Vala脚本)。
  ```vala
  vala-code:
	[ any vala code ] [ pp-condition ] [ any vala code ]

  pp-condition:
	#if pp-expression vala-code [ pp-elif ] [ pp-else ] #endif

  pp-elif:
	#elif pp-expression vala-code [ pp-elif ]

  pp-else:
	#else vala-code

  pp-expression:
	pp-or-expression

  pp-or-expression:
	pp-and-expression [ || pp-and-expression ]

  pp-and-expression:
	pp-binary-expression [ && pp-binary-expression ]

  pp-binary-expression:
	pp-equality-expression
	pp-inequality-expression

  pp-equality-expression:
	pp-unary-expression [ == pp-unary-expression ]

  pp-inequality-expression:
	pp-unary-expression [ != pp-unary-expression ]

  pp-unary-expression:
	pp-negation-expression
	pp-primary-expression

  pp-negation-expression:
	! pp-unary-expression

  pp-primary-expression:
	pp-symbol
	( pp-expression )
	true
	false

  pp-symbol:
	identifier
  ```
  预处理器的语义非常简单:如果条件为真，那么被预处理器包围的Vala代码将被解析，否则将被忽略。如果符号是在编译时定义的，则该符号的计算结果为true。如果预处理指令中的符号没有定义，它的计算结果为false。

### 定义符号
  在Vala代码中定义预处理器符号是不可能的(像C一样)。定义符号的唯一方法是通过valac选项 `-D` 提供它。

  - 内置定义

    | **Name**  | **Description**                                           |
    | --------- | --------------------------------------------------------- |
    | POSIX     | Set if the profile is posix                               |
    | GOBJECT   | Set if the profile is gobject                             |
    | DOVA      | Set if the profile is dova                                |
    | VALA_X_Y  | Set if Vala API version is equal or higher to version X.Y |
    | DBUS_GLIB | Set if using dbus-glib-1 package                          |

  - 例子
    如何基于valac选项-D有条件地编译代码。
    
    ```vala
    // Vala preprocessor example
    public class Preprocessor : Object {
    
        public Preprocessor () {
        }
    
        /* public instance method */
        public void run () {
    #if PREPROCESSOR_DEBUG
        // Use "-D PREPROCESSOR_DEBUG" to run this code path
        stdout.printf ("debug version \n");
    #else
        // Normally, we run this code path
        stdout.printf ("production version \n");
    #endif
        }
    
        /* application entry point */
        public static int main (string[] args) {
            var sample = new Preprocessor ();
            sample.run ();
            return 0;
        }
    }
    ```
    正常编译:
    ```shell
    valac -o preprocessor Preprocessor.vala
    ```
    debug编译:
    ```shell
    valac -D PREPROCESSOR_DEBUG -o preprocessor-debug Preprocessor.vala
    ```

## GIDL 元数据格式
  本节描述了.metadata文件的格式，vapigen将其用作生成.vapi文件的附加信息。元数据中指定的一些信息还可以用于设置符号属性。

- 注释: 注释以#开头，以行尾结束。例如:
   ```shell
   # this is a comment
   ```

- 其它行:
  1. 文件中的每一个非注释行都由两部分组成:说明符和参数。
  2. 说明符是该行中出现的第一个文本，它指定了该行其余部分将修改的内容。
  3. 参数是一个以空格分隔的参数名称列表，后跟等号和用引号括起来的值。
  例如，这一行设置foo.bar上的parameter1和parameter2:
  ```c
  foo.bar parameter1="value" parameter2="value"
  ```

- 说明符：
  1. 说明符总是对您正在修改的内容使用C名称。例如，如果您的名称空间是Foo，而类型的Vala名称是Bar，那么您将使用FooBar。
  2. 说明符也可以使用通配符，所有部分匹配说明符的项都将被选中。例如:
    ```shell
    *.klass hidden="1"
    ```
    将隐藏所有类型中类字段

- 指定不同的事情

   | Function             | `name_of_function`        |
   | -------------------- | ------------------------- |
   | Type                 | `Type`                    |
   | Property             | `Type:property_name`      |
   | Signal               | `Type::signal_name`       |
   | Field                | `Type.field_name`         |
   | Parameter (Function) | `name_of_function.param`  |
   | Parameter (Delegate) | `DelegateName.param`      |
   | Parameter (Signal)   | `Type::signal_name.param` |

   例如，隐藏一个符号

   | Type     | `Foo hidden="1"`           |
   | -------- | -------------------------- |
   | Function | `some_function hidden="1"` |
   | Field    | `Foo.bar hidden="1"`       |

- 属性引用

   条目的格式如下所示

   | **Name** | **Applies To**                       | **Values**            | **Description**            |
   | -------- | ------------------------------------ | --------------------- | -------------------------- |
   | `foobar` | Signal, Function, Class, Struct, etc | The acceptable values | The description goes here. |

   并按字母顺序排列：

   | **Name**                | **Applies To**                                               | **Values**                                            | **Description**                                              |
   | ----------------------- | ------------------------------------------------------------ | ----------------------------------------------------- | ------------------------------------------------------------ |
   | `abstract`              | Class, Function                                              | 0, 1                                                  |                                                              |
   | `accessor_method`       | Property                                                     | 0, 1                                                  |                                                              |
   | `array_length_cname`    | Field                                                        | C identifier                                          |                                                              |
   | `array_length_pos`      | Parameter (Function)                                         | Double (position between two Vala parameters)         | Sets the position of the length for the parameter, length needs to be hidden separately. |
   | `array_length_type`     | Parameter (Function), Function (returning an array), Field   | C type                                                |                                                              |
   | `array_null_terminated` | Function (returning an array), Parameter (Function), Field   | 0, 1                                                  |                                                              |
   | `async`                 | Function                                                     | 0, 1                                                  | Force async function, even if it doesn't end in _async       |
   | `base_class`            | Class                                                        | C type                                                | Marks the base class for the type                            |
   | `base_type`             | Struct                                                       | Vala type                                             | Marks the struct as inheriting                               |
   | `cheader_filename`      | Anything (except parameters)                                 | Header include path                                   | Compiler will adds the specified header when thing is used.  |
   | `common_prefix`         | Enum                                                         | String                                                | Removes a common prefix from enumeration values              |
   | `const_cname`           | Class (non-GObject)                                          | C type                                                |                                                              |
   | `copy_function`         | Class (non-GObject)                                          | C function name                                       |                                                              |
   | `cprefix`               | Module                                                       | String                                                |                                                              |
   | `ctype`                 | Parameter (Function), Field                                  | C type                                                |                                                              |
   | `default_value`         | Parameter (Function)                                         | Any Vala value that would be valid for the type       | Sets the default value for a parameter.                      |
   | `delegate_target_pos`   | Parameter (Function)                                         | Double (position between two Vala parameters)         |                                                              |
   | `deprecated`            | Anything (except parameters)                                 | 0, 1                                                  | Marks the thing as deprecated                                |
   | `deprecated_since`      | Anything (except parameters)                                 | Version                                               | Marks the thing as deprecated                                |
   | `ellipsis`              | Function                                                     | 0, 1                                                  | Marks that the function has a variable argument list         |
   | `errordomain`           | Enum                                                         | 0, 1                                                  | Marks the enumeration as a GError domain                     |
   | `finish_name`           | Function                                                     | C function name                                       | Sets custom asynchronous finish function                     |
   | `free_function`         | Class (non-GObject)                                          | C function name                                       | Sets a free function for the struct                          |
   | `gir_namespace`         | Module                                                       | String                                                |                                                              |
   | `gir_version`           | Module                                                       | Version                                               |                                                              |
   | `has_copy_function`     | Struct                                                       | 0, 1                                                  | marks the struct as having a copy function                   |
   | `has_destroy_function`  | Struct                                                       | 0, 1                                                  |                                                              |
   | `has_emitter`           | Signal                                                       | 0, 1                                                  |                                                              |
   | `has_target`            | Delegate                                                     | 0, 1                                                  |                                                              |
   | `has_type_id`           | Class, Enum, Struct                                          | 0, 1                                                  | Marks whether a GType is registered for this thing           |
   | `hidden`                | Anything                                                     | 0, 1                                                  | Causes the selected thing to not be output in the vapi file. |
   | `immutable`             | Struct                                                       | 0, 1                                                  | Marks the struct as immutable                                |
   | `instance_pos`          | Delegate                                                     | Double (Position between two Vala parameters)         |                                                              |
   | `is_array`              | Function (returning an array), Parameter, Field              | 0, 1                                                  | Marks the thing as an array                                  |
   | `is_fundamental`        | Class (non-GObject)                                          | 0, 1                                                  |                                                              |
   | `is_immutable`          | Class (non-GObject)                                          | 0, 1                                                  |                                                              |
   | `is_out`                | Parameter                                                    | 0, 1                                                  | Marks the parameter as "out"                                 |
   | `is_ref`                | Parameter                                                    | 0, 1                                                  | Marks the parameter as "ref"                                 |
   | `is_value_type`         | Struct, Union                                                | 0, 1                                                  | Marks type as a value type (aka struct)                      |
   | `lower_case_cprefix`    | Module                                                       | String                                                |                                                              |
   | `lower_case_csuffix`    | Interface                                                    | String                                                |                                                              |
   | `name`                  | Any Type, Function, Signal                                   | Vala identifier                                       | Changes the name of the thing, does not change namespace     |
   | `namespace`             | Any Type                                                     | String                                                | Changes the namespace of the thing                           |
   | `namespace_name`        | Signal Parameter                                             | String                                                | Specify the namespace of the parameter type indicated with type_name |
   | `no_array_length`       | Function (returning an array), Parameter (Function, Delegate) | 0, 1                                                  | Does not implicitly pass/return array length to/from function |
   | `nullable`              | Function (having a return value), Parameter                  | 0, 1                                                  | Marks the value as nullable                                  |
   | `owned_get`             | Property                                                     | 0, 1                                                  |                                                              |
   | `parent`                | Any module member                                            | String (Namespace)                                    | Strip namespace prefix from symbol and put it into given sub-namespace |
   | `printf_format`         | Function                                                     | 0, 1                                                  |                                                              |
   | `rank`                  | Struct                                                       | Integer                                               |                                                              |
   | `ref_function`          | Class (non-GObject)                                          | C function name                                       |                                                              |
   | `ref_function_void`     | Class (non-GObject)                                          | 0, 1                                                  |                                                              |
   | `rename_to`             | Any Type                                                     | Vala identifier                                       | Renames the type to something else, ie fooFloat to float (not exactly the same as `name`, AFAIK name changes both the vala name and the cname. rename_to adds the required code so that when the rename_to'ed type is used, the c type is used) |
   | `replacement`           | Anything (except parameters)                                 | The thing that replaces this                          | Specifies a replacement for a deprecated symbol              |
   | `sentinel`              | Function (with ellipsis)                                     | C value                                               | The sentinel value marking the end of the vararg list        |
   | `simple_type`           | Struct                                                       | 0, 1                                                  | Marks the struct as being a simple type, like int            |
   | `takes_ownership`       | Parameter (Function, Delegate)                               | 0, 1                                                  |                                                              |
   | `throws`                | Function                                                     | 0, 1                                                  | Marks that the function should use an out parameter instead of throwing an error |
   | `to_string`             | Enum                                                         | C function name                                       |                                                              |
   | `transfer_ownership`    | Function/Delegate/Signal (having a return value), Parameter (Function, Signal) | 0, 1                                                  | Transfers ownership of the value                             |
   | `type_arguments`        | Function/Delegate/Signal (having a return value), Property, Field, Parameter | Vala types, comma separated                           | Restricts the generic type of the thing                      |
   | `type_check_function`   | Class (GObject)                                              | C function/macro name                                 |                                                              |
   | `type_id`               | Struct, Class (GObject)                                      | C macro                                               |                                                              |
   | `type_name`             | Function (having a return value), Property, Parameter, Field | Vala type name                                        | Changes the type of the selected thing. Overwrites old type, so "type_name" must be before any other type modifying metadata |
   | `type_parameters`       | Delegate, Class (non-GObject)                                | Vala generic type parameters, e.g. T, comma separated |                                                              |
   | `unref_function`        | Class (non-GObject)                                          | C function name                                       |                                                              |
   | `value_owned`           | Parameter (Function)                                         | 0, 1                                                  |                                                              |
   | `vfunc_name`            | Function                                                     | C function pointer name                               |                                                              |
   | `virtual`               | Function                                                     | 0, 1                                                  |                                                              |
   | `weak`                  | Field                                                        | 0, 1                                                  | Marks the field as weak                                      |

## GIR metadata 格式

GIR格式实际上有很多用于生成绑定的信息，但它与Vala是不同的语言。因此，几乎不可能将整个.gir文件直接映射到Vala树中，因此需要元数据。另一方面，我们可能希望直接使用. GIR + .metadata，而不是生成.vapi，但是.vapi比GIR更易于理解，解析起来也更快，因此需要使用vapigen来生成.vapi。

### 定位元数据

SomeLib元数据的文件名。gir必须是SomeLib.metadata。默认情况下，Vala在.gir文件的相同目录中查找.metadata，但是可以使用——metadatadir选项指定其他目录。

### 注释

元数据中的注释与Vala代码中的语法相同:

```shell
// this is a comment
/*
 * multi-line comment
 */
```

### 语法

每个符号的元数据信息必须在不同的行上提供:

```vala
rule:
	pattern [ arguments ] [  relative-rules ] 

relative-rules:
	. pattern [ arguments ] [  relative-rules ]

pattern:
	[ # selector ] [ . pattern ]

arguments:
	[ = expression ] [ arguments ]

expression:
	null
	true
	false
	- expression
	integer-literal
	real-literal
	string-literal
	symbol

symbol:
	identifier [ . identifier ]
```

- 模式被绑定到GIR树:如果一个类FooBar包含一个方法baz_method，那么它就可以在元数据中作为FooBar.baz_method被引用。

- 选择器用于指定GIR树的特定元素名，例如FooBar。Baz_method #方法将只选择名称为Baz_method的方法元素。用于解决名称冲突。
- 给定一个名为Foo的命名空间，一个特殊的模式Foo可用于设置一般参数。
- 如果一个GIR符号匹配多个规则，那么所有的规则都将被应用:如果参数之间存在冲突，文件中最后写入的规则优先。
- 如果未提供参数的表达式，则默认将其视为true。
- 相对规则是相对于最近的前一个绝对规则。元数据必须包含至少一个绝对规则。不可能制定一个相对于另一个相对规则的规则。

### 有效的参数

| **Name**           | **Applies to**                                      | **Type** | **Description**                                              |
| ------------------ | --------------------------------------------------- | -------- | ------------------------------------------------------------ |
| skip               | all                                                 | bool     | Skip processing the symbol                                   |
| hidden             | all                                                 | bool     | Process the symbol but hide from output                      |
| type               | method, parameter, property, field, constant, alias | string   | Complete Vala type                                           |
| type_arguments     | method, parameter, property, field, constant, alias | string   | Vala type parameters for generics, separated by commas       |
| cheader_filename   | all including namespace                             | string   | C headers separated by commas                                |
| name               | all including namespace                             | string   | Vala symbol name                                             |
| owned              | parameter                                           | bool     | Whether the parameter value should be owned                  |
| unowned            | method, property, field, constant                   | bool     | Whether the symbol is unowned                                |
| parent             | all                                                 | string   | Move the symbol to the specified container symbol. If no container exists, a new namespace will be created. |
| nullable           | method, parameter, property, field, constant, alias | bool     | Whether the type is nullable or not                          |
| deprecated         | all                                                 | bool     | Whether the symbol is deprecated or not                      |
| replacement        | all                                                 | string   | Deprecation replacement, implies `deprecated=true`           |
| deprecated_since   | all                                                 | string   | Deprecated since version, implies `deprecated=true`          |
| array              | method, parameter, property, field, constant, alias | bool     | Whether the type is an array or not                          |
| array_length_idx   | parameter                                           | int      | The index of the C array length parameter                    |
| default            | parameter                                           | any      | Default expression for the parameter                         |
| out                | parameter                                           | bool     | Whether the parameter direction is out or not                |
| ref                | parameter                                           | bool     | Whether the parameter direction is ref or not                |
| vfunc_name         | method                                              | string   | Name of the C virtual function                               |
| virtual            | method, signal, property                            | bool     | Whether the symbol is virtual or not                         |
| abstract           | method, signal, property                            | bool     | Whether the symbol is abstract or not                        |
| scope              | parameter (async method)                            | string   | Scope of the delegate, in GIR terms                          |
| struct             | record (detected as boxed compact class)            | bool     | Whether the boxed type must be threaten as struct instead of compact class |
| printf_format      | method                                              | bool     | Add the [PrintfFormat] attribute to the method if true       |
| array_length_field | field (array)                                       | string   | The name of the length field                                 |
| sentinel           | method                                              | string   | C expression of the last argument for varargs                |
| closure            | parameter                                           | int      | Specifies the index of the parameter representing the user data for this callback |
| errordomain        | enumeration                                         | bool     | Whether the enumeration is an errordomain or not             |
| destroys_instance  | method                                              | bool     | Whether the instance is owned by the method                  |
| throws             | method                                              | string   | Type of exception the method throws                          |

### Overriding Types

当你有以下表达式:

```vala
typedef GList MyList;
```

其中，GList将保存整数，使用类型元数据如下:

```vala
MyList type="GLib.List<int>"
```

以上元数据将生成以下代码:

```vala
public class MyList : GLib.List<int> {
	[CCode (has_construct_function = false)]
    protected MyList ();
    public static GLib.Type get_type ();
}
```

然后你可以使用 GLib.List 或 NameSpace.MyList 

