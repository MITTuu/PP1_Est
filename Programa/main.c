#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ventas.h"

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

void leerRutaArchivo(char *ruta, size_t longitud) {
    printf("Ingrese la ruta del archivo JSON: ");
    fgets(ruta, longitud, stdin);
    printf("\n");
    
    // Eliminar el salto de línea al final de la entrada
    size_t len = strlen(ruta);
    if (len > 0 && ruta[len - 1] == '\n') {
        ruta[len - 1] = '\0';
    }
}

int main() {
    // Configurar la consola para UTF-8
    #ifdef _WIN32
    system("chcp 65001 > nul");
    #endif

    listaVentas *lista = crearListaVentas();
    char opcion;
    char ruta_archivo[256];

    do {
        mostrar_menu();
        scanf(" %c", &opcion);
        // Limpiar el buffer de entrada
        while (getchar() != '\n');

        switch (opcion) {
        case '1':
            // Leer la ruta del archivo JSON desde la consola
            leerRutaArchivo(ruta_archivo, sizeof(ruta_archivo));
            importarDatos(lista, ruta_archivo);
            break;

        case '2':
            eliminarDatosDuplicados(lista);
            completarDatosFaltantes(lista);
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

    } while (opcion != '6');

    liberarListaVentas(lista);
    return 0;
}