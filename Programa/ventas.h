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
 * 3.0 | 08/30/2024 | Dylan Montiel Zúñiga
 **************************************************/

#include <stdlib.h>
#include <string.h>
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
        return NULL;
    }
    
    // Asignar memoria para el struct Venta
    lista->ventas = (Venta *)malloc(sizeof(Venta) *10);
    if (lista->ventas == NULL) {
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
void agregarVenta(listaVentas *lista, Venta venta) {
    if (lista->size == lista->capacity) {
        lista->capacity *= 2;
        lista->ventas = (Venta *)realloc(lista->ventas, sizeof(Venta) * lista->capacity);
    }
    lista->ventas[lista->size++] = venta;
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
        for (size_t i = 0; i < lista->size; i++) {
            free(lista->ventas[i].fecha);
            free(lista->ventas[i].producto_nombre);
            free(lista->ventas[i].categoria);
        }
        free(lista->ventas);
        free(lista);
    }
}

/*****Nombre***************************************
 * Función reportarAtributosFaltantes
 *****Descripción**********************************
 * Esta función verifica los atributos requeridos en un objeto JSON
 * que representa una venta. 
 *****Retorno**************************************
 * 
 ****Entradas**************************************
 * @param item: Un puntero al objeto `cJSON` que representa una venta 
 *               y contiene los atributos a verificar.
 * @param linea: El número de línea en el archivo JSON donde se 
 *                encuentra el objeto `item`, usado para el mensaje 
 *                de error si faltan atributos.
 **************************************************/
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

#endif //VENTAS_H