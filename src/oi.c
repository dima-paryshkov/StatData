#include <string.h>
#include <unistd.h>
#include "statdata.h"

int StoreDump(const char * filename, StatData * data, size_t count)
{
    if (!filename || !data)
    {
        fprintf(stderr, "<%s> invalid arguments\n", __func__);
        return -1;
    }

    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        fprintf(stderr, "<%s> failed to open file %s (%s)\n", __func__, filename, strerror(errno));
        return -1;
    }

    int WriteBytes = write(fd, data, count * sizeof(StatData));
    if (WriteBytes < 0 || (size_t)WriteBytes < sizeof(StatData) * count )
    {
        fprintf(stderr, "failed to write file %s (%s), writed %d bytes\n", filename, strerror(errno), WriteBytes);
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

int LoadDump(const char * filename, StatData ** data, size_t * count)
{
    if (!filename)
    {
        fprintf(stderr, "<%s> invalid arguments\n", __func__);
        return -1;
    }

    int fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        fprintf(stderr, "<%s> failed to open file %s (%s)\n", __func__, filename, strerror(errno));
        return -1;
    }

    size_t size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    if (size % sizeof(StatData) != 0)
    {
        fprintf(stderr, "Incorrect data in file %s\n", filename);
        close(fd);
        return -1;
    }

    void * result = NULL;
    result = malloc(size);
    if (!result)
    {
        fprintf(stderr, "Unable to allocate resources: %s\n", strerror(errno));
        close(fd);
        return -1;
    }

    int ReadBytes = read(fd, result, size);
    if (ReadBytes < 0 || (size_t)ReadBytes != size)
    {
        fprintf(stderr, "failed to read file %s (%s), readed %d bytes in %lu size\n", filename, strerror(errno), ReadBytes, size);
        free(result);
        close(fd);
        return -1;
    }

    *data = result;
    *count = size / sizeof(StatData);
    close(fd);
    return 0;
}

static inline void PrintBinary(unsigned int val, int bits)
{
    for (int i = bits - 1; i >= 0; i--)
        printf("%c", (val >> i) & 1 ? '1' : '0');
}

void PrintStatDataTable(const StatData * data, size_t count)
{
    if (!data || count == 0)
    {
        printf("No data to display.\n");
        return;
    }

    size_t DisplayCount = count > 10 ? 10 : count;

    printf("%-12s %-10s %-15s %-8s %-6s\n", "ID", "COUNT", "COST", "PRIMARY", "MODE");
    printf("------------------------------------------------------------\n");

    for (size_t i = 0; i < DisplayCount; i++)
    {
        printf("0x%08lx  %-10d %.3e      %-8c ", 
            data[i].id, 
            data[i].count, 
            data[i].cost, 
            data[i].primary ? 'y' : 'n');

        PrintBinary(data[i].mode, 3);
        printf("\n");
    }
}
