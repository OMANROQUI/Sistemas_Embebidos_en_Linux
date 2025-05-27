# resource_mon/src/Makefile

# Directorio donde van los .o
OBJDIR = ../obj

# Todos los .c de este directorio
SRCS   = $(wildcard *.c)
# Mismo nombre, pero con .o en OBJDIR
OBJS   = $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -I.

# Por defecto compilamos todos los objetos
all: $(OBJS)

# Regla genérica: obj/foo.o depende de foo.c
$(OBJDIR)/%.o: %.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Limpia sólo los .o
clean:
	rm -f $(OBJDIR)/*.o

.PHONY: all clean
