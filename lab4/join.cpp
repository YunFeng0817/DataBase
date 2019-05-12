#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string.h>
#include "extmem.h"
#include "BplusTree.h"
using namespace std;

void create_hash(Buffer *buf);
void nest_loop_join(Buffer *buf);
void print_result(int start_address, int n, Buffer *buf);
void sort_merge_join(Buffer *buf);
void hash_join(Buffer *buf);

const int R_root = 132, S_root = 197; // root address of B plus tree index
const int HASH_NUM = 6;               // amount of hash bucket
const bool DebugFlag = false;         // if run in debug mode
int r_hash_start_address = 500, s_hash_start_address = 750;
int r_buckets_num[HASH_NUM], s_buckets_num[HASH_NUM];

int main()
{
    Buffer buf; /* A buffer */
    /* Initialize the buffer */
    if (!initBuffer(520, 64, &buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }
    cout << "nest loop join result:" << endl;
    nest_loop_join(&buf);
    freeBuffer(&buf);
    /* Initialize the buffer */
    if (!initBuffer(520, 64, &buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }
    cout << "sort merge join result:" << endl;
    sort_merge_join(&buf);
    freeBuffer(&buf);
    /* Initialize the buffer */
    if (!initBuffer(520, 64, &buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }
    cout << "hash join result:" << endl;
    hash_join(&buf);
    freeBuffer(&buf);
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
    if (DebugFlag)
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
    cout << "IO num: " << buf->numIO << endl;
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
    cout << "count = " << result_count << endl
         << endl;
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
    cout << "count = " << result_count << endl
         << endl;
}

void create_hash(Buffer *buf)
{
    unsigned char *blk; // A pointer to a block
    int *blk_int = NULL;
    int block_index = 0;
    int disk_address = 1;                                       // data block address
    int **hash_blks = (int **)malloc(sizeof(int *) * HASH_NUM); // store hash bucket buffer block
    int blk_num[HASH_NUM];                                      // store tuple amount of each bucket

    // init variable for R relation
    for (int i = 0; i < HASH_NUM; i++)
    {
        blk_num[i] = 0;
        r_buckets_num[i] = 0;
    }
    memset(hash_blks, 0, sizeof(int *) * HASH_NUM);
    // generate hash bucket for R relation
    for (block_index = 0; block_index < 16; block_index++)
    {
        /* Read the block from the hard disk */
        blk = readBlockFromDisk(disk_address, buf);
        blk_int = (int *)blk;
        /* print one tuple in the block */
        for (int i = 0; i < 7; i++)
        {
            int index = *(blk_int + 2 * i) % HASH_NUM;
            if (blk_num[index] == 0)
            {
                hash_blks[index] = (int *)getNewBlockInBuffer(buf);
                memset(hash_blks[index], 0, buf->blkSize);
                hash_blks[index][15] = r_hash_start_address + index + r_buckets_num[index] * HASH_NUM;
                r_buckets_num[index]++;
            }
            hash_blks[index][2 * blk_num[index]] = *(blk_int + 2 * i);
            hash_blks[index][2 * blk_num[index] + 1] = *(blk_int + 2 * i + 1);
            blk_num[index]++;

            // if bucket is full
            if (blk_num[index] == 7)
            {
                hash_blks[index][14] = 7;
                writeBlockToDisk((unsigned char *)hash_blks[index], hash_blks[index][15], buf);
                blk_num[index] = 0;
                freeBlockInBuffer((unsigned char *)hash_blks[index], buf);
                hash_blks[index] = NULL;
            }
        }
        disk_address += 1;
        // free block used in buffer
        freeBlockInBuffer(blk, buf);
    }

    // write buffer to files and init variable for S relation
    for (int i = 0; i < HASH_NUM; i++)
    {
        if (blk_num[i] != 0)
        {
            hash_blks[i][14] = blk_num[i];

            writeBlockToDisk((unsigned char *)hash_blks[i], hash_blks[i][15], buf);
            freeBlockInBuffer((unsigned char *)hash_blks[i], buf);
            hash_blks[i] = NULL;
        }
        blk_num[i] = 0;
        s_buckets_num[i] = 0;
    }

    memset(hash_blks, 0, sizeof(int *) * HASH_NUM);

    // generate hash bucket for R relation
    for (block_index = 0; block_index < 32; block_index++)
    {
        /* Read the block from the hard disk */
        blk = readBlockFromDisk(disk_address, buf);
        blk_int = (int *)blk;
        /* print one tuple in the block */
        for (int i = 0; i < 7; i++)
        {
            int index = *(blk_int + 2 * i) % HASH_NUM;
            if (blk_num[index] == 0)
            {
                hash_blks[index] = (int *)getNewBlockInBuffer(buf);
                memset(hash_blks[index], 0, buf->blkSize);
                hash_blks[index][15] = s_hash_start_address + index + s_buckets_num[index] * HASH_NUM;
                s_buckets_num[index]++;
            }
            hash_blks[index][2 * blk_num[index]] = *(blk_int + 2 * i);
            hash_blks[index][2 * blk_num[index] + 1] = *(blk_int + 2 * i + 1);
            blk_num[index]++;
            if (blk_num[index] == 7)
            {
                hash_blks[index][14] = 7;
                writeBlockToDisk((unsigned char *)hash_blks[index], hash_blks[index][15], buf);
                blk_num[index] = 0;
                freeBlockInBuffer((unsigned char *)hash_blks[index], buf);
                hash_blks[index] = NULL;
            }
        }
        disk_address += 1;
        // free block used in buffer
        freeBlockInBuffer(blk, buf);
    }
    // write buffer to files
    for (int i = 0; i < HASH_NUM; i++)
    {
        if (blk_num[i] != 0)
        {
            hash_blks[i][14] = blk_num[i];
            writeBlockToDisk((unsigned char *)hash_blks[i], hash_blks[i][15], buf);
            blk_num[i] = 0;
            freeBlockInBuffer((unsigned char *)hash_blks[i], buf);
            hash_blks[i] = NULL;
        }
    }
    free(hash_blks);
}

void hash_join(Buffer *buf)
{
    int result_address = 1000;
    int property_count = 0; // count result property
    int result_count = 0;   // count result tuple
    int bucket_index = 0;
    int a, b, c, d;
    int *r_blk = NULL, *s_blk = NULL, *result_blk = NULL;
    create_hash(buf);
    for (bucket_index = 0; bucket_index < HASH_NUM; bucket_index++)
    {
        int r_bucket_index = 0;
        while (r_bucket_index < r_buckets_num[bucket_index])
        {
            r_blk = (int *)readBlockFromDisk(r_hash_start_address + bucket_index + r_bucket_index * HASH_NUM, buf);
            int r_bucket_size = r_blk[14];
            for (int rj = 0; rj < r_bucket_size; rj++)
            {
                a = *(r_blk + 2 * rj);
                int s_bucket_index = 0;
                while (s_bucket_index < s_buckets_num[bucket_index])
                {
                    s_blk = (int *)readBlockFromDisk(s_hash_start_address + bucket_index + s_bucket_index * HASH_NUM, buf);
                    int s_bucket_size = s_blk[14];
                    for (int sj = 0; sj < s_bucket_size; sj++)
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
                    freeBlockInBuffer((unsigned char *)s_blk, buf);
                    s_bucket_index++;
                }
            }
            freeBlockInBuffer((unsigned char *)r_blk, buf);
            r_bucket_index++;
        }
    }
    writeBlockToDisk((unsigned char *)result_blk, result_address, buf);
    freeBlockInBuffer((unsigned char *)result_blk, buf);

    print_result(1000, result_address - 1000, buf);
    cout << "count = " << result_count << endl
         << endl;
}