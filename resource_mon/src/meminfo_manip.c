#include <stdio.h>
#include <string.h>
#include "meminfo_manip.h"

int get_meminfo(MemInfo *info) {
    FILE *fp = fopen("/proc/meminfo", "r");
    if (!fp) return -1;

    char label[64];
    unsigned long value;
    char unit[16];

    while (fscanf(fp, "%63s %lu %15s\n", label, &value, unit) == 3) {
        if (strcmp(label, "MemTotal:") == 0)
            info->mem_total_kb = value;
        else if (strcmp(label, "MemFree:") == 0)
            info->mem_free_kb = value;
        else if (strcmp(label, "SwapTotal:") == 0)
            info->swap_total_kb = value;
        else if (strcmp(label, "SwapFree:") == 0)
            info->swap_free_kb = value;
    }

    fclose(fp);
    return 0;
}

float get_mem_usage_percent(const MemInfo *info) {
    if (info->mem_total_kb == 0) return 0.0f;
    return 100.0f * (1 - ((float)info->mem_free_kb / info->mem_total_kb));
}

float get_swap_usage_percent(const MemInfo *info) {
    if (info->swap_total_kb == 0) return 0.0f;
    return 100.0f * (1 - ((float)info->swap_free_kb / info->swap_total_kb));
}
