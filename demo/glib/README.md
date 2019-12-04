### glib-dbus 接口描述的type域数据类型

|     |     |
| --- | --- |
| a   | array 数组 |
| b   | bollean 布尔值 |
| d   | double 双精度浮点 |
| g   | SIGNATURE 类型签名 |
| i   | INT32 |
| n   | INT16 |
| o   | object_path 对象路径 |
| q   | UINT16 16位无符号整数 |
| s   | string 0 结尾的UTF-8字符串 |
| t   | UINT64 |
| u   | UINT32 |
| v   | VARIANT 可放任意类型的容器，数据中包含类型信息。例如glib中的GValue |
| x   | INT64 64位由符号整数 |
| y   | BYTE 8位无符号整数 |
| ()  | 定义结构时使用。例如 "(i(ii))" |
| {}  | 定义键值对时使用。例如 "a{us}" |
