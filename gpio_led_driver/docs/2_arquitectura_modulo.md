# 2. Arquitectura del módulo de kernel

El módulo está estructurado en:

1. **Inicialización** (`module_init`):  
   - Registro de major/minor con `register_chrdev`.  
   - Configuración del GPIO (export, dirección de salida).

2. **Operaciones de archivo** (`file_operations`):  
   - `open`: prepara el acceso.  
   - `release`: libera recursos.  
   - `read`: devuelve estado actual del LED.  
   - `write`: enciende o apaga según el byte recibido ('0'|'1').

3. **Exit** (`module_exit`):  
   - Desregistra el chrdev.  
   - Libera el GPIO.
