#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include "extmem.h"
#include "BplusTree.h"
using namespace std;

void nest_loop_join(Buffer *buf);
void print_result(int start_address, int n, Buffer *buf);
void sort_merge_join(Buffer *buf);

const int R_root = 132, S_root = 197; // root address of B plus tree index

int main()
{
    Buffer buf; /* A buffer */
    /* Initialize the buffer */
    if (!initBuffer(520, 64, &buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }
    // nest_loop_join(&buf);
    sort_merge_join(&buf);
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
    int property_count = 0; // count result property
    int result_count = 0;   // count result tuple
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
                        if (property_count == 0)
                        {
                            result_blk = (int *)getNewBlockInBuffer(buf);
                        }
                        *(result_blk + property_count) = a;
                        *(result_blk + property_count + 1) = b;
                        *(result_blk + property_count + 2) = d;
                        result_count++;
                        property_count += 3;
                        if (ri == 15 && si == 31) // if it's the last block, store all result
                        {
                            writeBlockToDisk((unsigned char *)result_blk, result_address, buf);
                            freeBlockInBuffer((unsigned char *)result_blk, buf);
                        }
                        else
                        {
                            if (property_count == 15) // if result block is full
                            {
                                *(result_blk + property_count) = result_address + 1; // last four bytes are used to store next block address
                                writeBlockToDisk((unsigned char *)result_blk, result_address, buf);
                                result_address++;
                                freeBlockInBuffer((unsigned char *)result_blk, buf);
                                property_count = 0;
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
    printf("\ncount = %d\n", result_count);
}

void sort_merge_join(Buffer *buf)
{
    valueNode *r_address_blk = NULL, *s_address_blk;
    int *r_int_blk = NULL, *s_int_blk = NULL;
    int result_address = 1000;
    int property_count = 0; // count result property
    int result_count = 0;   // count result tuple
    int b, d;
    int *r_blk = NULL, *s_blk = NULL, *result_blk = NULL;
    leaf_list *r_leaves = NULL, *s_leaves = NULL;
    createTree(R_root, buf);
    r_leaves = commandGetResult();
    writeAll();
    createTree(S_root, buf);
    s_leaves = commandGetResult();
    writeAll();

    int ri = 0, si = 0;
    while (ri < r_leaves->size)
    {
        while (si < s_leaves->size)
        {
            if (r_leaves->leaves[ri].key == s_leaves->leaves[si].key)
            {
                r_address_blk = (valueNode *)readBlockFromDisk(r_leaves->leaves[ri].value_address, buf);
                for (int rii = 0; rii < r_address_blk->size; rii++) // each R block whose R.A = S.C
                {
                    r_int_blk = (int *)readBlockFromDisk(r_address_blk->values[rii], buf);
                    for (int rj = 0; rj < 7; rj++) // each R.A
                    {
                        if (*(r_int_blk + 2 * rj) == r_leaves->leaves[ri].key) // if R.A equals to S.C
                        {
                            b = *(r_int_blk + 2 * rj + 1);

                            s_address_blk = (valueNode *)readBlockFromDisk(s_leaves->leaves[si].value_address, buf);
                            for (int sii = 0; sii < s_address_blk->size; sii++) // each S block whose R.A = S.C
                            {
                                s_int_blk = (int *)readBlockFromDisk(s_address_blk->values[sii], buf);
                                for (int sj = 0; sj < 7; sj++) // each S.C
                                {
                                    if (*(s_int_blk + 2 * sj) == s_leaves->leaves[si].key) // if R.A = S.C
                                    {
                                        d = *(s_int_blk + 2 * sj + 1);
                                        cout << "key: " << s_leaves->leaves[si].key << "value: " << d << endl;

                                        // start to add new tuple to result block
                                        if (property_count == 0)
                                        {
                                            result_blk = (int *)getNewBlockInBuffer(buf);
                                        }
                                        *(result_blk + property_count) = r_leaves->leaves[ri].key;
                                        *(result_blk + property_count + 1) = b;
                                        *(result_blk + property_count + 2) = d;
                                        result_count++;
                                        property_count += 3;
                                        if (property_count == 15) // if result block is full
                                        {
                                            *(result_blk + property_count) = result_address + 1; // last four bytes are used to store next block address
                                            writeBlockToDisk((unsigned char *)result_blk, result_address, buf);
                                            result_address++;
                                            freeBlockInBuffer((unsigned char *)result_blk, buf);
                                            property_count = 0;
                                        }
                                        // finish adding
                                    }
                                }
                                freeBlockInBuffer((unsigned char *)s_int_blk, buf);
                            }
                            freeBlockInBuffer((unsigned char *)s_address_blk, buf);
                        }
                    }
                    freeBlockInBuffer((unsigned char *)r_int_blk, buf);
                }
                freeBlockInBuffer((unsigned char *)r_address_blk, buf);
                ri++;
                si++;
            }
            else if (r_leaves->leaves[ri].key < s_leaves->leaves[si].key)
            {
                ri++;
            }
            else
            {
                si++;
            }
        }
    }
    writeBlockToDisk((unsigned char *)result_blk, result_address, buf);
    freeBlockInBuffer((unsigned char *)result_blk, buf);
    print_result(1000, result_address - 1000, buf);
    printf("\ncount = %d\n", result_count);
}

// key: 20value: 621
// key: 20value: 705
// key: 20value: 916
// key: 20value: 584
// key: 20value: 255
// key: 20value: 413

// key: 40value: 996
// key: 40value: 400
// key: 40value: 271
// key: 40value: 128
// key: 40value: 126
// key: 40value: 292
// key: 40value: 454
// key: 40value: 996
// key: 40value: 400
// key: 40value: 271
// key: 40value: 128
// key: 40value: 126
// key: 40value: 292
// key: 40value: 454