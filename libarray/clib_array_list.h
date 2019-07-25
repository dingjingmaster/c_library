/*************************************************************************
> FileName: clib_array_list.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年07月24日 星期三 16时45分13秒
 ************************************************************************/
#ifndef _CLIB_ARRAY_LIST_H
#define _CLIB_ARRAY_LIST_H
#ifdef __cplusplus
extern "C" {
#endif

typedef void* array_value_t;
typedef struct _array_list_t array_list_t;
typedef int (*array_list_value_cmp) (array_value_t v1, array_value_t v2);

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
 * 追加新的元素
 *
 * @param array: 
 * @param value: 要追加的元素值
 * @return 成功：返回要追加的元素
 *         失败：返回NULL
 */
array_value_t array_list_append(array_list_t* array, array_value_t value);


#ifdef __cplusplus
}
#endif
#endif
