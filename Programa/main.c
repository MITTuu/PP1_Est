#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void mostrar_menu() {
    printf(" _____________________________________________________________ \n");
    printf("|                                                             | \n");
    printf("|            Sistema de Análisis de Datos de Ventas           |\n");
    printf("|_____________________________________________________________|\n\n");
    printf("    1.  Importación de datos\n");
    printf("    2.  Procesamiento de datos\n");
    printf("    3.  Análisis de datos\n");
    printf("    4.  Análisis temporal\n");
    printf("    5.  Estadísticas\n");
    printf("    6.  Salir\n");
    printf(" _____________________________________________________________ \n");
    printf("  Seleccione una opción: ");
}

int main() {

    #ifdef _WIN32
    system("chcp 65001 > nul");
    #endif

    char opcion;

    do {
        mostrar_menu();
        scanf(" %c", &opcion);

        switch (opcion)
        {
        case '1':
            printf("Opción 1\n");
            break;

        case '2':
            printf("Opción 2\n");
            break;

        case '3':
            printf("Opción 3\n");
            break;

        case '4':
            printf("Opción 4\n");
            break;

        case '5':
            printf("Opción 5\n");
            break;

        case '6':
            printf("Saliendo del programa...\n");
            break;

        default:
            printf("Opción inválida.\n");
        }

    } while(opcion != '6');

    return 0;
}