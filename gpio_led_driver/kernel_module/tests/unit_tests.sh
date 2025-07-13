#!/bin/bash
DEVICE=/dev/tu_led
MODULE=led_driver

echo "=== PRUEBAS UNITARIAS DE $MODULE ==="

# 0) Descargar el módulo si ya está cargado (ignora errores)
sudo rmmod $MODULE 2>/dev/null || true

# 1) Cargar módulo
echo -n "Cargando módulo... "
sudo insmod ${MODULE}.ko && echo "OK" || { echo "FALLÓ"; exit 1; }
sleep 1

# 2) Verificar estado inicial (esperado off)
echo -n "Estado inicial (esperado off): "
state=$(cat $DEVICE | tr -d '\r\n')
if [ "$state" = "off" ]; then
  echo "PASÓ"
else
  echo "FALLÓ (obtuvo $state)"
fi

# 3) Encender LED (on)
echo -n "Encendiendo LED (on): "
echo on > $DEVICE 2>/dev/null
sleep 1
state=$(cat $DEVICE | tr -d '\r\n')
if [ "$state" = "on" ]; then
  echo "PASÓ"
else
  echo "FALLÓ (obtuvo $state)"
fi

# 4) Apagar LED (off)
echo -n "Apagando LED (off): "
echo off > $DEVICE 2>/dev/null
sleep 1
state=$(cat $DEVICE | tr -d '\r\n')
if [ "$state" = "off" ]; then
  echo "PASÓ"
else
  echo "FALLÓ (obtuvo $state)"
fi

# 5) Descargar módulo
echo -n "Descargando módulo... "
sudo rmmod $MODULE && echo "OK" || echo "FALLÓ"

echo "=== FIN DE PRUEBAS ==="
