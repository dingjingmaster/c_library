/*************************************************************************
> FileName: clib-array-list.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年07月24日 星期三 16时45分13秒
 ************************************************************************/
#ifndef _CLIBARRAYLIST_H
#define _CLIBARRAYLIST_H
#ifdef __cplusplus
extern "C" {
#endif

typedef void* array_value_t;
typedef struct _array_list_t array_list_t;
typedef int (*array_list_value_cmp) (array_value_t v1, array_value_t v2);
typedef void(*array_list_print_cb) (array_value_t v);

/**
 * 创建新的 array_list 
 * 
 * @param cap: 初始数组容量
 * @param cb: 数组元素值的大小比较函数，用来查找和排序
 * @return 成功: 返回给定数组
 *         失败: 返回 NULL
 */
array_list_t* array_list_new(unsigned long cap, array_list_value_cmp cb);

/**
 * 销毁 array_list
 *
 * @param array: 数组
 */
void array_list_free(array_list_t** array);

/**
 * 追加新的元素
 *
 * @param array: 
 * @param value: 要追加的元素值
 * @return 成功：返回要追加的元素
 *         失败：返回NULL
 */
array_value_t array_list_append(array_list_t* array, array_value_t value);

/**
 * 获取当前容量
 *
 * @param array:
 * @return 返回动态链表长度
 */
unsigned int array_list_length(array_list_t* array);

/**
 * 打印值
 * @param array:
 * @param print: 值的打印函数
 */
void array_list_print(array_list_t* array, array_list_print_cb print);

#ifdef __cplusplus
}
#endif
#endif
