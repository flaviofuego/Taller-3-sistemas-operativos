#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_LINE 1024

int main() {
    FILE *file;
    char line[MAX_LINE];
    int num_secciones, i, j;
    float **notas;
    int *num_estudiantes;
    
    // Abrir el archivo de notas
    file = fopen("notas.txt", "r");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        return 1;
    }
    
    // Leer el número de secciones
    fgets(line, MAX_LINE, file);
    num_secciones = atoi(line);
    
    // Asignar memoria dinámica para almacenar el número de estudiantes por sección
    num_estudiantes = (int *)calloc(num_secciones, sizeof(int));
    if (num_estudiantes == NULL) {
        perror("Error al asignar memoria");
        fclose(file);
        return 1;
    }
    
    // Asignar memoria para el arreglo de punteros a las notas de cada sección
    notas = (float **)calloc(num_secciones, sizeof(float *));
    if (notas == NULL) {
        perror("Error al asignar memoria");
        free(num_estudiantes);
        fclose(file);
        return 1;
    }
    
    // Leer la información de cada sección
    for (i = 0; i < num_secciones; i++) {
        // Leer número de estudiantes en esta sección
        fgets(line, MAX_LINE, file);
        *(num_estudiantes + i) = atoi(line);
        
        // Asignar memoria para las notas de esta sección
        *(notas + i) = (float *)calloc(*(num_estudiantes + i), sizeof(float));
        if (*(notas + i) == NULL) {
            perror("Error al asignar memoria");
            // Liberar la memoria ya asignada
            for (j = 0; j < i; j++) {
                free(*(notas + j));
            }
            free(notas);
            free(num_estudiantes);
            fclose(file);
            return 1;
        }
        
        // Leer las notas de esta sección
        fgets(line, MAX_LINE, file);
        char *token = strtok(line, " \n");
        j = 0;
        while (token != NULL && j < *(num_estudiantes + i)) {
            *(*(notas + i) + j) = atof(token);
            token = strtok(NULL, " \n");
            j++;
        }
    }
    
    fclose(file);
    
    // Crear pipe para cada sección (comunicación hijo -> padre)
    int pipes[num_secciones][2];
    
    for (i = 0; i < num_secciones; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("Error al crear pipe");
            // Liberar memoria y salir
            for (j = 0; j < num_secciones; j++) {
                free(*(notas + j));
            }
            free(notas);
            free(num_estudiantes);
            return 1;
        }
    }
    
    // Crear un proceso hijo para cada sección
    pid_t pid;
    float promedios[num_secciones];
    
    for (i = 0; i < num_secciones; i++) {
        pid = fork();
        
        if (pid < 0) {
            perror("Error al crear proceso hijo");
            return 1;
        } else if (pid == 0) {
            // Proceso hijo
            // Cerrar extremos no usados de pipes
            for (j = 0; j < num_secciones; j++) {
                if (j != i) {
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                } else {
                    close(pipes[j][0]);  // El hijo no lee de este pipe
                }
            }
            
            // Imprimir las notas recibidas
            printf("--- Sección %d ---\n", i + 1);
            printf("Notas: ");
            float suma = 0.0;
            for (j = 0; j < *(num_estudiantes + i); j++) {
                printf("%.2f ", *(*(notas + i) + j));
                suma += *(*(notas + i) + j);
            }
            printf("\n");
            
            // Calcular promedio
            float promedio = suma / *(num_estudiantes + i);
            
            // Enviar el promedio al padre
            write(pipes[i][1], &promedio, sizeof(float));
            close(pipes[i][1]);
            
            // Liberar memoria asignada
            for (j = 0; j < num_secciones; j++) {
                if (*(notas + j) != NULL) {
                    free(*(notas + j));
                }
            }
            free(notas);
            free(num_estudiantes);
            
            exit(0);  // Terminar proceso hijo
        }
    }
    
    // Proceso padre
    
    // Cerrar extremos de escritura de pipes
    for (i = 0; i < num_secciones; i++) {
        close(pipes[i][1]);
    }
    
    // Esperar a todos los hijos
    for (i = 0; i < num_secciones; i++) {
        wait(NULL);
    }
    
    // Leer los promedios enviados por los hijos
    float promedio_general = 0.0;
    int mejor_seccion = 0;
    float mejor_promedio = 0.0;
    
    for (i = 0; i < num_secciones; i++) {
        read(pipes[i][0], &promedios[i], sizeof(float));
        close(pipes[i][0]);
        
        promedio_general += promedios[i];
        
        if (i == 0 || promedios[i] > mejor_promedio) {
            mejor_promedio = promedios[i];
            mejor_seccion = i;
        }
    }
    
    promedio_general /= num_secciones;
    
    // Mostrar resultados
    for (i = 0; i < num_secciones; i++) {
        printf("Promedio sección %d: %.2f\n", i + 1, promedios[i]);
    }
    
    printf("Promedio general: %.2f\n", promedio_general);
    printf("Mejor sección: %d con promedio %.2f\n", mejor_seccion + 1, mejor_promedio);
    
    // Liberar memoria
    for (i = 0; i < num_secciones; i++) {
        free(*(notas + i));
    }
    free(notas);
    free(num_estudiantes);
    
    return 0;
}