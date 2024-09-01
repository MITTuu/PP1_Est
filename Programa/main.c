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

void mostrarSubmenuAnalisisTemporal() {
    printf(" _____________________________________________________________ \n");
    printf("|                                                             | \n");
    printf("|                      Análisis Temporal                      |\n");
    printf("|_____________________________________________________________|\n\n");
    printf("    1. Mes con mayor venta\n");
    printf("    2. Día de la semana más activo\n");
    printf("    3. Calcular tasa de crecimiento/decrecimiento\n");
    printf("    4. Volver al menú principal\n");
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
    // Solicitar tamaño del buffer al usuario para ajustar la memoria dinámica
    size_t longitud = 256;
    char *path = (char *)malloc(longitud * sizeof(char));
    if (path == NULL) {
        fprintf(stderr, "Error al asignar memoria.\n");
        exit(EXIT_FAILURE);
    }
    
    leerRutaArchivo(path, longitud);
    importarDatos(lista, path);
    
    free(path);
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
                    printf("%2zu) %-30s - Total: %.2f\n", i + 1, meses_totales[i], totales_mensuales[i]);
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
                    printf("%d) %s  		- Total: %.2f\n", i+1, años_totales[i], totales_anuales[i]);
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

void manejarAnalisisTemporal(listaVentas *lista) {
    char subOpcion;
    do {
        mostrarSubmenuAnalisisTemporal();
        scanf(" %c", &subOpcion);
        printf("\n");
        while (getchar() != '\n');

        switch (subOpcion) {
            case '1': {
                char *mes_mayor_venta = mesConMayorVenta(lista);
                printf("Mes con mayor venta: %s\n", mes_mayor_venta);
                break;
            }
            case '2': {
                char *dia_mas_activo = diaMasActivo(lista);
                printf("Día de la semana más activo: %s\n", dia_mas_activo);
                break;
            }
            case '3': {
                int trimestre, anio;
                printf("Ingrese el trimestre (1-4): ");
                scanf("%d", &trimestre);
                printf("Ingrese el año: ");
                scanf("%d", &anio);
                float tasa = tasaCrecimientoTrimestral(lista, trimestre, anio);
                printf("Tasa de crecimiento/decrecimiento: %.2f%%\n", tasa);
                break;
            }
            case '4':
                printf("Volviendo al menú principal...\n");
                break;

            default:
                printf("Opción inválida. Por favor, intente nuevamente.\n");
                break;
        }
    } while (subOpcion != '4');
}

void manejarMenuPrincipal() {
    listaVentas *lista = crearListaVentas();
    char opcion;
    char *datos_previos = leerArchivo("ventas_procesadas.json");
    if (datos_previos != NULL) {
        importarDatos(lista, "ventas_procesadas.json");
        free(datos_previos);
    }

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
                manejarAnalisisTemporal(lista);
                break;

            case '5':
                obtenerTopCategorias(lista);
                break;

            case '6':
                guardarDatosProcesados(lista, "ventas_procesadas.json");
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
