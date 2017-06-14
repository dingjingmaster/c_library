#include "clib_binarytree.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//  二叉树创建
int binaryTree_create(sDjBinaryTree **tree)
{
    sDjBinaryTree* newTree = NULL;

    //  分配内存
    newTree = (sDjBinaryTree*)malloc(sizeof(sDjBinaryTree));
    if(NULL == newTree)
    {
        return NULL;
    }

    //  初始化
    newTree ->nodeDup = NULL;
    newTree ->nodeFree = NULL;
    newTree ->treeNode = NULL;
    newTree ->treeDeep = 0;
    newTree ->treeNodeNum = 0;

    *tree = newTree;

    return OK;
}

int binaryTreec_insert(sDjBinaryTree* tree, sDjBinaryTreeNode** rootNode, void *data)
{
    if(NULL == tree || NULL == rootNode || NULL == data)
    {
        return NULL;
    }

    int resCompare = 0;
    unsigned int curDataLen = 0;
    unsigned int dataLen = 0;
    sDjBinaryTreeNode*  tempNode = NULL;
    sDjBinaryTreeNode*  curNode = NULL;

    //  插入操作 如果已经到了最后一个节点
    if(NULL == (*rootNode))
    {
        //  新节点
        tempNode = (sDjBinaryTreeNode*)malloc(sizeof(sDjBinaryTreeNode));
        tempNode ->left = NULL;
        tempNode ->right = NULL;
        tempNode ->data = data;

        *rootNode = tempNode;

        return OK;
    }
    /*    if (val < (*tree)->data) {
            insert(&(*tree)->left,val);
        }else if (val > (*tree)->data) {
            insert(&(*tree)->right,val);
        }

    */

    //  拥有节点比较函数的情况下还是用节点比较函数， 否则当字符串处理
    if(NULL != (tree ->nodeCompare))
    {
        resCompare =  tree ->nodeCompare(curNode ->data, data);
    }
    else
    {
        curDataLen = strlen(curNode ->data);
        dataLen = strlen(data);

        resCompare = memcmp (curNode ->data, data, curDataLen > dataLen ? dataLen : curDataLen);
        if(0 == resCompare)
        {
            resCompare = curDataLen - dataLen;
        }
    }

    //  左节点还是有节点进行查找插入位置
    if(resCompare > 0)
    {
        curNode = curNode ->left;
    }
    else if(resCompare < 0)
    {
        curNode = curNode ->right;
    }
    else
    {
        return EXIST;
    }

    binaryTreec_insert (tree, &curNode, data);
}


