#include "cpuinfo_manip.h"
#include <stdio.h>
#include <string.h>

int read_cpu_info(CPUInfo *info) {
    FILE *f = fopen("/proc/cpuinfo", "r");
    if (!f) return -1;

    info->cores = 0;
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "model name", 10) == 0) {
            // formatea: "model name\t: Intel(R) ..."
            char *p = strchr(line, ':');
            if (p) {
                strncpy(info->model_name, p+2, sizeof(info->model_name)-1);
                info->model_name[sizeof(info->model_name)-1] = '\0';
                // quitar el '\n'
                info->model_name[strcspn(info->model_name, "\n")] = '\0';
            }
        }
        else if (strncmp(line, "processor", 9) == 0) {
            info->cores++;
        }
    }
    fclose(f);
    return 0;
}

int read_cpu_stat(CPUStat *stat) {
    FILE *f = fopen("/proc/stat", "r");
    if (!f) return -1;

    // lectura de línea: cpu  3357 0 4313 1362393 ...
    char cpu_label[5];
    int ret = fscanf(f, "%4s %lu %lu %lu %lu %lu %lu %lu %lu",
                     cpu_label,
                     &stat->user,
                     &stat->nice,
                     &stat->system,
                     &stat->idle,
                     &stat->iowait,
                     &stat->irq,
                     &stat->softirq,
                     &stat->steal);
    fclose(f);
    return (ret == 9) ? 0 : -1;
}

double calculate_cpu_usage(const CPUStat *prev, const CPUStat *curr) {
    uint64_t prev_idle = prev->idle + prev->iowait;
    uint64_t curr_idle = curr->idle + curr->iowait;

    uint64_t prev_total = prev->user + prev->nice + prev->system +
                          prev->idle + prev->iowait + prev->irq +
                          prev->softirq + prev->steal;
    uint64_t curr_total = curr->user + curr->nice + curr->system +
                          curr->idle + curr->iowait + curr->irq +
                          curr->softirq + curr->steal;

    uint64_t total_delta = curr_total - prev_total;
    uint64_t idle_delta  = curr_idle  - prev_idle;

    if (total_delta == 0) return 0.0;
    return 100.0 * (double)(total_delta - idle_delta) / (double)total_delta;
}
