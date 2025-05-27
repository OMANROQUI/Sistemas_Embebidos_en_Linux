#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include "tui.h"

void tui_init() {
    initscr();
    noecho();
    curs_set(FALSE);
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    clear();
}

void tui_cleanup() {
    endwin();
}

void tui_draw(float cpu_percent[], int cpu_count, float mem_usage, float swap_usage) {
    clear();
    mvprintw(0, 0, "Monitor de Recursos del Sistema");
    
    for (int i = 0; i < cpu_count; i++) {
        mvprintw(i + 2, 0, "CPU %d: %.2f%%", i, cpu_percent[i]);
    }

    mvprintw(cpu_count + 3, 0, "Memoria usada: %.2f%%", mem_usage);
    mvprintw(cpu_count + 4, 0, "Swap usado: %.2f%%", swap_usage);

    mvprintw(cpu_count + 6, 0, "Presiona 'q' para salir.");
    refresh();
}

char tui_read_key() {
    int ch = getch();
    return (ch == ERR) ? 0 : (char) ch;
}
