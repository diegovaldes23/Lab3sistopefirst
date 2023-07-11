#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>


//Funciones

/*
Entrada: Funcion f que contiene la matriz del texto
Salidas: Retorna la cantidad de expresiones regulares y las que no son con sus respectivo booleano [si,no]
Descripción: Recorre cada caracter de la linea y realiza el reconocimiento de la expresion verificando las lineas que son expresiones regulares y las que no 
*/
char *recognizer(char* f);


void writeFile(char* outputFile, int numLines, char** f);

void printSolution(char** f, int numLines);