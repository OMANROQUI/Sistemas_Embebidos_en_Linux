#!/usr/bin/env bash
set -euo pipefail

MODULE="./kernel_module/led_driver.ko"
DEVICE_NODE="/dev/tu_led"
MAJOR=239
MINOR=0
APP="./user_app/led_app"

echo "=== PRUEBAS DE INTEGRACIÓN LED ==="

# 0) Compilar módulo de kernel
echo "-> Compilando módulo de kernel…"
pushd kernel_module >/dev/null
make clean
make
popd  >/dev/null

# 0.1) Compilar aplicación de usuario
echo "-> Compilando aplicación de usuario…"
pushd user_app >/dev/null
make clean
make
popd >/dev/null

# 1) Descargar cualquier módulo previo
echo "-> Descargando módulo anterior (si existe)…"
sudo rmmod led_driver 2>/dev/null || true

# 2) Cargar el nuevo módulo
echo "-> Cargando módulo: $MODULE"
sudo insmod "$MODULE"

# 3) (Re)crear el nodo de dispositivo
echo "-> Configurando nodo $DEVICE_NODE (major=$MAJOR, minor=$MINOR)…"
sudo rm -f "$DEVICE_NODE"
sudo mknod "$DEVICE_NODE" c $MAJOR $MINOR
sudo chmod 666 "$DEVICE_NODE"

# 4) Prueba: encender
echo
echo ">>> Test: ENCENDER"
$APP on

# 5) Prueba: estado tras encender
echo
echo ">>> Test: STATUS (debe mostrar 'on')"
$APP status

# 6) Prueba: apagar
echo
echo ">>> Test: APAGAR"
$APP off

# 7) Prueba: estado tras apagar
echo
echo ">>> Test: STATUS (debe mostrar 'off')"
$APP status

# 8) Limpiar: descargar el módulo
echo
echo "-> Descargando módulo al terminar pruebas…"
sudo rmmod led_driver

echo
echo "=== PRUEBAS COMPLETADAS ==="
