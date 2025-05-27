#include <stdio.h>
#include "../src/meminfo_manip.h"

int main() {
    MemInfo mi;
    if (get_meminfo(&mi) != 0) {
        printf("Error al obtener la información de memoria.\n");
        return 1;
    }

    printf("Memoria total: %lu KB\n", mi.mem_total_kb);
    printf("Memoria libre: %lu KB\n", mi.mem_free_kb);
    printf("Swap total: %lu KB\n", mi.swap_total_kb);
    printf("Swap libre: %lu KB\n", mi.swap_free_kb);
    printf("Uso memoria: %.2f%%\n", get_mem_usage_percent(&mi));
    printf("Uso swap: %.2f%%\n", get_swap_usage_percent(&mi));

    return 0;
}
