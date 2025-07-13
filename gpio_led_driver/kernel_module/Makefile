obj-m += led_driver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

.PHONY: test-unit

test-unit:
	@echo "Ejecutando pruebas unitarias..."
	@./tests/unit_tests.sh
