#ifndef __CQPG_MEMORY__
#define __CQPG_MEMORY__

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string.h>

// 内存监测 memory[0]峰值内存 memory[1]实时内存
int memory[2];
void CalMem(char const *info)
{
    static const char mem_data[] = "VmPeak:";
    static const char mem_data2[] = "VmSize:";
    FILE *mem_stream;
    char mem_cache[256];
    char mem_info[64];

    sprintf(mem_info, "/proc/%d/status", getpid());
    mem_stream = fopen(mem_info, "r");
    if (mem_stream == NULL) {
        return;
    }

    while(fscanf(mem_stream, "%s", mem_cache) != EOF) {
        if (strncmp(mem_cache, mem_data, sizeof(mem_data)) == 0) {
            if (fscanf(mem_stream, "%s", mem_cache) != EOF) {
                memory[0] = atoi(mem_cache);
            }
        } 
        if (strncmp(mem_cache, mem_data2, sizeof(mem_data2)) == 0) {
            if (fscanf(mem_stream, "%s", mem_cache) != EOF) {
                memory[1] = atoi(mem_cache);
                break;
            }
        }
    }

    static int count = 0;
    static int ini_peak;
    static int ini_size;
    if (count == 0) {
        ini_peak = memory[0];
        ini_size = memory[1];
    }

    std::cout << fixed << "阶段: " << info << ", 峰值内存: " << (memory[0] - ini_peak) * 1. / 1024 << "MB, 实时内存: " << (memory[1] - ini_size) * 1. / 1024 << "MB" << std::endl;

    ++ count;

}

#endif
