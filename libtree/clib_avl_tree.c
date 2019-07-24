#include "clib_avl_tree.h"
#include <stdlib.h>

/* avl 树 */
struct _avl_tree_t {
    unsigned int            node_num;
    avl_tree_node_t*        root;
    avl_tree_compare        cmp_func;
};

/* avl 树节点 */
struct _avl_tree_node_t {
    int                     height;
    void*                   key;
    void*                   value;
    avl_tree_node_t*        parent;
    avl_tree_node_t*        children[2];
};

/* 创建新的 AVL 树 */
void *avl_tree_new(avl_tree_compare func) {
    if (NULL == func) {
        return NULL;
    }

    avl_tree_t*             tree = NULL;
    tree = (avl_tree_t*)malloc(sizeof(avl_tree_t));
    if (NULL == tree) {
        return NULL;
    }
    tree->root = NULL;
    tree->cmp_func = func;
    tree->node_num = 0;

    return tree;
}

/* 递归删除子节点 */
static void avl_tree_subtree(avl_tree_node_t* node) {
    if (NULL == node) {
        return;
    }
    avl_tree_subtree(node->children[AVL_TREE_NODE_LEFT]);
    avl_tree_subtree(node->children[AVL_TREE_NODE_RIGHT]);
    free(node);
}

/* 销毁 AVL 树 */
void avl_tree_free(avl_tree_t* tree) {
    avl_tree_subtree(tree->root);
    free(tree);
}

/* 返回子树高度 */
int avl_tree_subtree_height(avl_tree_node_t* node) {
    if (NULL == node) {
        return 0;
    }
    return node->height;
}

/* 更新子节点高度值 */
static void avl_tree_update_height(avl_tree_node_t* node) {
    int                     left_height = 0;
    int                     right_height = 0;
    avl_tree_node_t*        left = NULL;
    avl_tree_node_t*        right = NULL;

    left = node->children[AVL_TREE_NODE_LEFT];
    right = node->children[AVL_TREE_NODE_RIGHT];

    left_height = avl_tree_subtree_height(left);
    right_height = avl_tree_subtree_height(right);

    /* 高度取子树高度较大的 */
    if(left_height > right_height) {
        node->height = left_height + 1;
    } else {
        node->height = right_height + 1;
    }
}

/* 节点相对于父节点在那一边 */
static avl_tree_side_t avl_tree_node_parent_sider(avl_tree_node_t* node) {
    if (node == node->parent->children[AVL_TREE_NODE_LEFT]){
        return AVL_TREE_NODE_LEFT;
    }

    return AVL_TREE_NODE_RIGHT;
}

/* 两节点互换 */
static void avl_tree_node_replace(avl_tree_t* tree,
        avl_tree_node_t* node1, avl_tree_node_t* node2) {
    avl_tree_side_t side;
    if(NULL != node2) {
        node2->parent = node1->parent;
    }
    if (NULL == node1->parent) {
        tree->root = node2;
    } else {
        side = avl_tree_node_parent_sider(node1);
        // node1->parent <==> 要删除的节点
        node1->parent->children[side] = node2;
        avl_tree_update_height(node1->parent);
    }
}

/* 二叉树旋转 */
static avl_tree_node_t* avl_tree_rotate(avl_tree_t* tree, avl_tree_node_t* node, avl_tree_side_t side) {
    avl_tree_node_t* new_root = NULL;

    /**
     * 树的旋转
     *    左旋转:                      右旋转
     *
     *      B                             D
     *     / \                           / \
     *    A   D                         B   E
     *       / \                       / \
     *      C   E                     A   C
     *
     *    旋转为:                      旋转为:
     *        D                           B
     *       / \                         / \
     *      B   E                       A   D
     *     / \                             / \
     *    A   C                           C   E
     *
     * 右子左旋  左子右旋
     *
     * 针对如下失衡子树(括号里表示高度)
     *                  z (4)                               (3)  y
     *                /  \                                    /    \
     *           (3) y   T4 (1)         ------>           (2)x      z
     *             /  \                                    /  \   /  \
     *         (2)x   T3 (1)                           (1)T1  T2 T3  T4
     *          /  \
     *        T1  T2(1)
     *
     *  显然 z 节点左右子节点失衡,且左子树高度大于右子树高度,需要右旋转
     *  第一步:
     *      找到失衡的左子(相对较高的子节点): y 作为新子树的根节点
     *      并将 y 变为旋转后子树的根节点，更改 y 的父指针
     *  第二步:
     *      将 T3 给z的左子节点
     */
    new_root = node->children[1 - side];
    avl_tree_node_replace(tree, node, new_root);
    node->children[1 - side] = new_root->children[side];
    new_root->children[side] = node;
    node->parent = new_root;
    if(NULL != node->children[1 - side]){
        node->children[1 - side]->parent = node;
    }
    avl_tree_update_height(new_root);
    avl_tree_update_height(node);

    return new_root;
}

/* 二叉树节点节点平衡 */
static avl_tree_node_t* avl_tree_node_balance(avl_tree_t* tree, avl_tree_node_t* node) {
    int diff = 0;
    avl_tree_node_t* left_tree = NULL;
    avl_tree_node_t* right_tree = NULL;
    avl_tree_node_t* child = NULL;

    left_tree = node->children[AVL_TREE_NODE_LEFT];
    right_tree = node->children[AVL_TREE_NODE_RIGHT];
    diff = avl_tree_subtree_height(left_tree) - avl_tree_subtree_height(right_tree);
    if (diff >= 2) {
        child = node->children[AVL_TREE_NODE_LEFT];
        if(avl_tree_subtree_height(child->children[AVL_TREE_NODE_LEFT])
                < avl_tree_subtree_height(child->children[AVL_TREE_NODE_RIGHT])) {
            avl_tree_rotate(tree, left_tree, AVL_TREE_NODE_LEFT);
        }
        node = avl_tree_rotate(tree, node, AVL_TREE_NODE_RIGHT);
    } else if (diff <= -2) {
        child = right_tree;
        if(avl_tree_subtree_height(child->children[AVL_TREE_NODE_RIGHT])
                < avl_tree_subtree_height(child->children[AVL_TREE_NODE_LEFT])) {
            avl_tree_rotate(tree, node, AVL_TREE_NODE_RIGHT);
        }
        node = avl_tree_rotate(tree, node, AVL_TREE_NODE_LEFT);
    }
    avl_tree_update_height(node);

    return node;
}

/* 二叉树重新平衡 */
static void avl_tree_balance_to_root(avl_tree_t* tree, avl_tree_node_t* node) {
    avl_tree_node_t* rover;
    rover = node;
    while (NULL != rover) {
        rover = avl_tree_node_balance(tree, rover);
        rover = rover->parent;
    }
}

/* 插入 key value */
avl_tree_node_t* avl_tree_insert(avl_tree_t* tree, void* key, void* value) {
    avl_tree_node_t** rover = NULL;
    avl_tree_node_t* new_node = NULL;
    avl_tree_node_t* prev_node = NULL;

    rover = &(tree->root);
    prev_node = NULL;

    while (NULL != *rover) {
        prev_node = *rover;
        if (tree->cmp_func(key, (*rover)->key) < 0) {
            rover = &((*rover)->children[AVL_TREE_NODE_LEFT]);
        } else if (tree->cmp_func(key, (*rover)->key) > 0) {
            rover = &((*rover)->children[AVL_TREE_NODE_RIGHT]);
        } else {
            return *rover;
        }
    }

    new_node = (avl_tree_node_t*)malloc(sizeof(avl_tree_node_t));
    if(NULL == new_node) {
        return NULL;
    }
    new_node->children[AVL_TREE_NODE_LEFT] = NULL;
    new_node->children[AVL_TREE_NODE_RIGHT] = NULL;
    new_node->parent = prev_node;
    new_node->height = 1;
    new_node->key = key;
    new_node->value = value;
    *rover = new_node;
    avl_tree_balance_to_root(tree, prev_node);
    ++(tree->node_num);

    return new_node;
}

/* 根据给定节点 查找树中节点并替代 */
static avl_tree_node_t* 
avl_tree_node_get_replacement(avl_tree_t* tree, avl_tree_node_t* node) {
    avl_tree_node_t* left = NULL;
    avl_tree_node_t* right = NULL;
    avl_tree_node_t* result = NULL;
    int left_height = 0;
    int right_height = 0;
    int side;

    left = node->children[AVL_TREE_NODE_LEFT];
    right = node->children[AVL_TREE_NODE_RIGHT];
    if((NULL == left) && (NULL == right)) {
        return NULL;
    }

    left_height = avl_tree_subtree_height(left);
    right_height = avl_tree_subtree_height(right);
    side = left_height < right_height?AVL_TREE_NODE_RIGHT:AVL_TREE_NODE_LEFT;
    result = node->children[side];
    // 摘下要删除节点的(某一子节点)
    avl_tree_node_replace(tree, result, NULL);
    avl_tree_update_height(result->parent);

    return result;/* 返回摘下的子节点 */ 
}

/* 删除节点 */
void avl_tree_remove_node(avl_tree_t *tree, avl_tree_node_t *node) {
    if ((NULL == tree) || (NULL == node)) {
        return;
    }
    int i = 0;
    avl_tree_node_t* swap_node = NULL;
    avl_tree_node_t* balance_point = NULL;

    swap_node = avl_tree_node_get_replacement(tree, node); 
    if(NULL == swap_node) { // 删除节点是叶子节点 
        avl_tree_node_replace(tree, node, NULL);
        balance_point = node->parent;
    } else {
        if(swap_node->parent == node) {
            balance_point = swap_node;
        } else {
            balance_point = swap_node->parent;
        }
        for(i = 0; i < 2; ++i) {
            swap_node->children[i] = node->children[i];
            if(NULL != swap_node->children[i]){
                swap_node->children[i]->parent = swap_node;
            }
        } // ??? 少了取下来节点的 左右子树啊啊啊啊啊？？？？？？？
        swap_node->height = node->height;
        avl_tree_node_replace(tree, node, swap_node);
    }
    free(node);
    --(tree->node_num);
    avl_tree_balance_to_root(tree, balance_point);
}

/* 查询 key 所在节点 */
avl_tree_node_t* avl_tree_lookup_node(avl_tree_t* tree, void* key) {
    if ((NULL == tree) || (NULL == key)) {
        return NULL;
    }
    int diff = 0;
    avl_tree_node_t* node = tree->root;
    while (NULL != node) {
        diff = tree->cmp_func(key, node->key);
        if(0 == diff) {
            return node;
        } else if (-1 == diff) {
            node = node->children[AVL_TREE_NODE_LEFT];
        } else {
            node = node->children[AVL_TREE_NODE_RIGHT];
        }
    }
    return NULL;
}

/* 通过比较 key 进行删除 */
int avl_tree_remove(avl_tree_t* tree, void* key) {
    if((NULL == tree) || (NULL == key)) {
        return -1;
    }
    avl_tree_node_t* node = NULL;
    node = avl_tree_lookup_node(tree, key);
    if(NULL != node) {
        avl_tree_remove_node(tree, node);
    }
    return 0;
}

/* 根据节点返回 key */
void* avl_tree_node_key(avl_tree_node_t* node) {
    if (NULL == node) {
        return NULL;
    }
    return node->key;
}

/* 根据节点返回 value */
void* avl_tree_node_value(avl_tree_node_t* node) {
    if (NULL == node) {
        return NULL;
    }
    return node->value;
}

/* 根据 key 值获取 value */
void *avl_tree_lookup_value(avl_tree_t *tree, void *key) {
    if (NULL == tree || NULL == key) {
        return NULL;
    }
    avl_tree_node_t* node = avl_tree_lookup_node(tree, key);
    return avl_tree_node_value(node);
}

avl_tree_node_t *avl_tree_root_node(avl_tree_t *tree) {
    if (NULL == tree) {
        return NULL;
    }
    return tree->root;
}

/* 查询给定树节点的子节点 */
avl_tree_node_t *avl_tree_node_child(avl_tree_node_t *node, avl_tree_side_t side) {
    if(AVL_TREE_NODE_LEFT == side || AVL_TREE_NODE_RIGHT == side) {
        return node->children[side];
    }
    return NULL;
}

/* 返回树节点的父节点 */
avl_tree_node_t *avl_tree_node_parent(avl_tree_node_t *node) {
    if(NULL != node) {
        return node->parent;
    }
    return NULL;
}

/* 返回树节点的个数 */
unsigned int avl_tree_num(avl_tree_t *tree) {
    return tree->node_num;
}

void preorder_print_tree(avl_tree_node_t *node, avl_tree_print_key print) {
    if(NULL == node || NULL == print) {
        return;
    }
    print(node->key);
    preorder_print_tree(node->children[AVL_TREE_NODE_LEFT], print);
    preorder_print_tree(node->children[AVL_TREE_NODE_RIGHT], print);
}

void midorder_print_tree(avl_tree_node_t *node, avl_tree_print_key print) {
    if(NULL == node || NULL == print) {
        return;
    }
    midorder_print_tree(node->children[AVL_TREE_NODE_LEFT], print);
    print(node->key);
    midorder_print_tree(node->children[AVL_TREE_NODE_RIGHT], print);
}

void postorder_print_tree(avl_tree_node_t *node, avl_tree_print_key print) {
    if(NULL == node || NULL == print) {
        return;
    }
    postorder_print_tree(node->children[AVL_TREE_NODE_LEFT], print);
    postorder_print_tree(node->children[AVL_TREE_NODE_RIGHT], print);
    print(node->key);
}
