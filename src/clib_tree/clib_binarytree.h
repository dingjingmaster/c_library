#ifndef CLIB_BINARYTREE_H
#define CLIB_BINARYTREE_H

#ifdef __cplusplue
extern "C"{
#endif

typedef struct _sDjBinaryTree sDjBinaryTree;
typedef struct _sDjBinaryTreeNode sDjBinaryTreeNode;

enum
{
    OK = 0x00000000,
    ERROR = 0x00000001,
    EXIST = 0x00000002

};

struct _sDjBinaryTreeNode
{
    void* data;   // Data area
    sDjBinaryTree * left;
    sDjBinaryTree * right;
};

struct _sDjBinaryTree
{
    //  node
    sDjBinaryTreeNode* treeNode;

    //  树的节点数
    unsigned long treeNodeNum;

    //  树的深度
    unsigned long treeDeep;

    //  节点的复制函数
    void* (*nodeDup) (sDjBinaryTreeNode* node);

    //  节点的释放函数
    void* (*nodeFree) (sDjBinaryTreeNode* node);

    //  节点数据的比较 (节点中数据的比较 二叉树 key 值不能相同, 值的比较，返回值 1 代表 data1 大于 data2， 0  代表 二者相等)
    int (*nodeCompare)(void* data1, void* data2);
};

/*  创建一个二叉树 */
int binaryTree_create(sDjBinaryTree** tree);

/*  二叉树的插入  */
int binaryTreec_insert(sDjBinaryTree* tree, sDjBinaryTreeNode** treeNode, void* data);





#ifdef __cplusplue
}
#endif
#endif // CLIB_BINARYTREE_H
