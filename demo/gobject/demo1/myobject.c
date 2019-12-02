/*************************************************************************
> FileName: myobject.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月02日 星期一 18时14分13秒
 ************************************************************************/
#include "myobject.h"

G_DEFINE_TYPE(MyObject, my_object, G_TYPE_OBJECT);
 
static void
my_object_init(MyObject *self)
{
    printf("对象初始化\n");
}
 
static void
my_object_class_init(MyObjectClass *klass)
{
}


