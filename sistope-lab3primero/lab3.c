#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "funciones.h"

int n=0,threads = 0, chunk = 0, option, b = 0;  // Variables globales obtenidas mediante flags
char** masterArray;  // Puntero a arreglo bidimensional para almacenar las propiedades de cada punto a leer
FILE *f;              // Declaración del archivo mediante un FILE stream global
pthread_mutex_t mutex; // Declaración del mutex global


void *hebras(void *tid){
  long *myID = (long *)tid; // Id de cada hebra para verificar que el proceso se ejecuta de forma concurrente
  char buffer[256];         // Buffer para almacenar la linea leida del archivo
  while(feof(f) == 0){      // Mientras no se llegue al final de archivo
    pthread_mutex_lock(&mutex); // Se ingresa a la sección crítica
    for(int i = 0; i < chunk; i++){  // Iteracion para leer las líneas de cada chunk
      if(feof(f) != 0){     // Si se llegó al final del archivo mientras se esta leyendo lineas del chunk, se sale del ciclo
        break;
      }
	  fgets(buffer,sizeof(buffer),f); // Se almacena la línea del archivo en buffer
      sscanf(buffer, "%s", buffer); // linea del archivo se almacena en buffer
      char reconoce = recognizer(buffer); // se obtiene el número del chunk al que pertenece
      masterArray[chunk][i] += reconoce; // Se almacena el número del chunk en el arreglo bidimensional
      //No es chunk
	    }
    pthread_mutex_unlock(&mutex); // Se sale de la sección crítica
  }
  pthread_exit(NULL); // La hebra finaliza su ejecución
}

//Acá se usa el getopt para conseguir el nombre del archivo de entrada y de salida.
int main(int argc, char *argv[]){

    //n: Cantidad de workers
    //c: Cantidad de chunks
    //b: Bandera que indica si se debe imprimir el uso del programa

    char *input_file = NULL, *output_file = NULL;

    while((option = getopt(argc, argv, "i:o:h:c:b")) != -1){
        switch (option){
            case 'i'://Nombre del archivo de entrada
                input_file = optarg;
                break;
        
            case 'o': //Nombre del archivo de salida
                output_file = optarg;
                break;

            case 'h': //Cantidad de hebras
                sscanf(optarg, "%d", &threads);
                break;

            case 'c': //Cantidad de chunks
                sscanf(optarg, "%d", &chunk);
                break;

            case 'b': //Flag para imprimir el uso del programa
                b = 1;
                break;
            
            default://En caso de que se ingrese una opción no válida
                abort();
        }
    }
    //Numero de lineas a leer
    n=10;
    //Verificaciones 

    if (threads <= 0) { // Si el numero de hebras es menor o igual a 0
        printf("Cantidad hebras invalida\n");
        return 0;
    }
    
    f = fopen(input_file, "r");  // Se abre el archivo de entrada en modo lectura
    if (f == NULL) {  // Si el archivo no existe.
        printf("No se pudo abrir archivo\n"); // Se imprime mensaje de error
         exit(0);  // Se termina la ejecución
    }

    if(chunk <= 0){
        fprintf(stderr, "El argumento -c es obligatorio y debe ser mayore a 0.\n");
        exit(1);
    }

    masterArray = (char **)malloc(n * sizeof(char *));  // Se reserva memoria para masterArray
    for (int i = 0; i < n; i++) {
         masterArray[i] = (char *)malloc(5 * sizeof(char));  // Se reserva memoria para cada disco
    }
    pthread_t threadsArray[threads];  // Se reserva memoria para el arreglo de hebras
    pthread_mutex_init(&mutex, NULL); // Se inicializa el mutex
    for (int tid = 0; tid < threads; tid++) { // Se crean las hebras
        pthread_create(&threadsArray[tid], NULL, hebras, (void *)&threadsArray[tid]);
    }
    for(int tid = 0; tid < threads;tid++){  // Se espera que cada hebra termine de ejecutarse
        pthread_join(threadsArray[tid], NULL);
    }
    for(int i = 0; i < n; i++){  // Se calcula la media real y media imaginaria de cada disco
        for(int j = 0; j < 2; j++){
            masterArray[i][j] = masterArray[i][j]/masterArray[i][4];  // mediaR y mediaI = sumaR|sumaI/cantidad de puntos por disco
        }
    }
    if(b == 1){ // Si se ingresa la flag b
        imprimirProps(n,masterArray); // Se imprime las propiedades por consola
    }

    escribirSalida(n, output_file, masterArray); // Se escribe el archivo de salida
    free(masterArray);  // Se libera la memoria reservada para masterArray
    pthread_mutex_destroy(&mutex); // Se destruye el mutex
    

}