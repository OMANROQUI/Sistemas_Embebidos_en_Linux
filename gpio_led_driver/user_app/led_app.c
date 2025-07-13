#include <fcntl.h>      // Para open()
#include <unistd.h>     // Para close(), read(), write()
#include <stdio.h>      // Para printf(), perror()
#include <stdlib.h>     // Para EXIT_SUCCESS, EXIT_FAILURE

int main(int argc, char **argv) {
    const char *device_path = "/dev/tu_led";   // ← Ajusta el nombre según tu driver
    int fd;

    // 1) Intentar abrir el dispositivo
    printf("Abriendo dispositivo %s...\n", device_path);
    fd = open(device_path, O_RDWR);
    if (fd < 0) {
        perror("Error al abrir el dispositivo");
        return EXIT_FAILURE;
    }
    printf("¡Dispositivo abierto con descriptor: %d!\n", fd);

    // Aquí irá más lógica (write, read, etc.)

    // 2) Cerrar el dispositivo
    printf("Cerrando dispositivo...\n");
    if (close(fd) < 0) {
        perror("Error al cerrar el dispositivo");
        return EXIT_FAILURE;
    }
    printf("¡Dispositivo cerrado con éxito!\n");

    return EXIT_SUCCESS;
}
