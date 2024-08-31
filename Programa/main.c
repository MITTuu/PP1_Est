#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ventas.h"

void mostrarMenu() {
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

void mostrarSubmenuAnalisis() {
    printf(" _____________________________________________________________ \n");
    printf("|                                                             | \n");
    printf("|                       Análisis de Datos                     |\n");
    printf("|_____________________________________________________________|\n\n");
    printf("    1.  Total de ventas\n");
    printf("    2.  Total de ventas mensuales\n");
    printf("    3.  Total de ventas anuales\n");
    printf("    4.  Volver al menu principal\n");
    printf(" _____________________________________________________________ \n");
    printf("  Seleccione una opción: ");
}

void leerRutaArchivo(char *path, size_t longitud) {
    printf("Ingrese la ruta del archivo JSON: ");
    fgets(path, longitud, stdin);
    printf("\n");
    
    // Eliminar el salto de línea al final de la entrada
    size_t len = strlen(path);
    if (len > 0 && path[len - 1] == '\n') {
        path[len - 1] = '\0';
    }
}

void manejarImportacion(listaVentas *lista) {
    char path[256];
    leerRutaArchivo(path, sizeof(path));
    importarDatos(lista, path);
}

void manejarProcesamiento(listaVentas *lista) {
    eliminarDatosDuplicados(lista);
    completarDatos(lista);
}

void manejarAnalisis(listaVentas *lista) {
    char subOpcion1;
    do {
        mostrarSubmenuAnalisis();
        scanf(" %c", &subOpcion1);
        printf("\n");
        while (getchar() != '\n');

        switch (subOpcion1) {
            case '1': {
                float total = totalVentas(lista);
                printf("Total de ventas: %.2f\n", total);
                break;
            }
            case '2': {
                char **meses_totales;
                float *totales_mensuales;
                size_t num_meses;

                totalVentasMensuales(lista, &meses_totales, &totales_mensuales, &num_meses);
                for (size_t i = 0; i < num_meses; i++) {
                    printf("Mes: %s - Total: %.2f\n", meses_totales[i], totales_mensuales[i]);
                    free(meses_totales[i]);
                }
                free(meses_totales);
                free(totales_mensuales);
                break;
            }
            case '3': {
                char **años_totales;
                float *totales_anuales;
                size_t num_años;

                totalVentasAnuales(lista, &años_totales, &totales_anuales, &num_años);
                for (size_t i = 0; i < num_años; i++) {
                    printf("Año: %s - Total: %.2f\n", años_totales[i], totales_anuales[i]);
                    free(años_totales[i]);
                }
                free(años_totales);
                free(totales_anuales);
                break;
            }
            case '4':
                printf("Volviendo al menú principal...\n");
                break;

            default:
                printf("Opción inválida. Por favor, intente nuevamente.\n");
                break;
        }
    } while (subOpcion1 != '4');
}

void manejarMenuPrincipal() {
    listaVentas *lista = crearListaVentas();
    char opcion;

    do {
        mostrarMenu();
        scanf(" %c", &opcion);
        printf("\n");
        while (getchar() != '\n');

        switch (opcion) {
            case '1':
                manejarImportacion(lista);
                break;

            case '2':
                manejarProcesamiento(lista);
                break;

            case '3':
                manejarAnalisis(lista);
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
}

int main() {
    #ifdef _WIN32
    system("chcp 65001 > nul");
    #endif

    manejarMenuPrincipal();
    
    return 0;
}
