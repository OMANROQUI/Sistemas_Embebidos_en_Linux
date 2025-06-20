// led_control.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DEVICE_PATH "/dev/led_driver"

// Muestra el menú interactivo
void print_menu() {
    printf("\n=== Control de LED ===\n");
    printf("1) Encender LED\n");
    printf("2) Apagar LED\n");
    printf("3) Salir\n");
    printf("Elige una opción: ");
}

// Función que abre el dispositivo y escribe '1' o '0'
int toggle_led(char val) {
    int fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Error al abrir el dispositivo");
        return -1;
    }
    if (write(fd, &val, 1) != 1) {
        perror("Error al escribir en el dispositivo");
        close(fd);
        return -1;
    }
    close(fd);
    printf("LED %s correctamente.\n", (val=='1') ? "encendido" : "apagado");
    return 0;
}

int main(int argc, char **argv) {
    // Modo argumento: ./led_control on/off
    if (argc == 2) {
        if      (strcmp(argv[1], "on")  == 0) return toggle_led('1');
        else if (strcmp(argv[1], "off") == 0) return toggle_led('0');
        else {
            fprintf(stderr, "Uso: %s [on|off]\n", argv[0]);
            return EXIT_FAILURE;
        }
    }

    // Modo interactivo (menú) si no se pasan argumentos
    int choice;
    while (1) {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "Entrada inválida. Intenta de nuevo.\n");
            int c; while ((c = getchar()) != '\n' && c != EOF); // limpiar buffer
            continue;
        }
        switch (choice) {
            case 1: toggle_led('1'); break;
            case 2: toggle_led('0'); break;
            case 3: 
                printf("Saliendo...\n");
                return EXIT_SUCCESS;
            default:
                printf("Opción inválida. Elige 1, 2 o 3.\n");
        }
    }

    return EXIT_SUCCESS;
}
