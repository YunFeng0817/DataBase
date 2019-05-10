#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string.h>
#include <vector>
#include "extmem.h"
using namespace std;

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

vector<treeNode *> buffer_address;
vector<int> addresses;

// disk address of root node
int root_address = 100;
int disk_address = root_address;

// Flag activated only for tests
bool flagDebug = true;

// Here it's defined the tree order, works better for even's higher than 2 numbers
int maxOrder = 4;

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
bool isRoot(int parent)
{
    return parent == 0;
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

int writeNode(treeNode *node, int address)
{
    if (writeBlockToDisk((unsigned char *)node, address, &buf) != 0)
    {
        perror("Writing Block Failed!\n");
        return -1;
    }
    freeBlockInBuffer((unsigned char *)node, &buf);
}

void writeAll()
{
    treeNode *blk;
    while (addresses.size() != 0)
    {
        blk = buffer_address.front();
        int address = addresses.front();
        if (writeNode(blk, address) != -1)
        {
            buffer_address.erase(buffer_address.begin());
            addresses.erase(addresses.begin());
        }
    }
}

void clean_buffer()
{
    treeNode *blk;
    blk = buffer_address.front();
    int address = addresses.front();
    if (writeNode(blk, address) != -1)
    {
        buffer_address.erase(buffer_address.begin());
        addresses.erase(addresses.begin());
    }
}

treeNode *getNode(int address)
{
    treeNode *blk;
    // get block from buffer
    for (int i = 0; i < addresses.size(); i++)
    {
        if (addresses[i] == address)
        {
            return buffer_address[i];
        }
    }
    // the block isn't in the buffer
    if (addresses.size() >= 8)
    {
        clean_buffer();
    }
    if ((blk = (treeNode *)readBlockFromDisk(address, &buf)) == NULL)
    {
        perror("Reading Block Failed!\n");
        cout << address << endl;
        return NULL;
    }
    buffer_address.push_back(blk);
    addresses.push_back(address);
    return (treeNode *)blk;
}

// Creates a new node allocating all the needed resources
treeNode *createNode()
{
    /* Get a new block in the buffer */
    if (addresses.size() >= 8)
    {
        clean_buffer();
    }
    treeNode *tree = (treeNode *)getNewBlockInBuffer(&buf);
    memset(tree, 0, 64);
    buffer_address.push_back(tree);
    addresses.push_back(disk_address);
    disk_address++;
    return tree;
}

//
// KEY POSITION METHODS
//
// Find key insert position
int findKeyInsertPosition(int node, int key)
{
    int lastIndex = numKeys(getNode(node));

    int position = 0;
    while (key > getNode(node)->values[position] && position < lastIndex)
    {
        position = position + 1;
    }
    return position;
}

// Find key position or -1 for failure
int findKeyPosition(int node, int key)
{
    int keyPosition = -1;
    int lastIndex = numKeys(getNode(node));

    int position = 0;
    while (position < lastIndex)
    {
        if (getNode(node)->values[position] == key)
        {
            keyPosition = position;
            break;
        }

        position = position + 1;
    }
    return keyPosition;
}

// Get the value of the last positioned value in the node
int findLastKeyValue(int node)
{
    int lastIndex = numKeys(getNode(node));

    return getNode(node)->values[lastIndex - 1];
}

// Shift positions to use a tree with array - Right is used for insertions
void shiftRightPositions(int node, int startPosition, int endPosition)
{
    treeNode *temp = getNode(node);
    while (endPosition >= startPosition)
    {
        temp->values[endPosition + 1] = temp->values[endPosition];
        temp->nodes[endPosition + 1] = temp->nodes[endPosition];
        endPosition--;
    }
}

// Shift positions to use a tree with array - Left is used for deletions
void shiftLeftPositions(int node, int startPosition, int endPosition)
{
    treeNode *temp = getNode(node);
    while (endPosition >= startPosition)
    {
        temp->values[startPosition] = temp->values[startPosition + 1];
        temp->nodes[startPosition] = temp->nodes[startPosition + 1];
        startPosition++;
    }
}

// Check if the key is on tree recursively
bool keyIsOnTree(int node, int key, int height)
{
    bool isOnTree = false;

    if (numKeys(getNode(node)) != 0)
    {
        if (isLeave(height) && findKeyPosition(node, key) != -1)
        {
            isOnTree = true;
        }
        else
        {
            int position = findKeyInsertPosition(node, key);
            isOnTree = keyIsOnTree(getNode(node)->nodes[position], key, height + 1);
        }
    }
    return isOnTree;
}

//
// START INSERTION METHODS
//

// The insert in leaves methods sets the value and creates emptys nodes to allow futures insertions
void insertInEmptyLeave(int leave, int key)
{
    createNode();
    getNode(leave)->nodes[0] = disk_address - 1;
    createNode();
    getNode(leave)->nodes[1] = disk_address - 1;

    getNode(leave)->values[0] = key;
}

void insertInLeaveLastPosition(treeNode *leave, int key)
{
    int lastIndex = numKeys(leave);

    // leave->nodes[lastIndex + 1] = createNode(-1);
    leave->nodes[lastIndex + 1] = leave->nodes[lastIndex];

    leave->values[lastIndex] = key;
}

// This method insert in a position shifting values to right to allow a positional insert
void insertInPosition(int leave, int key, int index)
{
    int lastIndex = numKeys(getNode(leave));

    shiftRightPositions(leave, index, lastIndex + 1);

    createNode();
    getNode(leave)->nodes[index + 1] = disk_address - 1;

    getNode(leave)->values[index] = key;
}

// Checks how it's the best way to insert in a given leave and do it
void insertInLeave(int leave, int key)
{
    int size = numKeys(getNode(leave));

    if (isEmptyNode(getNode(leave)))
    {
        insertInEmptyLeave(leave, key);
    }
    else
    {
        int position = findKeyInsertPosition(leave, key);

        if (position == size)
        {
            insertInLeaveLastPosition(getNode(leave), key);
        }
        else
        {
            insertInPosition(leave, key, position);
        }
    }

    setNumKeys(getNode(leave), (size + 1));
}

// This method insert some value in a non-leave (comes from promotion)
// In the process it's gets the values bigger than the promoted to a new node
void splitLeave(int node, int key, int height)
{
    int size = numKeys(getNode(node));

    int position = findKeyInsertPosition(node, key);

    insertInPosition(node, key, position);

    treeNode *oldNode = getNode(getNode(node)->nodes[position]);
    treeNode *newNode = getNode(getNode(node)->nodes[position + 1]);

    int middleIndex = (int)(maxOrder / 2);

    int index = 0;
    while (index <= middleIndex)
    {
        newNode->values[index] = oldNode->values[index + middleIndex];

        newNode->nodes[index] = oldNode->nodes[index + middleIndex];
        index++;
    }

    newNode->nodes[index] = oldNode->nodes[index + middleIndex];
    oldNode->nodes[index - 1] = getNode(node)->nodes[position + 1];

    setNumKeys(newNode, index);
    setNumKeys(oldNode, numKeys(oldNode) - index);

    setNumKeys(getNode(node), (size + 1));
}

// This method insert some value in a non-leave (comes from promotion)
// In the process it's gets the values bigger than the promoted to a new node
void splitNonLeave(int node, int key, int height)
{
    int size = numKeys(getNode(node));

    int position = findKeyInsertPosition(node, key);

    insertInPosition(node, key, position);

    treeNode *oldNode = getNode(getNode(node)->nodes[position]);
    treeNode *newNode = getNode(getNode(node)->nodes[position + 1]);

    int middleIndex = (int)(maxOrder / 2);

    int index = 0;
    while (index < middleIndex)
    {
        newNode->values[index] = oldNode->values[index + middleIndex + 1];

        newNode->nodes[index] = oldNode->nodes[index + middleIndex + 1];
        index++;
    }

    newNode->nodes[index] = oldNode->nodes[index + middleIndex + 1];
    oldNode->nodes[index + middleIndex + 1] = 0;

    setNumKeys(newNode, index);
    setNumKeys(oldNode, numKeys(oldNode) - index - 1);

    setNumKeys(getNode(node), (size + 1));
}

// Special case when overflowing the root element
void keepOnlyMiddleElementInRoot(int node)
{
    createNode();
    int newNode = disk_address - 1;

    int position = 0;
    int middleIndex = (int)(numKeys(getNode(node)) / 2);

    insertInLeave(newNode, getNode(node)->values[middleIndex]);

    treeHeight++;
    // IF the whole tree was a leave until then
    if (treeHeight == 2)
    {
        int subNode = getNode(newNode)->nodes[0];
        while (position < middleIndex)
        {
            insertInLeave(subNode, getNode(node)->values[position]);
            position++;
        }
        getNode(subNode)->nodes[position] = getNode(newNode)->nodes[1];

        position = middleIndex;
        subNode = getNode(newNode)->nodes[1];
        while (position <= maxOrder)
        {
            insertInLeave(subNode, getNode(node)->values[position]);
            position++;
        }
    }
    else
    {
        int leftNode = getNode(newNode)->nodes[0];

        int rightNode = getNode(newNode)->nodes[1];

        while (position < middleIndex)
        {
            getNode(leftNode)->values[position] = getNode(node)->values[position];
            getNode(leftNode)->nodes[position] = getNode(node)->nodes[position];
            position++;
        }
        getNode(leftNode)->nodes[position] = getNode(node)->nodes[position];
        setNumKeys(getNode(leftNode), middleIndex);

        position = 0;
        while (position <= maxOrder)
        {
            getNode(rightNode)->values[position] = getNode(node)->values[position + middleIndex + 1];
            getNode(rightNode)->nodes[position] = getNode(node)->nodes[position + middleIndex + 1];
            position++;
        }
        getNode(rightNode)->nodes[position] = getNode(node)->nodes[position + middleIndex + 1];
        setNumKeys(getNode(rightNode), middleIndex);
    }

    root_address = newNode;
}

// Promotes middle element and subdivide his elements
void promoteMiddleElement(int node, int parent, int height)
{
    if (isRoot(parent))
    {
        keepOnlyMiddleElementInRoot(node);
    }
    else if (isLeave(height))
    {
        int middleIndex = (int)(numKeys(getNode(node)) / 2);

        splitLeave(parent, getNode(node)->values[middleIndex], height);
    }
    else
    {
        int middleIndex = (int)(numKeys(getNode(node)) / 2);

        splitNonLeave(parent, getNode(node)->values[middleIndex], height);
    }
}

// Main recursive insertion method
void insertInNode(int node, int key, int value, int parent, int height)
{
    if (isLeave(height))
    {
        insertInLeave(node, key);

        if (haveOverflow(getNode(node)))
        {
            promoteMiddleElement(node, parent, height);
        }
    }
    else
    {
        int position = findKeyInsertPosition(node, key);
        insertInNode(getNode(node)->nodes[position], key, value, node, height + 1);

        if (haveOverflow(getNode(node)))
        {
            promoteMiddleElement(node, parent, height);
        }
    }
}

// Process user key insert
void commandInsert(int key, int value)
{
    if (!keyIsOnTree(root_address, key, 1))
    {
        insertInNode(root_address, key, value, 0, 1);
    }
}

// print the values as height sets to debug
void printNode(int node, int height)
{
    int position = 0;
    int numValues = numKeys(getNode(node));

    printf("Keys - height: %d\n", height);
    while (position < numValues)
    {
        printf("%d ", getNode(node)->values[position]);
        position++;
    }
    if (position > 0)
    {
        printf("\n");
    }

    if (numValues > 0)
    {
        position = 0;

        if (numKeys(getNode(getNode(node)->nodes[0])) > 0)
        {
            while (position <= numValues)
            {
                if (numKeys(getNode(getNode(node)->nodes[position])) > 0)
                {
                    printNode(getNode(node)->nodes[position], height + 1);
                }
                position++;
            }

            if (position > 0)
            {
                printf("\n");
            }
        }
    }
}

// print the values as a ordered list
void printOrdered(int node, int height)
{
    int size = numKeys(getNode(node));
    int position = 0;

    if (size > 0)
    {
        if (isLeave(height))
        {
            while (position < size)
            {
                if (getNode(node)->values[position] != (int)NULL)
                {
                    printf("%d ", getNode(node)->values[position]);
                }
                position++;
            }
            printOrdered(getNode(node)->nodes[size], height);
        }
        else
        {
            printOrdered(getNode(node)->nodes[position], height + 1);
        }
    }
}

// Process user key print
void commandPrint()
{
    if (flagDebug)
    {
        printNode(root_address, 1);
    }
    printf("\n");
    printOrdered(root_address, 1);
    printf("\n");
}

// Allocate through malloc the needed resources
void createTree()
{
    createNode();
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
            commandInsert(key, 1);
        }
        else if (strcmp("p", operation) == 0)
        {
            commandPrint();
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

    writeAll();

    return 0;
}
