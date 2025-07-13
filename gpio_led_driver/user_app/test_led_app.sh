#!/bin/bash
APP=./led_app
DRIVER_MODULE=../kernel_module/led_driver.ko
DEVICE=/dev/tu_led

echo
echo "=== PRUEBAS DE INTEGRACIÓN: led_app + driver ==="
echo

# 0) Descargar driver si ya está cargado
echo -n "Descargando driver antiguo… "
sudo rmmod led_driver 2>/dev/null && echo "OK" || echo "No estaba cargado"

# 1) Vaciar buffer del kernel (opcional)
sudo dmesg -C

# 2) Cargar driver
echo -n "Cargando driver… "
sudo insmod $DRIVER_MODULE && echo "OK" || { echo "FALLÓ"; exit 1; }
sleep 1

# 3) Crear nodo de dispositivo si falta
if [ ! -e $DEVICE ]; then
  MAJOR=$(awk "/gpio_led/ {print \$1}" /proc/devices)
  echo "Creando $DEVICE (major=$MAJOR, minor=0)…"
  sudo mknod $DEVICE c $MAJOR 0
  sudo chmod 666 $DEVICE
fi

# 4) Compilar la aplicación si no existe
if [ ! -x $APP ]; then
  echo -n "Compilando led_app… "
  gcc -o led_app led_app.c
  if [ $? -ne 0 ]; then echo "FALLÓ"; exit 1; else echo "OK"; fi
fi

# 5) Estado inicial (off)
echo -n "Estado inicial (esperado off): "
state=$(cat $DEVICE | tr -d '\r\n')
if [ "$state" = "off" ]; then echo "PASÓ"; else echo "FALLÓ (obtuvo='$state')"; fi

# 6) Prueba ON
echo -n "Ejecutando led_app on… "
$APP on
sleep 1
state=$(cat $DEVICE | tr -d '\r\n')
if [ "$state" = "on" ]; then echo "PASÓ"; else echo "FALLÓ (device='$state')"; fi

# 7) Prueba OFF
echo -n "Ejecutando led_app off… "
$APP off
sleep 1
state=$(cat $DEVICE | tr -d '\r\n')
if [ "$state" = "off" ]; then echo "PASÓ"; else echo "FALLÓ (device='$state')"; fi

# 8) Descarga el driver
echo -n "Descargando driver… "
sudo rmmod led_driver && echo "OK" || echo "FALLÓ"

# 9) Mostrar logs relevantes
echo
echo ">> dmesg de gpio_led:"
dmesg | grep gpio_led
echo
echo "=== FIN DE PRUEBAS INTEGRACIÓN ==="
