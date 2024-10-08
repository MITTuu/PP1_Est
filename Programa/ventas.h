#ifndef VENTAS_H
#define VENTAS_H

/*****Datos administrativos************************
 * Nombre del archivo: ventas
 * Tipo de archivo: C Encabezado
 * Proyecto: Sistema de Análisis de Datos de Ventas
 * Autor: Dylan Montiel Zúñiga
 *****Descripción**********************************
 *
 *****Versión**************************************
 * 4.0 | 08/31/2024 | Dylan Montiel Zúñiga
 **************************************************/

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "funcs_json.h"

/*****Nombre****************************************
 * struct Venta
 *****Descripción***********************************
 * Representa una venta con detalles asociados, 
 * incluyendo el identificador de la venta, la fecha,
 * el identificador y nombre del producto, la categoría,
 * la cantidad vendida y el precio unitario.
 *****Campos****************************************
 * @int venta_id: Identificador único de la venta.
 * @char *fecha: Fecha en la que se realizó la venta.
 * @int producto_id: Identificador único del producto.
 * @char *producto_nombre: Nombre del producto.
 * @char *categoria: Categoría del producto.
 * @int cantidad: Cantidad de unidades vendidas.
 * @float precio_unitario: Precio por unidad del producto.
 * @float total: Total calculado para la venta.
 ***************************************************/
typedef struct {
    int venta_id;
    char *fecha;
    int producto_id;
    char *producto_nombre;
    char *categoria;
    int cantidad;
    float precio_unitario;
    float total;
} Venta;

/*****Nombre****************************************
 * struct listaVentas
 *****Descripción***********************************
 * Representa una lista dinámica de ventas. 
 * Incluye un arreglo de punteros a estructuras `Venta`,
 * el tamaño actual de la lista y la capacidad total del arreglo.
 *****Campos****************************************
 * @Venta *ventas: Puntero a un arreglo dinámico de estructuras `Venta`.
 * @size: Tamaño actual de la lista.
 * @capacity: Capacidad total del arreglo de ventas.
 ***************************************************/
typedef struct {
    Venta *ventas;
    size_t size;
    size_t capacity;
} listaVentas;

/*****Nombre****************************************
 * struct CategoriaVenta
 *****Descripción***********************************
 * Representa una lista dinámica de ventas por categoria
 *****Campos****************************************
 * @categoria: Categoria de productos vendidos.
 * @totalVentas: Total de ventas.
 ***************************************************/
typedef struct {
    char *categoria;
    float totalVentas;
} CategoriaVenta;

/*****Nombre***************************************
 * Función crearListaVentas
 *****Descripción**********************************
 * Crea una nueva lista de ventas inicializando un struct
 * `listaVentas` con un arreglo dinámico para almacenar 
 * objetos `Venta`. La capacidad inicial del arreglo es 
 * 10 y el tamaño inicial es 0.
 *****Retorno**************************************
 * @return: Un puntero a una nueva instancia de `listaVentas` 
 *          con el arreglo de ventas inicializado y la capacidad 
 *          establecida en 10. Retorna `NULL` si la asignación de 
 *          memoria falla.
 ****Entradas************************************** 
 * 
 **************************************************/
listaVentas* crearListaVentas() {
    // Asignar memoria para el struct listaVentas
    listaVentas *lista = (listaVentas *)malloc(sizeof(listaVentas));
    if (lista == NULL) {
        printf("Error al asignar memoria para la lista de ventas.\n");
        return NULL;
    }
    
    // Asignar memoria para un array de structs Venta de tamaño inicial 10
    lista->ventas = (Venta *)malloc(sizeof(Venta) * 10);
    if (lista->ventas == NULL) {
        printf("Error al asignar memoria para las ventas.\n");
        free(lista);
        return NULL;
    }
    
    // Inicializar campos del struct listaVentas
    lista->size = 0;       
    lista->capacity = 10;  

    return lista;
}

/*****Nombre***************************************
 * Función agregarVenta
 *****Descripción**********************************
 * Agrega una nueva venta a la lista dinámica de ventas. 
 * Si la lista está llena, duplica la capacidad del arreglo 
 * de ventas mediante `realloc` para acomodar la nueva venta.
 *****Retorno**************************************
 * 
 ****Entradas************************************** 
 * @param lista: Un puntero al struct `listaVentas`.
 * @param venta: Un struct `Venta`.
 **************************************************/
void agregarVenta(listaVentas *lista, Venta nuevaVenta) {
    if (lista == NULL) {
        printf("Error: La lista de ventas no está inicializada.\n");
        return;
    }
    
    // Verificar si hay suficiente capacidad; si no, redimensionar el array
    if (lista->size >= lista->capacity) {
        // Duplicar la capacidad actual
        lista->capacity *= 2;
        Venta *temp = (Venta *)realloc(lista->ventas, sizeof(Venta) * lista->capacity);
        if (temp == NULL) {
            printf("Error al redimensionar la memoria para las ventas.\n");
            return;
        }
        lista->ventas = temp;
    }
    
    // Agregar la nueva venta
    lista->ventas[lista->size] = nuevaVenta;
    lista->size++;
}


/*****Nombre***************************************
 * Función liberarListaVentas
 *****Descripción**********************************
 * Libera la memoria asociada con una lista de ventas. 
 * Primero libera la memoria de las cadenas de caracteres 
 * asociadas con cada estructura `Venta`, luego libera 
 * el arreglo de estructuras `Venta` y finalmente libera 
 * la memoria de la estructura `listaVentas`.
 *****Retorno**************************************
 * 
 ****Entradas************************************** 
 * @param lista: Un puntero al struct `listaVentas`.
 **************************************************/
void liberarListaVentas(listaVentas *lista) {
    if (lista != NULL) {
        free(lista->ventas); 
        free(lista);         
    }
}

// Función para construir el mensaje de error sobre atributos faltantes
void reportarAtributosFaltantes(cJSON *item, int linea) {
    const char *atributos[] = { "venta_id", "fecha", "producto_id", "producto_nombre", "categoria" };
    const char *nombre_atributos[] = { "Identificador de venta", "Fecha", "Identificador de producto", "Nombre de producto", "Categoría" };
    int faltan = 0;
    char mensaje[256] = "La línea ";
    snprintf(mensaje + strlen(mensaje), sizeof(mensaje) - strlen(mensaje), "%d no se pudo importar debido a que faltan los atributos: ", linea);

    for (int i = 0; i < 5; i++) {
        if (!cJSON_HasObjectItem(item, atributos[i])) {
            if (faltan > 0) {
                strncat(mensaje, ", ", sizeof(mensaje) - strlen(mensaje));
            }
            strncat(mensaje, nombre_atributos[i], sizeof(mensaje) - strlen(mensaje));
            faltan++;
        }
    }

    if (faltan == 0) {
        // Si no faltan atributos (esto no debería pasar con esta función), eliminar el mensaje
        mensaje[0] = '\0';
    } else {
        strncat(mensaje, ".", sizeof(mensaje) - strlen(mensaje));
        printf("%s\n", mensaje);
    }
}


/*****Nombre***************************************
 * Función importarDatos
 *****Descripción**********************************
 * Lee un archivo JSON que contiene un array de objetos
 * representando ventas, y agrega cada venta a la lista
 * dinámica proporcionada. La función maneja la lectura
 * del archivo, el parseo del contenido JSON y la 
 * adición de ventas a la lista. Reporta errores si no
 * se puede leer o parsear el archivo, y si falta algún
 * atributo en los objetos JSON.
 *****Retorno**************************************
 * 
 ****Entradas************************************** 
 * @param lista: Un puntero al struct `listaVentas` 
 *                donde se agregarán las ventas importadas.
 * @param path: Ruta del archivo JSON que contiene los datos 
 *               de ventas a importar.
 **************************************************/
void importarDatos(listaVentas *lista, const char *path) {
    char *contenido_json = leerArchivo(path);
    if (contenido_json == NULL) {
        printf("Error al leer el archivo JSON.\n");
        return;
    }

    // Verificar si el archivo está vacío
    if (strlen(contenido_json) == 0) {
        // El archivo está vacío, no hay nada que importar
        printf("El archivo JSON está vacío, no hay datos para importar.\n");
        free(contenido_json);
        return;
    }

    cJSON *lista_json = cJSON_Parse(contenido_json);
    if (lista_json == NULL) {
        printf("Error al parsear el archivo JSON.\n");
        free(contenido_json);
        return;
    }

    cJSON *item = NULL;
    int linea = 1;
    cJSON_ArrayForEach(item, lista_json) {
        // Verificar si todos los campos obligatorios están presentes
        if (!cJSON_HasObjectItem(item, "venta_id") ||
            !cJSON_HasObjectItem(item, "fecha") ||
            !cJSON_HasObjectItem(item, "producto_id") ||
            !cJSON_HasObjectItem(item, "producto_nombre") ||
            !cJSON_HasObjectItem(item, "categoria")) {
            reportarAtributosFaltantes(item, linea);
            linea++;
            continue;
        }

        Venta venta;
        venta.venta_id = cJSON_GetObjectItem(item, "venta_id")->valueint;
        venta.fecha = strdup(cJSON_GetObjectItem(item, "fecha")->valuestring);
        venta.producto_id = cJSON_GetObjectItem(item, "producto_id")->valueint;
        venta.producto_nombre = strdup(cJSON_GetObjectItem(item, "producto_nombre")->valuestring);
        venta.categoria = strdup(cJSON_GetObjectItem(item, "categoria")->valuestring);
        venta.cantidad = cJSON_GetObjectItem(item, "cantidad") ? cJSON_GetObjectItem(item, "cantidad")->valueint : 0;
        venta.precio_unitario = cJSON_GetObjectItem(item, "precio_unitario") ? cJSON_GetObjectItem(item, "precio_unitario")->valuedouble : 0.0;
        venta.total = cJSON_GetObjectItem(item, "total") ? cJSON_GetObjectItem(item, "total")->valuedouble : 0.0;

        agregarVenta(lista, venta);
        linea++;
    }

    cJSON_Delete(lista_json);
    free(contenido_json);

    printf("\nDatos importados correctamente.\n");
}

/*****Nombre***************************************
 * Función guardarDatosProcesados
 *****Descripción**********************************
 * Guarda los datos de ventas procesados en un 
 * archivo JSON.
 *****Retorno**************************************
 *
 ****Entradas************************************** 
 * @param lista: Un puntero a una estructura listaVentas que 
 *               contiene los datos de ventas a ser guardados.
 * @param path: Ruta del archivo JSON donde se deben guardar
 *              los datos.
 **************************************************/
void guardarDatosProcesados(listaVentas *lista, const char *path) {
    cJSON *jsonExistente = cJSON_CreateArray();

    // Añadir los datos de ventas procesados
    for (size_t i = 0; i < lista->size; i++) {
        cJSON *ventaJSON = cJSON_CreateObject();
        cJSON_AddNumberToObject(ventaJSON, "venta_id", lista->ventas[i].venta_id);
        cJSON_AddStringToObject(ventaJSON, "fecha", lista->ventas[i].fecha);
        cJSON_AddNumberToObject(ventaJSON, "producto_id", lista->ventas[i].producto_id);
        cJSON_AddStringToObject(ventaJSON, "producto_nombre", lista->ventas[i].producto_nombre);
        cJSON_AddStringToObject(ventaJSON, "categoria", lista->ventas[i].categoria);
        cJSON_AddNumberToObject(ventaJSON, "cantidad", lista->ventas[i].cantidad);
        cJSON_AddNumberToObject(ventaJSON, "precio_unitario", lista->ventas[i].precio_unitario);
        cJSON_AddNumberToObject(ventaJSON, "total", lista->ventas[i].total);

        cJSON_AddItemToArray(jsonExistente, ventaJSON);
    }

    // Convertir el JSON modificado a una cadena
    char *jsonString = cJSON_Print(jsonExistente);
    if (jsonString == NULL) {
        // Manejo de errores: fallo al convertir a cadena
        fprintf(stderr, "Error al convertir JSON a cadena.\n");
        cJSON_Delete(jsonExistente);
        return;
    }

    // Guardar en el archivo (sobrescribir el contenido anterior)
    FILE *archivo = fopen(path, "w");
    if (archivo != NULL) {
        fputs(jsonString, archivo);
        fclose(archivo);
    } else {
        // Manejo de errores: fallo al abrir el archivo
        fprintf(stderr, "Error al abrir el archivo para escritura.\n");
    }

    // Liberar memoria
    free(jsonString);
    cJSON_Delete(jsonExistente);
}

/*****Nombre***************************************
 * Función calcularModa
 *****Descripción**********************************
 * Calcula la moda para una lista de enteros.
 *****Retorno**************************************
 * @return: La moda de los enteros dados.
 ****Entradas************************************** 
 * @param valores: Un arreglo de enteros.
 * @param size: El tamaño del arreglo.
 **************************************************/
int calcularModa(int *valores, size_t size) {
    if (size == 0) return 0;

    int max_count, moda = valores[0];
    for (size_t i = 0; i < size; i++) {
        int count = 0;
        for (size_t j = 0; j < size; j++) {
            if (valores[i] == valores[j]) {
                count++;
            }
        }
        if (count > max_count) {
            max_count = count;
            moda = valores[i];
        }
    }
    return moda;
}

/*****Nombre***************************************
 * Función calcularMedia
 *****Descripción**********************************
 * Calcula la media para una lista de flotantes.
 *****Retorno**************************************
 * @return: La media de los flotantes dados.
 ****Entradas************************************** 
 * @param valores: Un arreglo de flotantes.
 * @param size: El tamaño del arreglo.
 **************************************************/
float calcularMedia(float *valores, size_t size) {
    if (size == 0) return 0.0f;

    float suma = 0.0f;
    for (size_t i = 0; i < size; i++) {
        suma += valores[i];
    }
    return suma / size;
}

/*****Nombre***************************************
 * Función calcularMediana
 *****Descripción**********************************
 * Calcula la mediana para una lista de flotantes.
 *****Retorno**************************************
 * @return: La mediana de los flotantes dados.
 ****Entradas************************************** 
 * @param valores: Un arreglo de flotantes.
 * @param size: El tamaño del arreglo.
 **************************************************/
float calcularMediana(float *valores, size_t size) {
    if (size == 0) return 0.0f;

    for (size_t i = 0; i < size - 1; i++) {
        for (size_t j = i + 1; j < size; j++) {
            if (valores[i] > valores[j]) {
                float temp = valores[i];
                valores[i] = valores[j];
                valores[j] = temp;
            }
        }
    }
    if (size % 2 == 0) {
        return (valores[size / 2 - 1] + valores[size / 2]) / 2.0f;
    } else {
        return valores[size / 2];
    }
}

/*****Nombre***************************************
 * Función completarDatosFaltantes
 *****Descripción**********************************
 * Completa los datos faltantes en la lista de ventas utilizando moda, media o mediana.
 *****Retorno**************************************
 * 
 ****Entradas************************************** 
 * @param lista: Un puntero al struct `listaVentas` 
 *                que contiene las ventas a procesar.
 **************************************************/
void completarDatos(listaVentas *lista) {
    int *cantidades = (int *)malloc(sizeof(int) * lista->size);
    float *precios = (float *)malloc(sizeof(float) * lista->size);
    size_t cantidadCount = 0;
    size_t precioCount = 0;

    // Tomar totales de cantidades y precios
    for (size_t i = 0; i < lista->size; i++) {
        if (lista->ventas[i].cantidad > 0) {
            cantidades[cantidadCount++] = lista->ventas[i].cantidad;
        }
        if (lista->ventas[i].precio_unitario > 0) {
            precios[precioCount++] = lista->ventas[i].precio_unitario;
        }
    }

    // Completar datos faltantes
    for (size_t i = 0; i < lista->size; i++) { 
        if (lista->ventas[i].cantidad <= 0) {
            int modaCantidad = calcularModa(cantidades, cantidadCount);
            lista->ventas[i].cantidad = modaCantidad;
            printf("\nRegistro %d: cantidad reemplazada por moda %d\n", lista->ventas[i].venta_id, modaCantidad);
        }

        if (lista->ventas[i].precio_unitario <= 0) {
            char opcion;
            printf("\nRegistro %d: Precio unitario faltante. Seleccione el método de imputación:\n", lista->ventas[i].venta_id);
            printf("  1. Media\n");
            printf("  2. Mediana\n");
            printf("  Seleccione una opción: ");
            scanf(" %c", &opcion);

            float valor_imputado = 0.0f;
            switch (opcion) {
                case '1':
                    valor_imputado = calcularMedia(precios, precioCount);
                    printf("\nRegistro %d: precio unitario reemplazado por media %.2f\n", lista->ventas[i].venta_id, valor_imputado);
                    break;
                case '2':
                    valor_imputado = calcularMediana(precios, precioCount);
                    printf("\nRegistro %d: precio unitario reemplazado por mediana %.2f\n", lista->ventas[i].venta_id, valor_imputado);
                    break;
                default:
                    printf("\nOpción inválida. Usando media por defecto.\n");
                    valor_imputado = calcularMedia(precios, precioCount);
                    printf("\nRegistro %d: precio unitario reemplazado por media %.2f\n", lista->ventas[i].venta_id, valor_imputado);
                    break;
            }
            lista->ventas[i].precio_unitario = valor_imputado;
        }
    }
    free(cantidades);
    free(precios);
}

/*****Nombre***************************************
 * Función eliminarDatosDuplicados
 *****Descripción**********************************
 * Elimina las ventas duplicadas basándose en el identificador de venta.
 *****Retorno**************************************
 * 
 ****Entradas************************************** 
 * @param lista: Un puntero al struct `listaVentas` 
 *                que contiene las ventas a procesar.
 **************************************************/
void eliminarDatosDuplicados(listaVentas *lista) {
    // Usar un arreglo para marcar si un ID de venta ya ha sido visto
    int *ids_vistos = (int *)malloc(sizeof(int) * lista->size);
    size_t count_ids = 0;
    size_t i = 0;
    
    printf("\n");
    
    while (i < lista->size) {
        int id_actual = lista->ventas[i].venta_id;
        int duplicado = 0;

        for (size_t j = 0; j < count_ids; j++) {
            if (ids_vistos[j] == id_actual) {
                duplicado = 1;
                break;
            }
        }

        if (duplicado) {
            printf("Se eliminó el registro duplicado con venta ID %d\n", id_actual);
            // Desplazar todos los elementos hacia la izquierda
            for (size_t k = i; k < lista->size - 1; k++) {
                lista->ventas[k] = lista->ventas[k + 1];
            }
            lista->size--;
        } else {
            ids_vistos[count_ids++] = id_actual;
            i++;
        }
    }

    free(ids_vistos);
}

/*****Nombre***************************************
 * Función totalVentas
 *****Descripción**********************************
 * Calcula el total de ventas sumando el importe de cada venta.
 *****Retorno**************************************
 * @return: El total de ventas.
 ****Entradas************************************** 
 * @param lista: Un puntero al struct `listaVentas` 
 *                que contiene las ventas a procesar.
 **************************************************/
float totalVentas(listaVentas *lista) {
    // Verificar que la lista no sea nula
    if (lista == NULL) {
        printf("Error: La lista de ventas no está inicializada.\n");
        return 0.0f;
    }

    float total = 0.0f;

    // Iterar sobre las ventas para calcular el total
    for (size_t i = 0; i < lista->size; i++) {
        if (lista->ventas[i].total != 0.0f) {
            // Si el total de la venta ya está calculado, usarlo
            total += lista->ventas[i].total;
        } else {
            // Calcular el total a partir de la cantidad y el precio unitario
            total += lista->ventas[i].cantidad * lista->ventas[i].precio_unitario;
        }
    }

    return total;
}

/*****Nombre***************************************
 * Función totalVentasMensuales
 *****Descripción**********************************
 * Calcula el total de ventas mensuales a partir de la lista de ventas.
 *****Retorno**************************************
 * 
 ****Entradas************************************** 
 * @param lista: Un puntero al struct `listaVentas` que contiene las ventas a procesar.
 * @param meses_totales: Un puntero a un array de cadenas de texto para almacenar los nombres de los meses.
 * @param totales_mensuales: Un puntero a un array de flotantes para almacenar los totales de ventas por mes.
 * @param num_meses: Un puntero a un entero que contendrá la cantidad de meses únicos encontrados.
 **************************************************/
void totalVentasMensuales(listaVentas *lista, char ***meses_totales, float **totales_mensuales, size_t *num_meses) {
    if (lista == NULL) {
        printf("Error: la lista de ventas no está inicializada.\n");
        return;
    }

    const char *nombres_meses[] = {
        "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio",
        "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"
    };

    *meses_totales = NULL;
    *totales_mensuales = NULL;
    *num_meses = 0;

    for (size_t i = 0; i < lista->size; i++) {
        char *fecha = lista->ventas[i].fecha;
        
        // Extraer el año y el mes de la fecha en formato "YYYY-MM-DD"
        char anio[5];
        char mes_numero[3];
        strncpy(anio, fecha, 4);
        anio[4] = '\0';
        strncpy(mes_numero, fecha + 5, 2);
        mes_numero[2] = '\0';

        int mes_index = atoi(mes_numero) - 1; 

        // Construir la cadena con el nombre del mes y el año ("Mes YYYY")
        char mes_nombre[20];
        snprintf(mes_nombre, sizeof(mes_nombre), "%s %s", nombres_meses[mes_index], anio);

        // Verificar si el mes ya está en la lista de meses_totales
        int mes_existente = -1;
        for (size_t j = 0; j < *num_meses; j++) {
            if (strcmp((*meses_totales)[j], mes_nombre) == 0) {
                mes_existente = j;
                break;
            }
        }

        // Si el mes no está en la lista, agregarlo
        if (mes_existente == -1) {
            *meses_totales = (char **)realloc(*meses_totales, (*num_meses + 1) * sizeof(char *));
            (*meses_totales)[*num_meses] = strdup(mes_nombre);

            *totales_mensuales = (float *)realloc(*totales_mensuales, (*num_meses + 1) * sizeof(float));
            (*totales_mensuales)[*num_meses] = 0.0f;

            mes_existente = (*num_meses)++;
        }

        float total = (lista->ventas[i].total != 0.0f) ? lista->ventas[i].total : (lista->ventas[i].cantidad * lista->ventas[i].precio_unitario);
        (*totales_mensuales)[mes_existente] += total;
    }
}


/*****Nombre***************************************
 * Función totalVentasAnuales
 *****Descripción**********************************
 * Calcula el total de ventas anuales a partir de la lista de ventas.
 *****Retorno**************************************
 * 
 ****Entradas************************************** 
 * @param lista: Un puntero al struct `listaVentas` que contiene las ventas a procesar.
 * @param años_totales: Un puntero a un array de cadenas de texto para almacenar los años.
 * @param totales_anuales: Un puntero a un array de flotantes para almacenar los totales de ventas por año.
 * @param num_años: Un puntero a un entero que contendrá la cantidad de años únicos encontrados.
 **************************************************/
void totalVentasAnuales(listaVentas *lista, char ***años_totales, float **totales_anuales, size_t *num_años) {
    if (lista == NULL) {
        printf("Error: La lista de ventas no está inicializada.\n");
        return;
    }

    *años_totales = NULL;
    *totales_anuales = NULL;
    *num_años = 0;

    for (size_t i = 0; i < lista->size; i++) {
        // Obtener el año de la fecha en formato "YYYY"
        char *fecha = lista->ventas[i].fecha;
        char *año = strndup(fecha, 4); 

        // Verificar si el año ya está en la lista de años_totales
        int año_existente = -1;
        for (size_t j = 0; j < *num_años; j++) {
            if (strcmp((*años_totales)[j], año) == 0) {
                año_existente = j;
                break;
            }
        }

        // Si el año no está en la lista, agregarlo
        if (año_existente == -1) {
            *años_totales = (char **)realloc(*años_totales, (*num_años + 1) * sizeof(char *));
            (*años_totales)[*num_años] = año;

            *totales_anuales = (float *)realloc(*totales_anuales, (*num_años + 1) * sizeof(float));
            (*totales_anuales)[*num_años] = 0.0f;

            año_existente = (*num_años)++;
        } else {
            free(año); 
        }

        float total = (lista->ventas[i].total != 0.0f) ? lista->ventas[i].total : (lista->ventas[i].cantidad * lista->ventas[i].precio_unitario);
        (*totales_anuales)[año_existente] += total;
    }
}

/*****Nombre***************************************
 * Función mesConMayorVenta
 *****Descripción**********************************
 * Determina el mes con el mayor total de ventas a 
 * partir de la lista de ventas.
 *****Retorno**************************************
 * Retorna una cadena de texto que representa el mes con 
 * el mayor total de ventas.
 ****Entradas************************************** 
 * @param lista: Un puntero al struct `listaVentas` 
 * que contiene las ventas a procesar.
 **************************************************/
char* mesConMayorVenta(listaVentas *lista) {
    char **meses_totales = NULL;
    float *totales_mensuales = NULL;
    size_t num_meses = 0;
    totalVentasMensuales(lista, &meses_totales, &totales_mensuales, &num_meses);

    if (num_meses == 0) {
        return "No se encontraron ventas registradas.";
    }

    // Encontrar el mes con mayor venta
    int i_mayorVenta = 0;
    for (size_t i = 1; i < num_meses; i++) {
        if (totales_mensuales[i] > totales_mensuales[i_mayorVenta]) {
            i_mayorVenta = i;
        }
    }

    // Formatear el resultado
    static char resultado[50];
    snprintf(resultado, sizeof(resultado), "%s - Total: %.2f", meses_totales[i_mayorVenta], totales_mensuales[i_mayorVenta]);

    // Liberar memoria
    for (size_t i = 0; i < num_meses; i++) {
        free(meses_totales[i]);
    }
    free(meses_totales);
    free(totales_mensuales);

    return resultado;
}

/*****Nombre***************************************
 * Función diaMasActivo
 *****Descripción**********************************
 * Determina el día de la semana más activo (con más 
 * transacciones) a partir de la lista de ventas.
 *****Retorno**************************************
 * Retorna una cadena de texto que representa el día 
 * de la semana con más transacciones.
 ****Entradas************************************** 
 * @param lista: Un puntero al struct `listaVentas` 
 *               que contiene las ventas a procesar.
 **************************************************/
char* diaMasActivo(listaVentas *lista) {
    if (lista == NULL) {
        return "Error: la lista de ventas no está inicializada.";
    }

    const char *nombres_dias[] = {
        "Domingo", "Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado"
    };
    int transacciones_dias[7] = {0}; 

    for (size_t i = 0; i < lista->size; i++) {
        char *fecha = lista->ventas[i].fecha;

        // Extraer año, mes y día
        struct tm tiempo = {0};
        sscanf(fecha, "%4d-%2d-%2d", &tiempo.tm_year, &tiempo.tm_mon, &tiempo.tm_mday);
        tiempo.tm_year -= 1900; // Ajustar para el tipo tm
        tiempo.tm_mon -= 1;     // Ajustar del rango de meses (0-11)

        // Determinar el día de la semana
        mktime(&tiempo); 
        int dia_semana = tiempo.tm_wday; // 'tm_wday' es el día de la semana (0=domingo, 6=sábado)

        transacciones_dias[dia_semana]++;
    }

    // Encontrar el día de la semana con más transacciones
    int dia_mas_activo = 0;
    for (int i = 1; i < 7; i++) {
        if (transacciones_dias[i] > transacciones_dias[dia_mas_activo]) {
            dia_mas_activo = i;
        }
    }

    // Formatear el resultado
    static char resultado[50];
    snprintf(resultado, sizeof(resultado), "%s - Total de transacciones: %d", nombres_dias[dia_mas_activo], transacciones_dias[dia_mas_activo]);

    return resultado;
}

/*****Nombre***************************************
 * Función tasaCrecimientoTrimestral
 *****Descripción**********************************
 * Calcula la tasa de crecimiento o decrecimiento de las 
 * ventas en un trimestre específico en comparación con 
 * el trimestre anterior.
 *****Retorno**************************************
 * Retorna la tasa de crecimiento o decrecimiento en porcentaje.
 ****Entradas************************************** 
 * @param lista: Un puntero al struct `listaVentas` que contiene las ventas a procesar.
 * @param trimestre: Un entero que representa el trimestre a analizar (1, 2, 3 o 4).
 * @param anio: Un entero que representa el año del trimestre a analizar.
 **************************************************/
float tasaCrecimientoTrimestral(listaVentas *lista, int trimestre, int anio) {
    if (lista == NULL) {
        printf("Error: La lista de ventas no está inicializada.\n");
        return 0.0f;
    }

    float total_actual = 0.0f;
    float total_anterior = 0.0f;

    // Definir el rango de meses para el trimestre actual y anterior
    int mes_inicio_actual = (trimestre - 1) * 3 + 1; // Mes de inicio del trimestre actual
    int mes_fin_actual = trimestre * 3;              // Mes de fin del trimestre actual
    int mes_inicio_anterior = mes_inicio_actual - 3; // Mes de inicio del trimestre anterior

    // Calcular totales del trimestre actual y anterior
    for (size_t i = 0; i < lista->size; i++) {
        // Extraer el año y el mes de la fecha
        char *fecha = lista->ventas[i].fecha;
        int anio_venta, mes_venta;
        sscanf(fecha, "%d-%d", &anio_venta, &mes_venta);

        float total = (lista->ventas[i].total != 0.0f) ? lista->ventas[i].total : (lista->ventas[i].cantidad * lista->ventas[i].precio_unitario);

        // Acumular total para el trimestre actual
        if (anio_venta == anio && mes_venta >= mes_inicio_actual && mes_venta <= mes_fin_actual) {
            total_actual += total;
        }
        // Acumular total para el trimestre anterior
        else if (anio_venta == anio && mes_venta >= mes_inicio_anterior && mes_venta < mes_inicio_actual) {
            total_anterior += total;
        }
        // Si el trimestre anterior cruza años (es el primer trimestre)
        else if (anio_venta == anio - 1 && trimestre == 1 && mes_venta >= 10 && mes_venta <= 12) {
            total_anterior += total;
        }
    }

    // Calcular la tasa de crecimiento
    if (total_anterior == 0.0f) {
        printf("No hay datos suficientes para calcular la tasa de crecimiento.\n");
        return 0.0f;
    }

    float tasa_crecimiento = ((total_actual - total_anterior) / total_anterior) * 100.0f;

    printf("Tasa de crecimiento para el trimestre %d del año %d: %.2f%%\n", trimestre, anio, tasa_crecimiento);
    return tasa_crecimiento;
}

/*****Nombre***************************************
 * Función obtenerTopCategorias
 *****Descripción**********************************
 * Calcula las ventas totales por categoría y muestra 
 * el top 5 de categorías con mayores ventas.
 *****Retorno**************************************
 * No retorna valor. Imprime el top 5 de categorías en consola.
 ****Entradas************************************** 
 * @param lista: Un puntero al struct `listaVentas` que contiene las ventas a procesar.
 **************************************************/
void obtenerTopCategorias(listaVentas *lista) {
    if (lista == NULL || lista->size == 0) {
        printf("No hay datos de ventas disponibles.\n");
        return;
    }

    // struct para almacenar las ventas totales por categoría
    CategoriaVenta *categorias = NULL;
    size_t numCategorias = 0;

    // Calcular ventas totales por categoria
    for (size_t i = 0; i < lista->size; i++) {
        char *categoriaActual = lista->ventas[i].categoria;
        float totalVenta = (lista->ventas[i].total != 0.0f) ? lista->ventas[i].total : (lista->ventas[i].cantidad * lista->ventas[i].precio_unitario);

        int categoriaExistente = -1;
        for (size_t j = 0; j < numCategorias; j++) {
            if (strcmp(categorias[j].categoria, categoriaActual) == 0) {
                categoriaExistente = j;
                break;
            }
        }

        // Si la categoria no existe, agregarla
        if (categoriaExistente == -1) {
            categorias = (CategoriaVenta *)realloc(categorias, (numCategorias + 1) * sizeof(CategoriaVenta));
            categorias[numCategorias].categoria = strdup(categoriaActual);
            categorias[numCategorias].totalVentas = totalVenta;
            numCategorias++; 
        } else {
            categorias[categoriaExistente].totalVentas += totalVenta;
        }
    }

    // Aplicar Bubble sort descendente
    for (size_t i = 0; i < numCategorias - 1; i++) {
        for (size_t j = i; j < numCategorias; j++) {
            if (categorias[i].totalVentas < categorias[j].totalVentas) {
                CategoriaVenta temp = categorias[i];
                categorias[i] = categorias[j];
                categorias[j] = temp;  
            }
        }
    }

    // Mostrar el Top 5 de categorías con mayores ventas
    printf("Top 5 de categorías con mayores ventas:\n");
    for (size_t i = 0; i < (numCategorias < 5 ? numCategorias : 5); i++) {
        printf("%zu) %-30s - Total Ventas: %.2f\n", i + 1, categorias[i].categoria, categorias[i].totalVentas);
    }

    // Liberar memoria
    for (size_t i = 0; i < numCategorias; i++) {
        free(categorias[i].categoria);
    }
    free(categorias);
}

#endif //VENTAS_H