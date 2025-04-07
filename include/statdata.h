#ifndef STATDATA_H
#define STATDATA_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

typedef struct StatData
{
    long  id;
    int   count;
    float cost;
    unsigned int primary:1;
    unsigned int mode:3;
} StatData;

int StoreDump(const char * filename, StatData * data, size_t count);

int LoadDump(const char * filename, StatData ** data, size_t * count);

#define SWAP(a, b, size) do { \
    void *temp = malloc(size); \
    memcpy(temp, a, size); \
    memcpy(a, b, size); \
    memcpy(b, temp, size); \
    free(temp); \
} while(0)

void QuickSort(void * arr, size_t n, size_t size, int (*cmp)(const void *, const void *));

int JoinDump(StatData * data_1, size_t len_1, StatData * data_2, size_t len_2, StatData ** result, size_t * result_len);

int ProcessData(const char * filename_1, const char * filename_2, const char * filename_result);

void PrintStatDataTable(const StatData * data, size_t count);

#endif // STATDATA_H
