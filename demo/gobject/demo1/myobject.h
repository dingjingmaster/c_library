/*************************************************************************
> FileName: myobject.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月02日 星期一 18时14分13秒
 ************************************************************************/
#ifndef _MYOBJECT_H
#define _MYOBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

#include <glib-object.h>
 
#define MY_TYPE_OBJECT (my_object_get_type ())
#define MY_OBJECT(object) (G_TYPE_CHECK_INSTANCE_CAST((object), MY_TYPE_OBJECT, MyObject))
#define MY_OBJECT_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), MY_TYPE_OBJECT, MyObjectClass))
#define MY_IS_OBJECT(object) (G_TYPE_CHECK_INSTANCE_TYPE((object), MY_TYPE_OBJECT))
#define MY_IS_OBJECT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), MY_TYPE_OBJECT))
#define MY_OBJECT_GET_CLASS(object) (G_TYPE_INSTANCE_GET_CLASS((object), MY_TYPE_OBJECT, MyObjectClass))

typedef struct _MyObject MyObject;
typedef struct _MyObject
{
    GObject parent_instance;
};
typedef struct _MyObjectClass MyObjectClass;
typedef struct _MyObjectClass
{
    GObjectClass parent_class;
};
 
GType my_object_get_type(void);

#ifdef __cplusplus
}
#endif
#endif
