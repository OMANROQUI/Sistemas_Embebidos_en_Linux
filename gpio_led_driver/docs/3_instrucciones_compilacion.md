# 3. Instrucciones de compilación (módulo + app)

## Requisitos previos
- Kernel headers instalados (`linux-headers-$(uname -r)`).
- Herramientas de compilación (gcc, make).

## Compilar el módulo kernel
```bash
cd ~/Sistemas_Embebidos_en_Linux/gpio_led_driver/kernel_module
make clean
make
