#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include "extmem.h"
#include "BplusTree.h"
using namespace std;

const int R_root = 132, S_root = 197;

int main()
{
    Buffer buf; /* A buffer */
    /* Initialize the buffer */
    if (!initBuffer(520, 64, &buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }

    createTree(R_root, &buf);
    leaf_list *leaves;
    leaves = commandGetResult();
    writeAll();
    int size = leaves->size;
    cout << "Projection result: " << endl;
    cout << "A" << endl;
    for (int i = 0; i < size; i++)
    {
        cout << leaves->leaves[i].key << endl;
    }
}