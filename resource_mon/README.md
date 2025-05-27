# resource\_mon

## Descripción

**resource\_mon** es un monitor de recursos para Linux embebido que muestra en tiempo real el uso de CPU y memoria mediante una interfaz de usuario basada en **ncurses**.

## Estructura del proyecto

```text
resource_mon/
├── src/         # Código fuente de los módulos y bucle principal
├── obj/         # Archivos objeto generados por src/Makefile
├── test/        # Pruebas unitarias y Makefile asociado
├── bin/         # Ejecutables de pruebas y binario principal
├── Makefile     # Makefile raíz para compilar src y test
├── src/README.md  # Documentación de módulos y compilación de src
└── test/README.md # Documentación de pruebas unitarias
```

## Requisitos

* **GCC** (compatible con C11)
* **make**
* **ncurses** (paquete de desarrollo: e.g., `libncurses-dev`)

## Compilación y pruebas

Desde la carpeta raíz del proyecto (`resource_mon`), ejecuta:

```bash
# Compila los módulos y las pruebas
make

# Ejecuta las pruebas unitarias
bin/cpuinfo_test
bin/meminfo_test
bin/tui_test
```

Para limpiar los objetos y binarios:

```bash
make clean
```

## Ejecución del monitor

Una vez compilado, inicia la TUI con:

```bash
bin/resource_mon
```

Presiona `q` o `Ctrl+C` para salir.

## Documentación adicional

* **Módulos y compilación de src:** [src/README.md](src/README.md)
* **Pruebas unitarias:** [test/README.md](test/README.md)

## Integrantes

* Omar Andrés Rodríguez Quiceno
* Oscar David Guerrero Hernández
