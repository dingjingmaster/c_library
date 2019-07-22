#ifndef CLIB_AVL_TREE_H
#define CLIB_AVL_TREE_H

/**
 * 平衡二叉树
 *  AVL 是一种平衡二叉树
 *  树的每个节点都有一个 key 和与之对应的 value
 *  节点根据 key 的顺序排列
 *  即使修改输的节点，树依然保持平衡(树两边节点数量大致相等)
 *
 *  用途：
 *      1. 用作映射 (用于搜索)
 *      2. 总是有序的 key
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _avl_tree_t  avl_tree_t;
typedef struct _avl_tree_node_t avl_tree_node_t;

typedef enum {
    AVL_TREE_NODE_LEFT = 0,
    AVL_TREE_NODE_RIGHT = 1
} avl_tree_side_t;

/**
 * 遍历树的 key
 */
typedef void (*avl_tree_print_key) (void* key);

/**
 * 比较 tree 节点 key 值的函数指针
 *
 * @param k1   k1
 * @param k2   k2
 *
 * @return     k1 <  k2  返回 -1
 *             k1 == k2  返回 0
 *             k1 >  k2  返回 1
 */
typedef int (*avl_tree_compare)(void* k1, void* v2);

/**
 * 创建 AVL 树
 *
 * @param tree  avl树的句柄
 * @return      成功: 返回AVL树的句柄
 *              失败：返回NULL
 */
void* avl_tree_new(avl_tree_compare func);

/**
 * 销毁 AVL 树
 */
void avl_tree_free(avl_tree_t* tree);

/**
 * 插入一个 key-value 对
 *
 * @param tree  avl树的句柄
 * @param key   要插入的 key
 * @param value 要插入的节点
 */
avl_tree_node_t* avl_tree_insert(avl_tree_t* tree, void* k, void* v);

/**
 * 根据 key 查询avl树
 *
 * @param tree  树
 * @param key   要查询的key
 * @return      成功：返回找到的节点
 *              失败：返回 NULL
 */
avl_tree_node_t* avl_tree_lookup_node(avl_tree_t* tree, void* key);

/**
 * 删除一个节点
 *
 * @param tree  avl树的句柄
 * @param node  要删除的节点
 */
void avl_tree_remove_node(avl_tree_t* tree, avl_tree_node_t* node);


/**
 * 根据 key 查询avl树的值
 *
 * @param tree  树
 * @param key   要查询的key
 * @return      成功：返回找到的值
 *              失败：返回 NULL
 */
void* avl_tree_lookup_value(avl_tree_t* tree, void* key);

/**
 * 返回树的根节点
 *
 * @param tree  树
 * @return      成功：返回树的根节点
 *              失败：返回 NULL
 */
avl_tree_node_t* avl_tree_root_node(avl_tree_t* tree);

/**
 * 返回给定树节点的建
 *
 * @param node  给定节点
 * @return      返回键
 */
void *avl_tree_node_key(avl_tree_node_t* node);

/**
 * 返回树节点的值
 *
 * @param node  给定节点
 * @return      返回给定节点的值
 */
void* avl_tree_node_value(avl_tree_node_t* node);

/**
 * 查询给定树节点的子节点
 *
 * @param node  树节点
 * @param side  左子树还是右子树
 * @return      成功：返回子节点
 *              失败：返回NULL
 */
avl_tree_node_t* avl_tree_node_child(avl_tree_node_t* node, avl_tree_side_t side);

/**
 * 返回树节点的父节点
 *
 * @param node  树节点
 * @return      成功：返回父节点
 *              失败：返回NULL
 */
avl_tree_node_t* avl_tree_node_parent(avl_tree_node_t* node);

/**
 * 返回树的高度
 *
 * @param node  树节点
 * @return      子树的高度
 */
int avl_tree_subtree_height(avl_tree_node_t* node);

/**
 * 检索树的节点数
 *
 * @param tree  树
 * @return      树的节点数
 */
unsigned int avl_tree_num(avl_tree_t* tree);

/**
 * 树的前序遍历(调试使用)
 */
void preorder_print_tree(avl_tree_node_t* node, avl_tree_print_key print);

/**
 * 树的中序遍历(调试使用)
 */
void midorder_print_tree(avl_tree_node_t* node, avl_tree_print_key print);

/**
 * 树的后序遍历(调试使用)
 */
void postorder_print_tree(avl_tree_node_t* node, avl_tree_print_key print);
#ifdef __cplusplus
}
#endif
#endif // CLIB_AVL_TREE_H
