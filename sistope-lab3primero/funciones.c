#include <string.h>
#include <stdbool.h>
#include "funciones.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 10000

char *recognizer(char* f){

    int state = 1;

    for (int j = 0; j < strlen(f); j++) {
        char c = f[j];

        switch (state) {
            case 1:
                if(c == 'A' || c == 'C' || c == 'T'){
                    state = 1;
                }else if(c == 'G'){
                    state = 2;
                }else{
                    state = -1;
                }
                break;

            case 2:
                if(c == 'G'){
                    state = 2;
                }else if(c == 'A' || c == 'C'){
                    state = 1;
                }else if(c == 'T'){
                    state = 3;
                }else{
                    state = -1;
                }
                break;

            case 3:
                if(c == 'A'){
                    state = 1;
                }else if(c == 'G'){
                    state = 2;
                }else if(c == 'C'){
                    state = 4;
                }else if(c == 'T'){
                    state = 3;
                }else{
                    state = -1;
                }
                break;

            case 4:
                if(c == 'A' || c == 'C' || c == 'G' || c == 'T'){
                    state = 4;
                }else{
                    state = -1;
                }
                break;

            default:
                break;
        }
    }

    if(state == 4){
        strcat(f, " si\n");
    }else{
        strcat(f, " no\n");
    }
    
    return f;
}

void writeFile(char *outputFile, int numLines, char** f, int* booleans){
    FILE *archive;
    int i;

    //Se crea el archivo de salida con el nombre ingresado por consola y se abre en modo escritura
    archive = fopen(outputFile,"w");

    //Si no se pudo crear el archivo, se imprime mensaje de error y se termina la ejecución
    if(archive == NULL) {
        printf("Error al abrir el archivo.\n");
        exit(1);
    }

    //Se cierra el archivo de salida    
    fclose(archive);

    //Se modifica el archivo de salida con la información de las expresiones regulares y no regulares
    archive = fopen(outputFile, "a");

    //Si no se pudo abrir el archivo, se imprime mensaje de error y se termina la ejecución
    if(archive == NULL) {
        printf("Error al abrir el archivo.\n");
        exit(1);
    }

    //Se recorre la matriz de expresiones regulares y no regulares
    for(int i = 0; i < numLines; i++){
        for(int j = 0; j < strlen(f[i]); j++){
            //Se escribe en el archivo de salida
            fprintf(archive, "%c", f[i][j]);
        }
    }

    int expresionesRegulares = 0;
    int expresionesNoRegulares = 0;
    for(int i = 0; i < numLines; i++){
        if(booleans[i] == 1){
            expresionesRegulares++;
        }else{
            expresionesNoRegulares++;
        }
    }
    fprintf(archive, "\n");

    fprintf(archive, "Total de expresiones que Si son regulares: %d\n", expresionesRegulares);
    fprintf(archive, "Total de expresiones que No son regulares: %d\n", expresionesNoRegulares);
    fprintf(archive, "Total de lineas leídas: %d\n", numLines);

    fclose(archive); // Cierra el archivo

}

void printSolution(char** f, int numLines){
    for(int i = 0; i < numLines; i++){
        printf("%s", f[i]);
    }
}