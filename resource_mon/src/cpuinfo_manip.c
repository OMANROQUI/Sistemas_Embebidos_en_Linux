#include "cpuinfo_manip.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int read_cpu_info(CPUInfo *info) {
    FILE *f = fopen("/proc/cpuinfo", "r");
    if (!f) return -1;

    info->cores = 0;
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "model name", 10) == 0) {
            char *p = strchr(line, ':');
            if (p) {
                strncpy(info->model_name, p + 2, sizeof(info->model_name) - 1);
                info->model_name[sizeof(info->model_name) - 1] = '\0';
                info->model_name[strcspn(info->model_name, "\n")] = '\0';
            }
        } else if (strncmp(line, "processor", 9) == 0) {
            info->cores++;
        }
    }

    fclose(f);
    return 0;
}

int read_cpu_stat(CPUStat *stat) {
    FILE *f = fopen("/proc/stat", "r");
    if (!f) return -1;

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

int get_cpu_count(void) {
    CPUInfo info;
    if (read_cpu_info(&info) != 0) return 0;
    return info.cores;
}

void get_cpu_usage(float *percentages) {
    int n = get_cpu_count();
    if (n <= 0) return;

    CPUStat *prev = malloc(sizeof(CPUStat) * n);
    CPUStat *curr = malloc(sizeof(CPUStat) * n);
    char label[16];
    int i;
    FILE *f;

    // Primera instantánea
    f = fopen("/proc/stat", "r");
    if (!f) {
        free(prev);
        free(curr);
        return;
    }
    for (i = 0; i < n; i++) {
        rewind(f);
        while (fscanf(f, "%15s %lu %lu %lu %lu %lu %lu %lu %lu",
                      label,
                      &prev[i].user,
                      &prev[i].nice,
                      &prev[i].system,
                      &prev[i].idle,
                      &prev[i].iowait,
                      &prev[i].irq,
                      &prev[i].softirq,
                      &prev[i].steal) == 9) {
            if (strncmp(label, "cpu", 3) == 0 && atoi(label + 3) == i)
                break;
        }
    }
    fclose(f);

    sleep(1);

    // Segunda instantánea y cálculo
    f = fopen("/proc/stat", "r");
    if (!f) {
        free(prev);
        free(curr);
        return;
    }
    for (i = 0; i < n; i++) {
        rewind(f);
        while (fscanf(f, "%15s %lu %lu %lu %lu %lu %lu %lu %lu",
                      label,
                      &curr[i].user,
                      &curr[i].nice,
                      &curr[i].system,
                      &curr[i].idle,
                      &curr[i].iowait,
                      &curr[i].irq,
                      &curr[i].softirq,
                      &curr[i].steal) == 9) {
            if (strncmp(label, "cpu", 3) == 0 && atoi(label + 3) == i)
                break;
        }
        percentages[i] = (float)calculate_cpu_usage(&prev[i], &curr[i]);
    }
    fclose(f);

    free(prev);
    free(curr);
}
