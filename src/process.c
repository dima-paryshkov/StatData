#include "statdata.h"
#include <string.h>

#define SWAP(a, b, size) do { \
    void *temp = malloc(size); \
    memcpy(temp, a, size); \
    memcpy(a, b, size); \
    memcpy(b, temp, size); \
    free(temp); \
} while(0)

void QuickSort(void * arr, size_t n, size_t size, int (*cmp)(const void *, const void *))
{
    if (n <= 1 || !arr || !cmp) return;

    void * stack[n];
    int top = -1;

    stack[++top] = arr;
    stack[++top] = arr + (n - 1) * size;

    while (top >= 0)
    {
        void * high = stack[top--];
        void * low = stack[top--];

        void * pivot = low;
        void * i = low;
        void * j = high;

        while (1)
        {
            while (i <= high && cmp(i, pivot) <= 0)
                i += size;
            while (j > low && cmp(j, pivot) > 0)
                j -= size;

            if (i >= j) break;

            SWAP(i, j, size);
        }

        SWAP(low, j, size);

        if ((size_t)(j - low) > size)
        {
            stack[++top] = low;
            stack[++top] = j - size;
        }
        if ((size_t)(high - j) > size)
        {
            stack[++top] = (char *)j + size;
            stack[++top] = high;
        }
    }
}

static int CompareStatdataID(const void * a, const void * b)
{
    return ((StatData *)a)->id - ((StatData *)b)->id;
}

int JoinDump(StatData * Data_1, size_t len_1, StatData * Data_2, size_t len_2, StatData ** result, size_t * result_len)
{
    QuickSort(Data_1, len_1, sizeof(StatData), CompareStatdataID);
    QuickSort(Data_2, len_2, sizeof(StatData), CompareStatdataID);

    StatData * data_res = malloc((len_1 + len_2) * sizeof(StatData));
    if (!data_res)
    {
        fprintf(stderr, "Unable to allocate %lu bytes (%s)\n", (len_1 + len_2) * sizeof(StatData), strerror(errno));
        return -1;
    }

    size_t result_size = 0;
    size_t i = 0, j = 0;

    while (i < len_1 || j < len_2)
    {
        long current_id;

        if (i < len_1 && (j >= len_2 || Data_1[i].id < Data_2[j].id))
            current_id = Data_1[i].id;
        else if (j < len_2 && (i >= len_1 || Data_2[j].id < Data_1[i].id))
            current_id = Data_2[j].id;
        else
            current_id = Data_1[i].id;

        StatData merged = {.id = current_id, .count = 0, .cost = 0.0, .primary = 1, .mode = 0};

        while (i < len_1 && Data_1[i].id == current_id)
        {
            merged.count += Data_1[i].count;
            merged.cost  += Data_1[i].cost;
            if (Data_1[i].primary == 0)
                merged.primary = 0;
            if (Data_1[i].mode > merged.mode)
                merged.mode = Data_1[i].mode;
            i++;
        }


        while (j < len_2 && Data_2[j].id == current_id)
        {
            merged.count += Data_2[j].count;
            merged.cost  += Data_2[j].cost;
            if (Data_2[j].primary == 0)
                merged.primary = 0;
            if (Data_2[j].mode > merged.mode)
                merged.mode = Data_2[j].mode;
            j++;
        }

        data_res[result_size++] = merged;
    }

    data_res = realloc(data_res, result_size * sizeof(StatData));
    if (!data_res)
    {
        fprintf(stderr, "Unable to reallocate (%s)\n", strerror(errno));
        return -1;
    }

    *result = data_res;
    *result_len = result_size;
    return 0;
}

static int CompareStatdataCost(const void * a, const void * b)
{
    return ((StatData *)a)->cost - ((StatData *)b)->cost;
}

int ProcessData(const char * Filename_1, const char * Filename_2, const char * FilenameResult)
{
    if (!Filename_1 || !Filename_2 || !FilenameResult)
    {
        fprintf(stderr, "<%s> invalid arguments %s\n", __func__, Filename_1);
        return -1;
    }

    StatData * Data_1 = NULL;
    StatData * Data_2 = NULL;
    size_t CountData_1 = 0;
    size_t CountData_2 = 0;

    if (LoadDump(Filename_1, &Data_1, &CountData_1) != 0)
    {
        fprintf(stderr, "<%s> LoadDump failed for file %s\n", __func__, Filename_1);
        return -1;
    }

    if (LoadDump(Filename_2, &Data_2, &CountData_2) != 0)
    {
        fprintf(stderr, "<%s> LoadDump failed for file %s\n", __func__, Filename_2);
        free(Data_1);
        return -1;
    }
    
    StatData * DataResult = NULL;
    size_t CountDataResult = 0;
    
    if (JoinDump(Data_1, CountData_1, Data_2, CountData_2, &DataResult, &CountDataResult))
    {
        fprintf(stderr, "<%s> JoinDump failed\n", __func__);
        free(Data_1);
        free(Data_2);
        return -1;
    }
    
    free(Data_1);
    free(Data_2);

    QuickSort(DataResult, CountDataResult, sizeof(StatData), CompareStatdataCost);

    PrintStatDataTable(DataResult, CountDataResult);
    
    if (0 != StoreDump(FilenameResult, DataResult, CountDataResult))
    {
        fprintf(stderr, "<%s> StoreDump failed\n", __func__);
        free(DataResult);
        return -1;
    }
    
    free(DataResult);

    return 0;
}
