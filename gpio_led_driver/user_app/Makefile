# user_app/Makefile

CC      := gcc
CFLAGS  := -Wall -O2
TARGET  := led_app
SRC     := led_app.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGET)
