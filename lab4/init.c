/*
 * init.c
 * init test data for lab
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "extmem.h"

int generateA();
int generateB();
int generateC();
int generateD();

int main(int argc, char **argv)
{
    srand(time(NULL)); // set random seed

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

    /* Get a new block in the buffer */
    blk = getNewBlockInBuffer(&buf);
    blk_int = (int *)blk;
    // generate data for relation R
    for (block_index = 0; block_index < 16; block_index++)
    {
        /* Fill data into the block */
        for (i = 0; i < 7; i++)
        {
            *(blk_int + 2 * i) = generateA();
            *(blk_int + 2 * i + 1) = generateB();
        }

        *(blk_int + 2 * i) = 0;
        *(blk_int + 2 * i) = disk_address + 1;
        /* Write the block to the hard disk */
        if (writeBlockToDisk(blk, disk_address, &buf) != 0)
        {
            perror("Writing Block Failed!\n");
            return -1;
        }
        disk_address += 1;
    }

    // generate data for relation S
    for (block_index = 0; block_index < 32; block_index++)
    {
        /* Fill data into the block */
        for (i = 0; i < 7; i++)
        {
            *(blk_int + 2 * i) = generateC();
            *(blk_int + 2 * i + 1) = generateD();
        }
        *(blk_int + 2 * i) = 0;
        if (block_index == 31)
            *(blk_int + 2 * i + 1) = 0;
        else
            *(blk_int + 2 * i + 1) = disk_address + 1;
        /* Write the block to the hard disk */
        if (writeBlockToDisk(blk, disk_address, &buf) != 0)
        {
            perror("Writing Block Failed!\n");
            return -1;
        }
        disk_address += 1;
    }

    // /* Read the block from the hard disk */
    // if ((blk = readBlockFromDisk(31415926, &buf)) == NULL)
    // {
    //     perror("Reading Block Failed!\n");
    //     return -1;
    // }

    // /* Process the data in the block */
    // for (i = 0; i < 8; i++)
    //     printf("%c", *(blk + i));

    // printf("\n");
    // printf("# of IO's is %d\n", buf.numIO); /* Check the number of IO's */

    return 0;
}

int generateA()
{
    return rand() % 40 + 1;
}

int generateB()
{
    return rand() % 1000 + 1;
}

int generateC()
{
    return rand() % 41 + 20;
}

int generateD()
{
    return generateB();
}
