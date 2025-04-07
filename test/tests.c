#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "statdata.h"

StatData case_1_in_a[2] = {
    {.id = 90889, .count = 13, .cost = 3.567, .primary = 0, .mode = 3},
    {.id = 90089, .count = 1, .cost = 88.90, .primary = 1, .mode = 0}
};

StatData case_1_in_b[2] = {
    {.id = 90089, .count = 2, .cost = 10.10, .primary = 1, .mode = 2},
    {.id = 90889, .count = 10, .cost = 5.55, .primary = 0, .mode = 1}
};

StatData expected_result_case_1[2] = {
    {.id = 0x16335, .count = 23, .cost = 9.120, .primary = 0, .mode = 3},
    {.id = 0x16331, .count = 3, .cost = 99.000, .primary = 1, .mode = 2}  
};

StatData case_2_in_a[3] = {
    {.id = 12345, .count = 5, .cost = 1.23, .primary = 0, .mode = 1},
    {.id = 67890, .count = 10, .cost = 2.34, .primary = 1, .mode = 3},
    {.id = 11111, .count = 20, .cost = 3.45, .primary = 0, .mode = 2}
};

StatData case_2_in_b[3] = {
    {.id = 67890, .count = 2, .cost = 5.67, .primary = 0, .mode = 2},
    {.id = 11111, .count = 10, .cost = 8.90, .primary = 1, .mode = 1},
    {.id = 12345, .count = 15, .cost = 7.89, .primary = 1, .mode = 0}
};

StatData expected_result_case_2[3] = {
    {.id = 12345, .count = 20, .cost = 9.120, .primary = 1, .mode = 1},
    {.id = 67890, .count = 12, .cost = 8.010, .primary = 1, .mode = 3},
    {.id = 11111, .count = 30, .cost = 12.350, .primary = 1, .mode = 2}
};

StatData case_3_in_a[3] = {
    {.id = 43690, .count = 0, .cost = 0.0, .primary = 0, .mode = 0},
    {.id = 48059, .count = 100, .cost = 1000.0, .primary = 1, .mode = 1},
    {.id = 52428, .count = 50, .cost = 500.0, .primary = 1, .mode = 2}
};

StatData case_3_in_b[3] = {
    {.id = 43690, .count = 1, .cost = 1.0, .primary = 1, .mode = 1},
    {.id = 48059, .count = 0, .cost = 0.0, .primary = 0, .mode = 3},
    {.id = 56797, .count = 5, .cost = 5.0, .primary = 1, .mode = 3}
};

StatData expected_result_case_3[4] = {
    {.id = 0xAAAA, .count = 1, .cost = 1.0, .primary = 0, .mode = 1},
    {.id = 0xBBBB, .count = 100, .cost = 1000.0, .primary = 0, .mode = 3},
    {.id = 0xCCCC, .count = 50, .cost = 500.0, .primary = 1, .mode = 2},
    {.id = 0xDDDD, .count = 5, .cost = 5.0, .primary = 1, .mode = 3}
};

StatData case_4_in_a[2] = {
    {.id = 39321, .count = 1, .cost = 0.0001, .primary = 1, .mode = 1},
    {.id = 34952, .count = 2, .cost = 0.0002, .primary = 1, .mode = 2}
};

StatData case_4_in_b[2] = {
    {.id = 39321, .count = 3, .cost = 0.0003, .primary = 1, .mode = 2},
    {.id = 34952, .count = 4, .cost = 0.0004, .primary = 0, .mode = 3}
};

StatData expected_result_case_4[2] = {
    {.id = 0x8888, .count = 6, .cost = 0.0006, .primary = 0, .mode = 3},
    {.id = 0x9999, .count = 4, .cost = 0.0004, .primary = 1, .mode = 2}
};

StatData case_5_in_a[3] = {
    {.id = 4660, .count = 1, .cost = 1.0, .primary = 0, .mode = 1},
    {.id = 4660, .count = 2, .cost = 2.0, .primary = 1, .mode = 3},
    {.id = 4660, .count = 3, .cost = 3.0, .primary = 1, .mode = 0}
};

StatData case_5_in_b[2] = {
    {.id = 4660, .count = 4, .cost = 4.0, .primary = 1, .mode = 2},
    {.id = 4660, .count = 5, .cost = 5.0, .primary = 0, .mode = 1}
};

StatData expected_result_case_5[1] = {
    {.id = 0x1234, .count = 15, .cost = 15.0, .primary = 0, .mode = 3}
};


void RunTest(const char * TestName, StatData * Input_a, size_t Count_a,
             StatData * Input_b, size_t Count_b, StatData * ExpectedResult, size_t CountResult)
{
    const char * InputFile_1 = "input_1.bin";
    const char * InputFile_2 = "input_2.bin";
    const char * OutputFile = "output.bin";

    StoreDump(InputFile_1, Input_a, Count_a);
    StoreDump(InputFile_2, Input_b, Count_b);
    
    if (0 != ProcessData(InputFile_1, InputFile_2, OutputFile))
    {
        fprintf(stderr, "Error processing data for test: %s\n", TestName);
        return;
    }
    
    StatData * OutputData;
    size_t OutputSize = 0;
    if (0 != LoadDump(OutputFile, &OutputData, &OutputSize))
    {
        fprintf(stderr, "Error load data (LoadDump) for test: %s\n", TestName);
        return;
    }

    if (OutputSize != CountResult)
    {
        fprintf(stderr, "Error: Incorrect number of records in output file for test: %s (OutputSize %lu, CountResult %lu)\n", TestName, OutputSize, CountResult);
        free(OutputData);
        return;
    }

    if (!memcmp(OutputData, ExpectedResult, CountResult * sizeof(StatData)))
    {
        fprintf(stderr, "Error: The data in the output file does not match what is expected for the test: %s\n", TestName);
        free(OutputData);
        return;
    }

    printf("Test \"%s\" passed successfully!\n\n", TestName);
    free(OutputData);
}

int main()
{
    RunTest("Test 1", case_1_in_a, sizeof(case_1_in_a) / sizeof(StatData),
             case_1_in_b, sizeof(case_1_in_b) / sizeof(StatData), expected_result_case_1, 2);

    RunTest("Test 2", case_2_in_a, sizeof(case_2_in_a) / sizeof(StatData),
             case_2_in_b, sizeof(case_2_in_b) / sizeof(StatData), expected_result_case_2, 3);

    RunTest("Test 3", case_3_in_a, sizeof(case_3_in_a) / sizeof(StatData),
             case_3_in_b, sizeof(case_3_in_b) / sizeof(StatData), expected_result_case_3, 4);

    RunTest("Test 4", case_4_in_a, sizeof(case_4_in_a) / sizeof(StatData),
             case_4_in_b, sizeof(case_4_in_b) / sizeof(StatData), expected_result_case_4, 2);

    RunTest("Test 5", case_5_in_a, sizeof(case_5_in_a) / sizeof(StatData),
             case_5_in_b, sizeof(case_5_in_b) / sizeof(StatData), expected_result_case_5, 1);

    return EXIT_SUCCESS;
}
