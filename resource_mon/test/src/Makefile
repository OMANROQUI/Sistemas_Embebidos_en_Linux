BINDIR      = ../../bin
TEST_SRCS   = cpuinfo_test.c meminfo_test.c tui_test.c
CC          = gcc
CFLAGS      = -std=c11 -Wall -Wextra -I../../src

.PHONY: all clean

all: $(BINDIR) $(TEST_SRCS:%.c=$(BINDIR)/%)

$(BINDIR):
	@mkdir -p $(BINDIR)

$(BINDIR)/cpuinfo_test: cpuinfo_test.c ../../src/cpuinfo_manip.c
	$(CC) $(CFLAGS) $^ -o $@

$(BINDIR)/meminfo_test: meminfo_test.c ../../src/meminfo_manip.c
	$(CC) $(CFLAGS) $^ -o $@

$(BINDIR)/tui_test: tui_test.c ../../src/tui.c
	$(CC) $(CFLAGS) $^ -o $@ -lncurses

clean:
	rm -f $(BINDIR)/cpuinfo_test $(BINDIR)/meminfo_test $(BINDIR)/tui_test
