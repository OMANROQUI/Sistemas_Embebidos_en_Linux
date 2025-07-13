#include <fcntl.h>      // Para open()
#include <unistd.h>     // Para close(), read(), write()
#include <stdio.h>      // Para printf(), perror()
#include <stdlib.h>     // Para EXIT_SUCCESS, EXIT_FAILURE
#include <string.h>

#define CMD_ON     "on"
#define CMD_OFF    "off"
#define CMD_STATUS "status"

int main(int argc, char **argv) {
    const char *device_path = "/dev/tu_led";
    int fd, ret;
    char buf[8] = {0};   // Espacio para "off"+\0

    if (argc != 2) {
        fprintf(stderr, "Uso: %s <%s|%s|%s>\n",
                argv[0], CMD_ON, CMD_OFF, CMD_STATUS);
        return EXIT_FAILURE;
    }

    fd = open(device_path, O_RDWR);
    if (fd < 0) {
        perror("Error al abrir el dispositivo");
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], CMD_ON) == 0) {
        printf("Encendiendo LED...\n");
        ret = write(fd, CMD_ON, strlen(CMD_ON));
        if (ret < 0) perror("Error al escribir ON");
    }
    else if (strcmp(argv[1], CMD_OFF) == 0) {
        printf("Apagando LED...\n");
        ret = write(fd, CMD_OFF, strlen(CMD_OFF));
        if (ret < 0) perror("Error al escribir OFF");
    }
    else if (strcmp(argv[1], CMD_STATUS) == 0) {
        printf("Consultando estado...\n");
        ret = read(fd, buf, sizeof(buf)-1);
        if (ret > 0) {
            buf[ret] = '\0';  // Asegura terminación
            printf("Estado LED: %s\n", buf);
        } else {
            perror("Error al leer estado");
        }
    }
    else {
        fprintf(stderr, "Comando desconocido: %s\n", argv[1]);
        close(fd);
        return EXIT_FAILURE;
    }

    close(fd);
    return EXIT_SUCCESS;
}
