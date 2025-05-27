#include "../src/tui.h"
#include <unistd.h>

int main() {
    tui_init();

    float cpu[2] = {25.5, 32.7};
    tui_draw(cpu, 2, 44.2, 12.8);
    sleep(3);

    tui_cleanup();
    return 0;
}
