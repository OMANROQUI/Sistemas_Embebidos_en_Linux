# resource_mon/test/Makefile

# Carpeta donde irán los binarios de test
BINDIR    = ../bin

# Listado explícito de tests
TESTS     = cpuinfo_test meminfo_test tui_test

# Módulos para cada test
MODULE_cpuinfo = ../src/cpuinfo_manip.c
MODULE_meminfo = ../src/meminfo_manip.c
MODULE_tui     = ../src/tui.c

CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -I../src

.PHONY: all clean

all: $(BINDIR) $(patsubst %, $(BINDIR)/%, $(TESTS))

# Asegura que exista la carpeta bin/
$(BINDIR):
	@mkdir -p $(BINDIR)

# cpuinfo_test solo con cpuinfo_manip.c
$(BINDIR)/cpuinfo_test: cpuinfo_test.c $(MODULE_cpuinfo)
	$(CC) $(CFLAGS) $^ -o $@

# meminfo_test solo con meminfo_manip.c
$(BINDIR)/meminfo_test: meminfo_test.c $(MODULE_meminfo)
	$(CC) $(CFLAGS) $^ -o $@

# tui_test con tui.c y ncurses
$(BINDIR)/tui_test: tui_test.c $(MODULE_tui)
	$(CC) $(CFLAGS) $^ -o $@ -lncurses

clean:
	rm -f $(addprefix $(BINDIR)/, $(TESTS))
