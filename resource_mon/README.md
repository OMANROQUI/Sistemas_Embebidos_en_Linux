# Sistemas_Embebidos_en_Linux
# Sistema de Monitoreo de Recursos en Linux

**Objetivo:**  
Implementar un monitor TUI que muestre uso de CPU y memoria en tiempo real.

**Integrantes:**  
- Omar Andrés Rodriguez Quiceno  
- Oscar David Guerrero Hernandez  

## Módulo meminfo_manip

Este módulo proporciona funciones para leer información de memoria desde `/proc/meminfo`.

### Funciones

- `int get_meminfo(MemInfo *info)`: llena una estructura `MemInfo` con datos del sistema.
- `float get_mem_usage_percent(const MemInfo *info)`: calcula el porcentaje de uso de memoria.
- `float get_swap_usage_percent(const MemInfo *info)`: calcula el porcentaje de uso de swap.

### Estructuras

```c
typedef struct {
    unsigned long mem_total_kb;
    unsigned long mem_free_kb;
    unsigned long swap_total_kb;
    unsigned long swap_free_kb;
} MemInfo;
