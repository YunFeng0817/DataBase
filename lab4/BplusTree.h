#ifndef BPLUSTREE_H
#define BPLUSTREE_H

// struct of one node in B plus tree
typedef struct treeNode
{
    bool is_leaf;  // flag means if the node is leaf
    short numKeys; // number of value of the current node
    int height;    // height of the whole tree
    int values[7]; // index value
    int nodes[7];  // the disk address of child node
} treeNode;

// node used to store value disk address map to the same index
typedef struct valueNode
{
    int size;
    int values[14];
    int nextBlock;
} valueNode;

treeNode *getNode(int address);
int writeAll();
void createTree();
void createTree(int root_address, Buffer *buffer);
void commandPrint();
void commandInsert(int key, int value);
int commandSearch(int key);
#endif