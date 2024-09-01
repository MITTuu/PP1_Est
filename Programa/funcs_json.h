#ifndef FUNCS_JSON_H
#define FUNCS_JSON_H

/*****Datos administrativos************************
 * Nombre del archivo: funcs json
 * Tipo de archivo: C Encabezado
 * Proyecto: Sistema de Análisis de Datos de Ventas
 * Autor: Dylan Montiel Zúñiga
 *****Descripción**********************************
 * Este archivo contiene la declaración de funciones y tipos 
 * necesarios para la manipulación y análisis de datos en formato 
 * JSON. Incluye funciones para leer archivos JSON y procesar 
 * su contenido, facilitando la integración de datos en el sistema 
 * de análisis de ventas.
 *****Versión**************************************
 * 1.0 | 08/30/2024 | Dylan Montiel Zúñiga
 **************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <cjson/cJSON.h>

/*****Nombre***************************************
 * Función leerArchivo
 *****Descripción**********************************
 * Lee el contenido de un archivo de texto y lo devuelve 
 * como una cadena de caracteres. La memoria para la cadena 
 * se asigna dinámicamente y debe ser liberada por el 
 * llamador de la función.
 *****Retorno**************************************
 * @return: Un puntero a una cadena de caracteres que 
 *          contiene el contenido del archivo, o NULL 
 *          si el archivo no puede ser abierto.
 ****Entradas************************************** 
 * @param path: Ruta del archivo que se desea leer.
 **************************************************/
char* leerArchivo(const char *path) {
    FILE *archivo = fopen(path, "r");
    
    if (archivo == NULL) {
        return NULL;
    }

    fseek(archivo, 0, SEEK_END);
    long len = ftell(archivo);
    fseek(archivo, 0, SEEK_SET);

    char *contenido = (char *)malloc(len + 1);
    if (contenido) {
        fread(contenido, 1, len, archivo);
        contenido[len] = '\0';
    }

    fclose(archivo);

    return contenido;
}

#endif // FUNCS_JSON_H
