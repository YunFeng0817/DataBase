#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string.h>
#include "extmem.h"
#include "BplusTree.h"
using namespace std;

int main(int argc, char **argv)
{

    Buffer buffer;      /* A buffer */
    unsigned char *blk; /* A pointer to a block */
    int *blk_int = NULL;
    int i = 0, block_index = 0;
    int disk_address = 1;

    createTree();
    int A_root, B_root;

    /* Initialize the buffer */
    if (!initBuffer(520, 64, &buffer))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }

    for (block_index = 0; block_index < 16; block_index++)
    {
        /* Read the block from the hard disk */
        if ((blk = readBlockFromDisk(disk_address, &buffer)) == NULL)
        {
            perror("Reading Block Failed!\n");
            return -1;
        }
        blk_int = (int *)blk;
        /* print one tuple in the block */
        for (i = 0; i < 7; i++)
        {
            commandInsert(*(blk_int + 2 * i), disk_address);
            // commandInsert(*(blk_int + 2 * i), *(blk_int + 2 * i + 1));
        }
        disk_address += 1;
        // free block used in buffer
        freeBlockInBuffer(blk, &buffer);
    }
    // commandPrint();
    A_root = writeAll();

    createTree();

    for (block_index = 0; block_index < 32; block_index++)
    {
        /* Read the block from the hard disk */
        if ((blk = readBlockFromDisk(disk_address, &buffer)) == NULL)
        {
            perror("test Reading Block Failed!\n");
            return -1;
        }
        blk_int = (int *)blk;
        /* print one tuple in the block */
        for (i = 0; i < 7; i++)
        {
            commandInsert(*(blk_int + 2 * i), disk_address);
            // commandInsert(*(blk_int + 2 * i), *(blk_int + 2 * i + 1));
        }
        disk_address += 1;
        // free block used in buffer
        freeBlockInBuffer(blk, &buffer);
    }
    // commandPrint();
    B_root = writeAll();
    cout << A_root << endl;
    cout << B_root << endl;
    return 0;
}