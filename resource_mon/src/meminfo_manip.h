#ifndef MEMINFO_MANIP_H
#define MEMINFO_MANIP_H

typedef struct {
    unsigned long mem_total_kb;
    unsigned long mem_free_kb;
    unsigned long swap_total_kb;
    unsigned long swap_free_kb;
} MemInfo;

int get_meminfo(MemInfo *info);
float get_mem_usage_percent(const MemInfo *info);
float get_swap_usage_percent(const MemInfo *info);

#endif // MEMINFO_MANIP_H
