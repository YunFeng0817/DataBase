/*
 * select.c
 * select data according to specific condition in database
 */

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include "extmem.h"
#include "BplusTree.h"
using namespace std;

void B_plus_tree_select(Buffer *buf, int *R_result, int *S_result);
void linear_select(Buffer *buf, int *R_result, int *S_result);
void print_result(Buffer *buf, int *R_result, int *S_result);

const int RA_VALUE = 40, SC_VALUE = 60;
const int R_root = 132, S_root = 197;

int main(int argc, char **argv)
{

    Buffer buf; /* A buffer */
    int *S_result = NULL, *R_result = NULL;
    /* Initialize the buffer */
    if (!initBuffer(520, 64, &buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }
    R_result = (int *)getNewBlockInBuffer(&buf);
    S_result = (int *)getNewBlockInBuffer(&buf);
    // linear_select(&buf, R_result, S_result);
    B_plus_tree_select(&buf, R_result, S_result);

    /* Write the block to the hard disk */
    if (writeBlockToDisk((unsigned char *)R_result, 49, &buf) != 0)
    {
        perror("Writing Block Failed!\n");
        return -1;
    }
    /* Write the block to the hard disk */
    if (writeBlockToDisk((unsigned char *)S_result, 50, &buf) != 0)
    {
        perror("Writing Block Failed!\n");
        return -1;
    }
    return 0;
}

void linear_select(Buffer *buf, int *R_result, int *S_result)
{
    int *blk = NULL; /* A pointer to a block */
    int i = 0, block_index = 0, r_result_index = 0, s_result_index = 0;
    int disk_address = 1;

    for (block_index = 0; block_index < 16; block_index++)
    {
        /* Read the block from the hard disk */
        if ((blk = (int *)readBlockFromDisk(disk_address, buf)) == NULL)
        {
            perror("Reading Block Failed!\n");
            return;
        }
        for (i = 0; i < 7; i++)
        {
            if (*(blk + 2 * i) == RA_VALUE)
            {
                *(R_result + 2 * r_result_index) = RA_VALUE;
                *(R_result + 2 * r_result_index + 1) = *(blk + 2 * i + 1);
                r_result_index += 1;
            }
        }
        disk_address += 1;
        // free block used in buffer
        freeBlockInBuffer((unsigned char *)blk, buf);
    }
    *(R_result + 2 * (buf->blkSize / 8 - 1)) = r_result_index;

    for (block_index = 0; block_index < 32; block_index++)
    {
        /* Read the block from the hard disk */
        if ((blk = (int *)readBlockFromDisk(disk_address, buf)) == NULL)
        {
            perror("Reading Block Failed!\n");
            return;
        }
        for (i = 0; i < 7; i++)
        {
            if (*(blk + 2 * i) == SC_VALUE)
            {
                *(S_result + 2 * s_result_index) = SC_VALUE;
                *(S_result + 2 * s_result_index + 1) = *(blk + 2 * i + 1);
                s_result_index += 1;
            }
        }
        disk_address += 1;
        // free block used in buffer
        freeBlockInBuffer((unsigned char *)blk, buf);
    }
    printf("s index: %d\n", s_result_index);
    *(S_result + 2 * (buf->blkSize / 8 - 1)) = s_result_index;
    print_result(buf, R_result, S_result);
}

void binary_select(Buffer *buf, int *R_result, int *S_result)
{
    valueNode *blk = NULL; /* A pointer to a block */
    int *int_blk = NULL;
    freeBlockInBuffer((unsigned char *)R_result, buf);
    freeBlockInBuffer((unsigned char *)S_result, buf);
    createTree(R_root, buf);
}

void B_plus_tree_select(Buffer *buf, int *R_result, int *S_result)
{
    valueNode *blk = NULL; /* A pointer to a block */
    int *int_blk = NULL;
    freeBlockInBuffer((unsigned char *)R_result, buf);
    freeBlockInBuffer((unsigned char *)S_result, buf);
    createTree(R_root, buf);
    int value_node = commandSearch(RA_VALUE);
    writeAll();
    /* Read the block from the hard disk */
    if ((blk = (valueNode *)readBlockFromDisk(value_node, buf)) == NULL)
    {
        perror("Reading Block Failed!\n");
        return;
    };

    int length = blk->size;
    printf("R result:\n");
    printf("A\tB\n");
    for (int i = 0; i < length; i++)
    {
        if ((int_blk = (int *)readBlockFromDisk(blk->values[i], buf)) == NULL)
        {
            perror("Reading Block Failed!\n");
            return;
        }
        for (int j = 0; j < 7; j++)
        {
            if (*(int_blk + 2 * j) == RA_VALUE)
            {
                printf("%d\t", *(int_blk + 2 * j));
                printf("%d\n", *(int_blk + 2 * j + 1));
            }
        }
        freeBlockInBuffer((unsigned char *)int_blk, buf);
    }
    freeBlockInBuffer((unsigned char *)blk, buf);
    createTree(S_root, buf);
    writeAll();
    value_node = commandSearch(SC_VALUE);
    /* Read the block from the hard disk */
    if ((blk = (valueNode *)readBlockFromDisk(value_node, buf)) == NULL)
    {
        perror("Reading Block Failed!\n");
        return;
    }
    length = blk->size;
    printf("\nS result:\n");
    printf("C\tD\n");
    for (int i = 0; i < length; i++)
    {
        if ((int_blk = (int *)readBlockFromDisk(blk->values[i], buf)) == NULL)
        {
            perror("Reading Block Failed!\n");
            return;
        }
        for (int j = 0; j < 7; j++)
        {
            if (*(int_blk + 2 * j) == SC_VALUE)
            {
                printf("%d\t", *(int_blk + 2 * j));
                printf("%d\n", *(int_blk + 2 * j + 1));
            }
        }
        freeBlockInBuffer((unsigned char *)int_blk, buf);
    }
    writeAll();
    printf("IO num: %d", buf->numIO);
}

void print_result(Buffer *buf, int *R_result, int *S_result)
{
    int i = 0, block_index = 0, r_result_index = 0, s_result_index = 0;
    r_result_index = *(R_result + 2 * (buf->blkSize / 8 - 1));
    printf("R result:\n");
    printf("A\tB\n");
    for (i = 0; i < r_result_index; i++)
    {
        printf("%d\t", *(R_result + 2 * i));
        printf("%d\n", *(R_result + 2 * i + 1));
    }

    s_result_index = *(S_result + 2 * (buf->blkSize / 8 - 1));
    printf("\nS result:\n");
    printf("C\tD\n");
    for (i = 0; i < s_result_index; i++)
    {
        printf("%d\t", *(S_result + 2 * i));
        printf("%d\n", *(S_result + 2 * i + 1));
    }
    printf("IO num: %d", buf->numIO);
}

// 40      503
// 40      314

// 60      587
// 60      889
// 60      735
// 60      879
// 60      221