#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string.h>
#include <set>
#include <queue>
#include "extmem.h"

typedef struct node
{
    void *val;
    struct node *next;
} node_t;

// STRUCTURE DEFINITION
typedef struct treeNode
{
    bool is_root;
    short numKeys;
    int height;
    int values[7];
    int nodes[7];
} treeNode;

Buffer buf;

const int RA_VALUE = 40, SC_VALUE = 60;

node_t *nodes = NULL;

node_t *addresses = NULL;

int buffer_num = 0;

// Root of the tree
treeNode *root;
int root_address = 100;

// Flag activated only for tests
bool flagDebug = true;

// Here it's defined the tree order, works better for even's higher than 2 numbers
int maxOrder = 5;

// Tree Height
int treeHeight = 1;

// Node num keys management methods
int numKeys(treeNode *node)
{
    return (int)(node->numKeys);
}

void setNumKeys(treeNode *node, int value)
{
    node->numKeys = (short)value;
}

// if hasn't any parent it's the tree root
bool isRoot(treeNode *parent)
{
    return parent == NULL;
}

// if the node is in the same height than the tree height, then it's a leave
bool isLeave(int height)
{
    return height == treeHeight;
}

bool isEmptyNode(treeNode *node)
{
    return numKeys(node) == 0;
}

bool haveOverflow(treeNode *node)
{
    return numKeys(node) > maxOrder;
}

treeNode *getNode(int address)
{
    if (buffer_num < 8)
    {
        treeNode *blk;
        if ((blk = readBlockFromDisk(address, &buf)) == NULL)
        {
            perror("Reading Block Failed!\n");
            return -1;
        }
        return (treeNode *)blk;
    }
    else
    {
    }
}

writeNode(treeNode *node, int address)
{
    if (writeBlockToDisk((unsigned char *)node, address, &buf) != 0)
    {
        perror("Writing Block Failed!\n");
        return -1;
    }
}

// Creates a new node allocating all the needed resources
treeNode *createNode()
{
    /* Get a new block in the buffer */
    treeNode *tree = (treeNode *)getNewBlockInBuffer(&buf);
    memset(tree, 0, 64);
    return (treeNode *)getNewBlockInBuffer(&buf);
}

//
// KEY POSITION METHODS
//
// Find key insert position
int findKeyInsertPosition(treeNode *node, int key)
{
    int lastIndex = numKeys(node);

    int position = 0;
    while (key > node->values[position] && position < lastIndex)
    {
        position = position + 1;
    }
    return position;
}

// Find key position or -1 for failure
int findKeyPosition(treeNode *node, int key)
{
    int keyPosition = -1;
    int lastIndex = numKeys(node);

    int position = 0;
    while (position < lastIndex)
    {
        if (node->values[position] == key)
        {
            keyPosition = position;
            break;
        }

        position = position + 1;
    }
    return keyPosition;
}

// Get the value of the last positioned value in the node
int findLastKeyValue(treeNode *node)
{
    int lastIndex = numKeys(node);

    return node->values[lastIndex - 1];
}

// Shift positions to use a tree with array - Right is used for insertions
void shiftRightPositions(treeNode *node, int startPosition, int endPosition)
{
    while (endPosition >= startPosition)
    {
        node->values[endPosition + 1] = node->values[endPosition];
        node->nodes[endPosition + 1] = node->nodes[endPosition];
        endPosition--;
    }
}

// Shift positions to use a tree with array - Left is used for deletions
void shiftLeftPositions(treeNode *node, int startPosition, int endPosition)
{
    while (endPosition >= startPosition)
    {
        node->values[startPosition] = node->values[startPosition + 1];
        node->nodes[startPosition] = node->nodes[startPosition + 1];
        startPosition++;
    }
}

// Check if the key is on tree recursively
bool keyIsOnTree(treeNode *node, int key, int height)
{
    bool isOnTree = false;

    if (numKeys(node) != 0)
    {
        if (isLeave(height) && findKeyPosition(node, key) != -1)
        {
            isOnTree = true;
        }
        else
        {
            int position = findKeyInsertPosition(node, key);
            isOnTree = keyIsOnTree(node->nodes[position], key, height + 1);
        }
    }

    return isOnTree;
}

//
// START INSERTION METHODS
//

// The insert in leaves methods sets the value and creates emptys nodes to allow futures insertions
void insertInEmptyLeave(treeNode *leave, int key)
{
    leave->nodes[0] = createNode();
    leave->nodes[1] = createNode();

    leave->values[0] = key;
}

void insertInLeaveLastPosition(treeNode *leave, int key)
{
    int lastIndex = numKeys(leave);

    // leave->nodes[lastIndex + 1] = createNode(-1);
    leave->nodes[lastIndex + 1] = leave->nodes[lastIndex];

    leave->values[lastIndex] = key;
}

// This method insert in a position shifting values to right to allow a positional insert
void insertInPosition(treeNode *leave, int key, int index)
{
    int lastIndex = numKeys(leave);

    shiftRightPositions(leave, index, lastIndex + 1);

    leave->nodes[index + 1] = createNode();

    leave->values[index] = key;
}

// Checks how it's the best way to insert in a given leave and do it
void insertInLeave(treeNode *leave, int key)
{
    int size = numKeys(leave);

    if (isEmptyNode(leave))
    {
        insertInEmptyLeave(leave, key);
    }
    else
    {
        int position = findKeyInsertPosition(leave, key);

        if (position == size)
        {
            insertInLeaveLastPosition(leave, key);
        }
        else
        {
            insertInPosition(leave, key, position);
        }
    }

    setNumKeys(leave, (size + 1));
}

// This method insert some value in a non-leave (comes from promotion)
// In the process it's gets the values bigger than the promoted to a new node
void splitLeave(treeNode *node, int key, int height)
{
    int size = numKeys(node);

    int position = findKeyInsertPosition(node, key);

    insertInPosition(node, key, position);

    treeNode *oldNode = node->nodes[position];
    treeNode *newNode = node->nodes[position + 1];

    int middleIndex = (int)(maxOrder / 2);

    int index = 0;
    while (index <= middleIndex)
    {
        newNode->values[index] = oldNode->values[index + middleIndex];

        newNode->nodes[index] = oldNode->nodes[index + middleIndex];
        index++;
    }

    newNode->nodes[index] = oldNode->nodes[index + middleIndex];
    oldNode->nodes[index - 1] = newNode;

    setNumKeys(newNode, index);
    setNumKeys(oldNode, numKeys(oldNode) - index);

    setNumKeys(node, (size + 1));
}

// This method insert some value in a non-leave (comes from promotion)
// In the process it's gets the values bigger than the promoted to a new node
void splitNonLeave(treeNode *node, int key, int height)
{
    int size = numKeys(node);

    int position = findKeyInsertPosition(node, key);

    insertInPosition(node, key, position);

    treeNode *oldNode = node->nodes[position];
    treeNode *newNode = node->nodes[position + 1];

    int middleIndex = (int)(maxOrder / 2);

    int index = 0;
    while (index < middleIndex)
    {
        newNode->values[index] = oldNode->values[index + middleIndex + 1];

        newNode->nodes[index] = oldNode->nodes[index + middleIndex + 1];
        index++;
    }

    newNode->nodes[index] = oldNode->nodes[index + middleIndex + 1];
    oldNode->nodes[index + middleIndex + 1] = NULL;

    setNumKeys(newNode, index);
    setNumKeys(oldNode, numKeys(oldNode) - index - 1);

    setNumKeys(node, (size + 1));
}

// Special case when overflowing the root element
void keepOnlyMiddleElementInRoot(treeNode *node)
{
    treeNode *newNode = createNode();

    int position = 0;
    int middleIndex = (int)(numKeys(node) / 2);

    insertInLeave(newNode, node->values[middleIndex]);

    treeHeight++;
    // IF the whole tree was a leave until then
    if (treeHeight == 2)
    {
        treeNode *subNode = newNode->nodes[0];
        while (position < middleIndex)
        {
            insertInLeave(subNode, node->values[position]);
            position++;
        }
        subNode->nodes[position] = newNode->nodes[1];

        position = middleIndex;
        subNode = newNode->nodes[1];
        while (position <= maxOrder)
        {
            insertInLeave(subNode, node->values[position]);
            position++;
        }
    }
    else
    {
        treeNode *leftNode = newNode->nodes[0];

        treeNode *rightNode = newNode->nodes[1];

        while (position < middleIndex)
        {
            leftNode->values[position] = node->values[position];
            leftNode->nodes[position] = node->nodes[position];
            position++;
        }
        leftNode->nodes[position] = node->nodes[position];
        setNumKeys(leftNode, middleIndex);

        position = 0;
        while (position <= maxOrder)
        {
            rightNode->values[position] = node->values[position + middleIndex + 1];
            rightNode->nodes[position] = node->nodes[position + middleIndex + 1];
            position++;
        }
        rightNode->nodes[position] = node->nodes[position + middleIndex + 1];
        setNumKeys(rightNode, middleIndex);
    }

    root = newNode;
}

// Promotes middle element and subdivide his elements
void promoteMiddleElement(treeNode *node, treeNode *parent, int height)
{
    if (isRoot(parent))
    {
        keepOnlyMiddleElementInRoot(node);
    }
    else if (isLeave(height))
    {
        int middleIndex = (int)(numKeys(node) / 2);

        splitLeave(parent, node->values[middleIndex], height);
    }
    else
    {
        int middleIndex = (int)(numKeys(node) / 2);

        splitNonLeave(parent, node->values[middleIndex], height);
    }
}

// Main recursive insertion method
void insertInNode(treeNode *node, int key, treeNode *parent, int height)
{
    if (isLeave(height))
    {
        insertInLeave(node, key);

        if (haveOverflow(node))
        {
            promoteMiddleElement(node, parent, height);
        }
    }
    else
    {
        int position = findKeyInsertPosition(node, key);
        insertInNode(node->nodes[position], key, node, height + 1);

        if (haveOverflow(node))
        {
            promoteMiddleElement(node, parent, height);
        }
    }
}

// Process user key insert
void commandInsert(int key)
{
    if (!keyIsOnTree(root, key, 1))
    {
        insertInNode(root, key, NULL, 1);
    }
}

//
// START PRINT METHODS
//

// print the values as height sets to debug
// void printNode(treeNode *node, int height)
// {
//     int position = 0;
//     int numValues = numKeys(node);

//     printf("Keys - height: %d\n", height);
//     while (position < numValues)
//     {
//         printf("%d ", node->values[position]);
//         position++;
//     }
//     if (position > 0)
//     {
//         printf("\n");
//     }

//     if (numValues > 0)
//     {
//         position = 0;

//         if (numKeys(node->nodes[0]) > 0)
//         {
//             while (position <= numValues)
//             {
//                 if (node->nodes[position]->numKeys[0] > 0)
//                 {
//                     printNode(node->nodes[position], height + 1);
//                 }
//                 position++;
//             }

//             if (position > 0)
//             {
//                 printf("\n");
//             }
//         }
//     }
// }

// print the values as a ordered list
// void printOrdered(treeNode *node, int height)
// {
//     int size = numKeys(node);
//     int position = 0;

//     if (size > 0)
//     {
//         while (position < size)
//         {
//             printOrdered(node->nodes[position], height + 1);
//             if (isLeave(height) && node->values[position] != (int)NULL)
//             {
//                 printf("%d ", node->values[position]);
//             }

//             position++;
//         }
//         printOrdered(node->nodes[size], height + 1);
//     }
// }

// print the values as a ordered list
// void printOrdered1(treeNode *node, int height)
// {
//     int size = numKeys(node);
//     int position = 0;

//     if (size > 0)
//     {
//         if (isLeave(height))
//         {
//             while (position < size)
//             {
//                 if (node->values[position] != (int)NULL)
//                 {
//                     printf("%d ", node->values[position]);
//                 }
//                 position++;
//             }
//             printOrdered1(node->nodes[size], height);
//         }
//         else
//         {
//             printOrdered1(node->nodes[position], height + 1);
//         }
//     }
// }

// Process user key print
// void commandPrint()
// {
//     if (flagDebug)
//     {
//         printNode(root, 1);
//     }

//     printOrdered1(root, 1);
//     printf("\n");
// }

// Allocate through malloc the needed resources
void createTree()
{
    root = createNode();
}

// Run user commands
void runUserIterations()
{
    char operation[60];
    int key = 0;

    while (scanf("%s", operation) != EOF)
    {
        if (strcmp("exit", operation) == 0)
        {
            break;
        }
        else if (strcmp("i", operation) == 0)
        {
            scanf("%d", &key);
            commandInsert(key);
        }
        else if (strcmp("p", operation) == 0)
        {
            // commandPrint();
        }
    }
}

// The magic starts here!
int main()
{
    /* Initialize the buffer */
    if (!initBuffer(520, 64, &buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }
    createTree();

    runUserIterations();

    return 0;
}
