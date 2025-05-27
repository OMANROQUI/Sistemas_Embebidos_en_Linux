# resource\_mon

## Descripción

**resource\_mon** es un monitor de recursos para Linux embebido que muestra en tiempo real el uso de CPU y memoria mediante una interfaz de usuario basada en **ncurses**.

## Estructura del proyecto

```text
resource_mon/
├── bin/            # Ejecutables: resource_mon, cpuinfo_test, meminfo_test, tui_test
├── obj/            # Archivos objeto generados por src/Makefile
├── src/            # Código fuente:
│   ├── cpuinfo_manip.c
│   ├── cpuinfo_manip.h
│   ├── meminfo_manip.c
│   ├── meminfo_manip.h
│   ├── tui.c
│   ├── tui.h
│   ├── resource_mon.c
│   ├── Makefile    # Compila .c → .o en ../obj/
│   └── README.md   # Documentación de módulos y compilación
├── test/           # Pruebas unitarias:
│   ├── src/
│   │   ├── cpuinfo_test.c
│   │   ├── meminfo_test.c
│   │   ├── tui_test.c
│   │   └── Makefile  # Compila tests → ../bin/
│   ├── Makefile     # Wrapper: invoca test/src/Makefile
│   └── README.md    # Documentación de pruebas unitarias
├── Makefile        # Root Makefile: targets resource_mon, tests, all, clean
└── README.md       # Esta documentación principal
```

## Requisitos

* **gcc** (compatible con C11)
* **make**
* **ncurses** (paquete de desarrollo; e.g., `libncurses-dev`)

## Uso de make

Desde la raíz de `resource_mon/`:

```bash
# 1. Compilar todo (objetos y ejecutables)
make

# 2. Ejecutar pruebas unitarias
bin/cpuinfo_test
bin/meminfo_test
bin/tui_test

# 3. Ejecutar el monitor
bin/resource_mon

# 4. Limpiar objetos y binarios
git clean
```

### Targets disponibles

* `make resource_mon` → compila el ejecutable principal (`bin/resource_mon`)
* `make tests`        → compila los tests (`bin/cpuinfo_test`, `bin/meminfo_test`, `bin/tui_test`)
* `make all`          → ejecuta `resource_mon` + `tests`
* `make clean`        → borra `obj/*.o` y `bin/*`

## Ejecución

```bash
# Inicia la interfaz TUI
bin/resource_mon
```

Dentro de la TUI, presiona `q` o `Ctrl+C` para salir.

## Documentación adicional

* [Módulos y compilación de src](src/README.md)
* [Pruebas unitarias](test/README.md)

## Integrantes

* Omar Andrés Rodríguez Quiceno cc:1007702903
* Oscar David Guerrero Hernández cc:1192734080
