.PHONY: all resource_mon tests clean

all: resource_mon tests

resource_mon:
	$(MAKE) -C src

tests:
	$(MAKE) -C test

clean:
	$(MAKE) -C src clean
	$(MAKE) -C test clean
	rm -f obj/*.o
	rm -f bin/*
