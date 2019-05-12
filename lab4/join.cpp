#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include "extmem.h"
#include "BplusTree.h"
using namespace std;

void nest_loop_join(Buffer *buf);
void print_result(int start_address, int n, Buffer *buf);

int main()
{
    Buffer buf; /* A buffer */
    /* Initialize the buffer */
    if (!initBuffer(520, 64, &buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }
    nest_loop_join(&buf);
    return 0;
}

/** 
* print result data
* \param int start_address  start block address
* \param int n          number of blocks
* \param Buffer *buf
* \return void
*/

void print_result(int start_address, int n, Buffer *buf)
{
    int *blk;
    cout << "Join result: " << endl;
    cout << "RA/SC\tRB\tSD" << endl;
    for (int i = start_address; i < start_address + n; i++)
    {
        blk = (int *)readBlockFromDisk(i, buf);

        for (int j = 0; j < 15; j = j + 3)
        {
            cout << *(blk + j) << "\t" << *(blk + j + 1) << "\t" << *(blk + j + 2) << endl;
        }
        freeBlockInBuffer((unsigned char *)blk, buf);
    }
}

void nest_loop_join(Buffer *buf)
{
    int result_address = 1000;
    int result_count = 0; // count result tuple
    int count = 0;        // count result block
    int a, b, c, d;
    int *r_blk = NULL, *s_blk = NULL, *result_blk = NULL;
    for (int ri = 1; ri <= 16; ri++)
    {
        r_blk = (int *)readBlockFromDisk(ri, buf); // each block in R relationship
        for (int rj = 0; rj < 7; rj++)
        {
            a = *(r_blk + 2 * rj);
            for (int si = 1; si <= 32; si++)
            {
                s_blk = (int *)readBlockFromDisk(si + 16, buf); // each block in S relationship
                for (int sj = 0; sj < 7; sj++)
                {
                    c = *(s_blk + 2 * sj);
                    if (a == c)
                    {
                        b = *(r_blk + 2 * rj + 1);
                        d = *(s_blk + 2 * sj + 1);
                        if (result_count == 0)
                        {
                            result_blk = (int *)getNewBlockInBuffer(buf);
                        }
                        *(result_blk + result_count) = a;
                        *(result_blk + result_count + 1) = b;
                        *(result_blk + result_count + 2) = d;
                        count++;
                        result_count += 3;
                        if (ri == 15 && si == 31) // if it's the last block, store all result
                        {
                            writeBlockToDisk((unsigned char *)result_blk, result_address, buf);
                            freeBlockInBuffer((unsigned char *)result_blk, buf);
                        }
                        else
                        {
                            if (result_count == 15) // if result block is full
                            {
                                *(result_blk + result_count) = result_address + 1; // last four bytes are used to store next block address
                                writeBlockToDisk((unsigned char *)result_blk, result_address, buf);
                                result_address++;
                                freeBlockInBuffer((unsigned char *)result_blk, buf);
                                result_count = 0;
                            }
                        }
                    }
                }
                freeBlockInBuffer((unsigned char *)s_blk, buf);
            }
        }
        freeBlockInBuffer((unsigned char *)r_blk, buf);
    }
    print_result(1000, result_address - 1000, buf);
    printf("\ncount = %d\n", count);
}