#ifndef TUI_H
#define TUI_H

void tui_init();
void tui_cleanup();
void tui_draw(float cpu_percent[], int cpu_count, float mem_usage, float swap_usage);
char tui_read_key();

#endif // TUI_H
