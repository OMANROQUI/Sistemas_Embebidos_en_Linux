#ifndef CPUINFO_MANIP_H
#define CPUINFO_MANIP_H

#include <stdint.h>

// Estructura para datos básicos de CPU
typedef struct {
    char model_name[128];   // línea "model name" de /proc/cpuinfo
    uint32_t cores;         // número de núcleos (contar "processor")
} CPUInfo;

// Estructura para lecturas de /proc/stat
typedef struct {
    uint64_t user;
    uint64_t nice;
    uint64_t system;
    uint64_t idle;
    uint64_t iowait;
    uint64_t irq;
    uint64_t softirq;
    uint64_t steal;
} CPUStat;

/**
 * Lee /proc/cpuinfo y rellena un CPUInfo.
 * Devuelve 0 si tuvo éxito o -1 si hubo error.
 */
int read_cpu_info(CPUInfo *info);

/**
 * Lee /proc/stat y rellena un CPUStat con los primeros valores de la línea "cpu ".
 * Devuelve 0 si tuvo éxito o -1 si hubo error.
 */
int read_cpu_stat(CPUStat *stat);

/**
 * Calcula el % de uso de CPU entre dos lecturas:
 *   usage = 100 * (delta_total - delta_idle) / delta_total
 */
double calculate_cpu_usage(const CPUStat *prev, const CPUStat *curr);

#endif // CPUINFO_MANIP_H
