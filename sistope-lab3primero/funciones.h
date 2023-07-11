#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>


//Funciones

/*
Entrada:Archivo txt a leer
Salidas: Se guarda la matriz del texto
Descripción: Recorre cada caracter de la linea y la guarda en una matriz*/
void readFile(char* input_file);

/*
Entrada:Funcion f que contiene la matriz del texto
Salidas: Retorna la cantidad de expresiones regulares y las que no son con sus respectivo booleano [si,no]
Descripción: Recorre cada caracter de la linea y realiza el reconocimiento de la expresion verificando las lineas que son expresiones regulares y las que no 
 */
char *recognizer(char* f);

/*
Entrada: Un archivo txt donde guardar la info y Funcion f .
Salidas: Retorna la cantidad de expresiones regulares y las que no son con sus respectivo booleano [si,no] en el txt salida
Descripcion: Recorre cada la linea guardandolas en el txt con su respectivo [si,no] y se imprimen 
las cantidades de las que son expresiones regulares y las que no */
void writeFile(char* outputFile, char** f);
