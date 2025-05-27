# resource\_mon/test/README.md

Este directorio contiene las pruebas unitarias para los módulos de **resource\_mon**.

---

## Archivos de prueba

* **cpuinfo\_test.c**

  * Prueba la lectura de `/proc/cpuinfo` y `/proc/stat`.
  * Funciones testadas:

    * `read_cpu_info()`
    * `read_cpu_stat()`
    * `calculate_cpu_usage()`
  * **Resultado esperado:**

    ```
    Modelo: <nombre modelo>
    Núcleos: <número de núcleos > 0>
    Uso CPU (1s): <porcentaje entre 0.00% y 100.00%>
    CPU module tests passed.
    ```

* **meminfo\_test.c**

  * Prueba la lectura de `/proc/meminfo` y el cálculo de uso de memoria.
  * Funciones testadas:

    * `read_mem_info()`
    * `calculate_mem_usage()`
  * **Resultado esperado:**

    ```
    Total: <valor en KB > 0 KB
    Free: <valor en KB > 0 KB
    Available: <valor en KB <= Total KB>
    Uso Memoria: <porcentaje entre 0.00% y 100.00%>
    Memoria module tests passed.
    ```

* **tui\_test.c**

  * Prueba la inicialización, funciones básicas y limpieza de la interfaz TUI con **ncurses**.
  * Funciones testadas:

    * `tui_init()`
    * `tui_draw()` (borrando y escribiendo una línea de ejemplo)
    * `tui_read_key()` (sin bloqueo)
    * `tui_cleanup()`
  * **Resultado esperado:**

    * El test retorna **0** y puede mostrar internamente:

      ```
      TUI module tests passed.
      ```

---

## Instrucciones para ejecutar las pruebas

1. Desde el directorio `resource_mon`, compila los tests:

   ```bash
   make -C test
   ```
2. Ejecuta cada prueba:

   ```bash
   ./bin/cpuinfo_test
   ./bin/meminfo_test
   ./bin/tui_test
   ```
3. Comprueba que cada uno imprime la línea `* module tests passed.` y retorna 0.

---

## Limpieza

Para eliminar los binarios de prueba:

```bash
make -C test clean
```
