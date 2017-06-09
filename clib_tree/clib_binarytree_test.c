#include "clib_binarytree.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    sDjBinaryTree*  testTree = NULL;
    printf("binary tree testing!\n");

    binaryTree_create(&testTree);

    binaryTreec_insert (testTree, testTree ->treeNode, "ddd");


    return 0;
}
