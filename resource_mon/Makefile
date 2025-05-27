# Makefile raíz en resource_mon/

.PHONY: all resource_mon tests clean

# Por defecto construye todo
all: resource_mon tests

# Solo ejecutable principal
resource_mon: src
	@mkdir -p bin
	$(CC) $(CFLAGS) -o bin/resource_mon \
	    obj/cpuinfo_manip.o obj/meminfo_manip.o obj/tui.o obj/resource_mon.o \
	    -lncurses

# Solo tests
tests:
	@mkdir -p bin
	$(MAKE) -C test

# Primero compila los .o en obj/
src:
	$(MAKE) -C src

# Limpia .o y ejecutables
clean:
	$(MAKE) -C src clean
	$(MAKE) -C test clean
	rm -rf bin/* obj/*.o
