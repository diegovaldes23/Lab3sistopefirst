#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>


//Funciones

/*
Entrada: Arreglo de caracteres.
Salidas: Arreglo de caracteres y si es una expresión regular o no aceptada por el autómata.
Descripción: Función que determina si una cadena de caracteres es una expresión regular o 
no aceptada por el autómata.
*/
char* recognizer(char* f);

/*
Entrada: Archivo de salida, número de líneas, arreglo de líneas y arreglo de booleanos que
indican si la línea es una expresión regular o no aceptada por el autómata.
Salidas: Archivo de salida con las líneas que son expresiones regulares o no aceptadas por el autómata 
y el total de expresiones regulares con el formato especificado en el enunciado.
Descripción: Función que escribe en el archivo de salida las líneas que son expresiones regulares 
o no aceptadas por el autómata y el total de expresiones regulares con el formato especificado en el enunciado.
*/
void writeFile(char* outputFile, int numLines, char** f, int* booleans);

/*
Entrada: Arreglo de líneas, número de líneas y arreglo de booleanos que
indican si la línea es una expresión regular o no aceptada por el autómata.
Salidas: Impresión en consola de las líneas que son expresiones regulares o no aceptadas por el autómata
y el total de expresiones regulares con el formato especificado en el enunciado.
Descripción: Función que imprime en consola las líneas que son expresiones regulares
o no aceptadas por el autómata y el total de expresiones regulares con el 
formato especificado en el enunciado.
*/
void printSolution(char** f, int numLines, int* booleans);