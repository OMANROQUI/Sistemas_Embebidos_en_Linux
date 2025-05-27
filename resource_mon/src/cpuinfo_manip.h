#ifndef CPUINFO_MANIP_H
#define CPUINFO_MANIP_H

#include <stdint.h>

/**
 * Información básica de CPU obtenida de /proc/cpuinfo.
 */
typedef struct {
    char     model_name[128];   // la línea "model name"
    uint32_t cores;             // núcleos lógicos (líneas "processor")
} CPUInfo;

/**
 * Estadísticas de CPU obtenidas de /proc/stat.
 */
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
 * @return 0 si tuvo éxito, -1 en caso de error.
 */
int read_cpu_info(CPUInfo *info);

/**
 * Lee /proc/stat y rellena un CPUStat con la línea "cpu ".
 * @return 0 si tuvo éxito, -1 en caso de error.
 */
int read_cpu_stat(CPUStat *stat);

/**
 * Calcula el porcentaje de uso de CPU entre dos lecturas:
 *   usage = 100 * (delta_total - delta_idle) / delta_total
 */
double calculate_cpu_usage(const CPUStat *prev, const CPUStat *curr);

/**
 * Devuelve el número de CPUs lógicos detectados en /proc/cpuinfo.
 */
int get_cpu_count(void);

/**
 * Rellena en `percentages` (array de tamaño >= get_cpu_count()) el porcentaje
 * de uso de cada CPU individual (0.0 – 100.0).
 */
void get_cpu_usage(float *percentages);

#endif // CPUINFO_MANIP_H
