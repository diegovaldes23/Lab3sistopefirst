#include <string.h>
#include <stdbool.h>
#include "funciones.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 10000

void readFile(char *input_file){

    FILE *archive;
    char line[MAX_LINE_LENGTH];
    char **matrix;
    int i, num_lines = 0;

    archive = fopen(input_file,"r");
    if (archive == NULL) {
        printf("No se pudo abrir el archivo.\n");
        exit(1);
    }

    while (fgets(line, MAX_LINE_LENGTH, archive) != NULL) {
        num_lines++;
    }

    matrix = (char **)malloc(num_lines * sizeof(char *));
    rewind(archive);

    for (i = 0; i < num_lines; i++) {
        fgets(line, MAX_LINE_LENGTH, archive);
        matrix[i] = (char *)malloc((strlen(line) + 1) * sizeof(char));
        strcpy(matrix[i], line);
    }

    fclose(archive);
}

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

void writeFile(char *outputFile, char** f){

    FILE *archive;
    int i;

    //Se crea el archivo
    archive = fopen(outputFile,"w");

    if(archive == NULL) {
        printf("Error al abrir el archivo.\n");
        exit(1);
        }
    fprintf(archive, "Resultado \n");
    fclose(archive); // Cierra el archivo

    //Se modifica 
    archive = fopen(outputFile,"a");

    if(archive == NULL) {
        printf("Error al abrir el archivo.\n");
        exit(1);
    }

    fclose(archive); // Cierra el archivo

}