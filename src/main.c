#include "statdata.h"

int main(int argc, char * argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Error!\n");
        fprintf(stderr, "Using: %s Input_1.bin Input_2.bin Output.bin\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char * Input_1 = argv[1];
    const char * Input_2 = argv[2];
    const char * Output = argv[3];

    int result = ProcessData(Input_1, Input_2, Output);
    if (result != 0)
    {
        fprintf(stderr, "Error processing data.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
