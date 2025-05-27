#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "cpuinfo_manip.h"
#include "meminfo_manip.h"
#include "tui.h"

volatile sig_atomic_t running = 1;

void handle_signal(int sig) {
    running = 0;
}

int main() {
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    tui_init();

    int cpu_count = get_cpu_count();  // Esta función debe estar definida en cpuinfo_manip
    float *cpu_usage = malloc(sizeof(float) * cpu_count);
    MemInfo mem;

    while (running) {
        get_cpu_usage(cpu_usage);       // Actualiza cpu_usage[]
        get_meminfo(&mem);              // Llena la struct MemInfo

        float mem_percent = get_mem_usage_percent(&mem);
        float swap_percent = get_swap_usage_percent(&mem);

        tui_draw(cpu_usage, cpu_count, mem_percent, swap_percent);

        char key = tui_read_key();
        if (key == 'q') break;

        sleep(1);
    }

    tui_cleanup();
    free(cpu_usage);
    return 0;
}
