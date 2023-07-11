#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include "funciones.h"

//Variables globales obtenidas mediante consola
int threads = 0, chunkSize = 0, option, b = 0, chunk;
int* booleans;

//Puntero a matriz para almacenar los caracteres del archivo a leer
char** matrix;

//Declaración del archivo de forma global
FILE *f;

//Declaración del mutex global
pthread_mutex_t mutex;
pthread_mutex_t mutex2;

/*
Entrada: ID de la hebra
Salida: 
Descripción: 
*/
void *hebra(void *id){
    //ID de la hebra
    long *myID = (long *)id;

    //Arreglo para almacenar cada línea leída
    char buffer[256];
    //Mientras no se llegue al final del archivo, se leen las líneas
    pthread_mutex_lock(&mutex2);
    int j = 0;
    while(feof(f) == 0){
        //Entrando a sección crítica
        pthread_mutex_lock(&mutex);
        //Se lee el archivo seccionándolo en chunks
        for(int i = 0; i < chunk; i++){  // Iteracion para leer las líneas de cada chunk
            //Si se llega al final del archivo, se sale del ciclo
            if(feof(f) != 0){
                break;
            }

            //Se guarda la línea del archivo en el buffer
            fgets(buffer, sizeof(buffer), f);
            sscanf(buffer, "%s", buffer);

            //Se obtiene la línea del archivo y si es una expresión regular o no aceptada
            //por el autómata
            char* reconoce = recognizer(buffer);

            //Se imprime el id de la hebra y la línea del archivo con su respectivo booleano
            printf("Hebra %ld: %s\n", *myID, reconoce);
            
            //Si el final del reconocedor es una i, se almacena un 1 en el arreglo de booleanos
            if(reconoce[strlen(reconoce)-2] == 'i'){
                booleans[j] = 1;
            }else if(reconoce[strlen(reconoce)-2] == 'o'){
                booleans[j] = 0;
            }

            //Si el final del reconocedor es una o, se almacena un 0 en el arreglo de booleanos


            //Se almacena en la matriz la línea del archivo
            for(int k = 0; k < strlen(reconoce); k++){
                matrix[j][k] = reconoce[k];
            }
        }
        j++;
        //Saliendo de sección crítica
        pthread_mutex_unlock(&mutex);
    }
    pthread_mutex_unlock(&mutex2);
    //Se termina la ejecución de la hebra
    pthread_exit(NULL);
}

/*
Entrada: Nombre de archivo de entrada, nombre de archivo de salida, 
         cantidad de hebras, tamaño de cada chunk y bandera para imprimir uso del programa.
Salida: Archivo de salida con las expresiones regulares y no regulares.
Descripción: Función principal del programa.
*/
int main(int argc, char *argv[]){
    char *inputFile = NULL, *outputFile = NULL;

    while((option = getopt(argc, argv, "i:o:h:c:b")) != -1){
        switch (option){
            //Nombre del archivo de entrada
            case 'i':
                inputFile = optarg;
                break;
            //Nombre del archivo de salida
            case 'o': 
                outputFile = optarg;
                break;

            //Cantidad de hebras
            case 'h': 
                sscanf(optarg, "%d", &threads);
                break;

            //Tamaño de cada chunk
            case 'c':
                sscanf(optarg, "%d", &chunkSize);
                break;

            //Flag para imprimir el uso del programa
            case 'b':
                b = 1;
                break;
            
            //Si se ingresa una opción no válida, se aborta la ejecución
            default:
                abort();
        }
    }

    //Verificaciones de argumentos de entrada 
    if(threads <= 0){ // Si el numero de hebras es menor o igual a 0
        printf("Cantidad hebras inválida. Deben ser mayores a 0.\n");
        return 0;
    }
    
    //Se abre el archivo de entrada en modo lectura
    f = fopen(inputFile, "r");
    //Si el archivo no existe, se imprime mensaje de error y se termina la ejecución
    if(f == NULL){
        printf("No se pudo abrir archivo.\n"); 
        exit(0);
    }

    //Si el tamaño del chunk es menor o igual a 0, se imprime mensaje de error y se termina la ejecución
    if(chunkSize <= 0){
        fprintf(stderr, "El argumento -c es obligatorio y debe ser mayor a 0.\n");
        exit(1);
    }

    //Se obtiene el número de líneas del archivo
    int numLines = 0;
    FILE *f2 = fopen(inputFile, "r");

    while(!feof(f2)){
        char ch = fgetc(f2);
        if(ch == '\n'){
            numLines++;
        }
    }

    numLines++;

    //Se calcula el número de chunks que se deben leer del archivo
    chunk = (numLines/chunkSize);

    //Se almacena memoria para la matriz.
    matrix = (char **)malloc(numLines * sizeof(char *));
    for(int i = 0; i < numLines; i++){
        matrix[i] = (char *)malloc(63 * sizeof(char));
    }

    //Se almacena memoria para el arreglo de booleanos
    booleans = (int *)malloc(numLines * sizeof(int));

    //Se almacena memoria para el arreglo de hebras
    pthread_t threadsArray[threads]; 

    //Se inicializa el mutex 
    pthread_mutex_init(&mutex, NULL);

    //Se crean las hebras, las cuales ejecutarán la función hebra y se 
    //les pasa como argumento el id de cada hebra
    for(int i = 0; i < threads; i++){
        pthread_create(&threadsArray[i], NULL, hebra, (void *)&threadsArray[i]);
    }

    //Todas las hebras deben terminar su ejecución, por lo que se espera a que esto ocurra.
    for(int i = 0; i < threads; i++){
        pthread_join(threadsArray[i], NULL);
    }

    //Se imprimen los booleanos
    for(int i = 0; i < numLines; i++){
        printf("%d ", booleans[i]);
    }

    printf("\n");

    //Si b es verdadero, se imprimen los resultados por consola
    if(b == 1){
        printSolution(matrix, numLines);
    }

    //Se escribe en el archivo de salida
    writeFile(outputFile, numLines, matrix);
    free(matrix);
    //Se destruye el mutex
    pthread_mutex_destroy(&mutex);
}