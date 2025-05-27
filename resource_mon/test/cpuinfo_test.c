#include "cpuinfo_manip.h"
#include <assert.h>
#include <stdio.h>
#include <unistd.h>    // para sleep()

int main(void) {
    CPUInfo info;
    assert(read_cpu_info(&info) == 0);
    printf("Modelo: %s\nNúcleos: %u\n", info.model_name, info.cores);
    assert(info.cores > 0);
    assert(info.model_name[0] != '\0');

    CPUStat s1, s2;
    assert(read_cpu_stat(&s1) == 0);
    sleep(1);
    assert(read_cpu_stat(&s2) == 0);

    double uso = calculate_cpu_usage(&s1, &s2);
    printf("Uso CPU (1s): %.2f%%\n", uso);
    assert(uso >= 0.0 && uso <= 100.0);

    printf("CPU module tests passed.\n");
    return 0;
}
