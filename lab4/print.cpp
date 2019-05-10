/*
 * print.c
 * print all data in database
 */
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <set>
#include <queue>
#include "extmem.h"

int main(int argc, char **argv)
{

    Buffer buf;         /* A buffer */
    unsigned char *blk; /* A pointer to a block */
    int *blk_int = NULL;
    int i = 0, block_index = 0;
    int disk_address = 1;

    /* Initialize the buffer */
    if (!initBuffer(520, 64, &buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }

    printf("Relation R:\n");
    printf("A\t B\n");
    for (block_index = 0; block_index < 16; block_index++)
    {
        /* Read the block from the hard disk */
        if ((blk = readBlockFromDisk(disk_address, &buf)) == NULL)
        {
            perror("Reading Block Failed!\n");
            return -1;
        }
        blk_int = (int *)blk;
        /* print one tuple in the block */
        for (i = 0; i < 7; i++)
        {
            printf("%d\t", *(blk_int + 2 * i));
            printf("%d\n", *(blk_int + 2 * i + 1));
        }
        disk_address += 1;
        // free block used in buffer
        freeBlockInBuffer(blk, &buf);
    }

    printf("\nRelation S:\n");
    printf("C\t D\n");
    for (block_index = 0; block_index < 32; block_index++)
    {
        /* Read the block from the hard disk */
        if ((blk = readBlockFromDisk(disk_address, &buf)) == NULL)
        {
            perror("Reading Block Failed!\n");
            return -1;
        }
        blk_int = (int *)blk;
        /* print one tuple in the block */
        for (i = 0; i < 7; i++)
        {
            printf("%d\t", *(blk_int + 2 * i));
            printf("%d\n", *(blk_int + 2 * i + 1));
        }
        disk_address += 1;
        // free block used in buffer
        freeBlockInBuffer(blk, &buf);
    }

    return 0;
}