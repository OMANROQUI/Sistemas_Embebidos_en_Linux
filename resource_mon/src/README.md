# resource\_mon/src/README.md

Este directorio contiene la implementación de los módulos de lectura de información del sistema y la lógica principal del monitor de recursos.

---

## Requisitos

* **GCC**  (versiones compatibles con C11)
* **ncurses** (desarrollo): `libncurses-dev` o `libncurses5-dev`
* **Make**

---

## Compilación

### 1. Compilación de objetos

Dentro de este directorio, ejecuta:

```bash
make       # usa src/Makefile para generar los .o en ../obj/
```

### 2. Compilación del binario principal

Para generar el ejecutable `resource_mon` en `../bin/`, ejecuta en el directorio raíz de `resource_mon`:

```bash
gcc -std=c11 -Wall -Wextra -I src \
    src/cpuinfo_manip.c src/meminfo_manip.c src/tui.c src/resource_mon.c \
    -o bin/resource_mon -lncurses
```

O usando el Makefile raíz:

```bash
cd ..               # subir a resource_mon/
make                # compila src y test, y enlaza (si está configurado)
```

### 3. Ejecución

```bash
./bin/resource_mon    # inicia la interfaz TUI de monitorización
```

---

## Descripción de módulos

### 1. cpuinfo\_manip

* **Archivos:** `cpuinfo_manip.h`, `cpuinfo_manip.c`
* **Funciones:**

  * `int read_cpu_info(CPUInfo *info);`  Lee `/proc/cpuinfo`, recupera nombre de modelo y cuenta núcleos.
  * `int read_cpu_stat(CPUStat *stat);`  Lee primera línea de `/proc/stat` y rellena estadísticas globales.
  * `double calculate_cpu_usage(const CPUStat *prev, const CPUStat *curr);`  Calcula el porcentaje de uso entre dos lecturas.
  * `int get_cpu_count(void);`  Devuelve el número de CPUs lógicos detectados.
  * `void get_cpu_usage(float *percentages);`  Rellena un array con el uso (%) de cada CPU individual.

### 2. meminfo\_manip

* **Archivos:** `meminfo_manip.h`, `meminfo_manip.c`
* **Funciones:**

  * `int read_mem_info(MemInfo *info);`  Lee `/proc/meminfo`, recupera `MemTotal`, `MemFree` y `MemAvailable`.
  * `double calculate_mem_usage(const MemInfo *info);`  Calcula el porcentaje de memoria usada.

### 3. tui

* **Archivos:** `tui.h`, `tui.c`
* **Funciones:**

  * Inicialización y configuración de pantalla con ncurses (`initscr`, `noecho`, etc.).
  * Funciones para dibujar texto, refrescar pantalla y leer teclas sin bloqueo.
  * Limpieza de la interfaz (`endwin`).

### 4. resource\_mon.c (bucle principal)

* Orquesta la lectura periódica de CPU y memoria.
* Actualiza la TUI en pantalla cada segundo.
* Captura señales (`SIGINT`, `SIGTERM`) y teclas (`q`) para terminar.

---

## Limpieza

Para borrar los objetos compilados dentro de `src`:

```bash
make clean
```

Y para eliminar el binario principal (si se compiló manualmente):

```bash
rm -f ../bin/resource_mon
```
