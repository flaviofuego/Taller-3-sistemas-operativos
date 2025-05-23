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
    
    file = fopen("notas.txt", "r");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        return 1;
    }
    
    fgets(line, MAX_LINE, file);
    num_secciones = atoi(line);
    
    num_estudiantes = (int *)calloc(num_secciones, sizeof(int));
    if (num_estudiantes == NULL) {
        perror("Error al asignar memoria");
        fclose(file);
        return 1;
    }
    
    notas = (float **)calloc(num_secciones, sizeof(float *));
    if (notas == NULL) {
        perror("Error al asignar memoria");
        free(num_estudiantes);
        fclose(file);
        return 1;
    }
    
    for (i = 0; i < num_secciones; i++) {
        fgets(line, MAX_LINE, file);
        num_estudiantes[i] = atoi(line);
        
        notas[i] = (float *)calloc(num_estudiantes[i], sizeof(float));
        if (notas[i] == NULL) {
            perror("Error al asignar memoria");
            for (j = 0; j < i; j++) {
                free(notas[j]);
            }
            free(notas);
            free(num_estudiantes);
            fclose(file);
            return 1;
        }
        
        fgets(line, MAX_LINE, file);
        char *token = strtok(line, " \n");
        j = 0;
        while (token != NULL && j < num_estudiantes[i]) {
            notas[i][j] = atof(token);
            token = strtok(NULL, " \n");
            j++;
        }
    }
    
    fclose(file);
    
    int pipes_to_child[num_secciones][2]; 
    int pipes_from_child[num_secciones][2]; 
    
    for (i = 0; i < num_secciones; i++) {
        if (pipe(pipes_to_child[i]) == -1 || pipe(pipes_from_child[i]) == -1) {
            perror("Error al crear pipe");
            for (j = 0; j < num_secciones; j++) {
                free(notas[j]);
            }
            free(notas);
            free(num_estudiantes);
            return 1;
        }
    }
    
    pid_t pids[num_secciones]; 
    float promedios[num_secciones];
    
    for (i = 0; i < num_secciones; i++) {
        pids[i] = fork();
        
        if (pids[i] < 0) {
            perror("Error al crear proceso hijo");
            return 1;
        } else if (pids[i] == 0) {

            for (j = 0; j < num_secciones; j++) {
                if (j != i) {
                    close(pipes_to_child[j][0]);
                    close(pipes_to_child[j][1]);
                    close(pipes_from_child[j][0]);
                    close(pipes_from_child[j][1]);
                } else {
                    close(pipes_to_child[j][1]);
                    close(pipes_from_child[j][0]);
                }
            }
            
            int num_est;
            read(pipes_to_child[i][0], &num_est, sizeof(int));
            
            float *notas_seccion = (float *)calloc(num_est, sizeof(float));
            read(pipes_to_child[i][0], notas_seccion, num_est * sizeof(float));
            
            close(pipes_to_child[i][0]);
            
            printf("--- Sección %d ---\n", i + 1);
            printf("Notas: ");
            float suma = 0.0;
            for (j = 0; j < num_est; j++) {
                printf("%.2f ", notas_seccion[j]);
                suma += notas_seccion[j];
            }
            printf("\n");
            
            float promedio = suma / num_est;
            
            write(pipes_from_child[i][1], &promedio, sizeof(float));
            close(pipes_from_child[i][1]);
            
            free(notas_seccion);
            
            exit(0);
        }
    }
    
    for (i = 0; i < num_secciones; i++) {
        close(pipes_to_child[i][0]); 
        close(pipes_from_child[i][1]); 
    }
    
    for (i = 0; i < num_secciones; i++) {
        write(pipes_to_child[i][1], &num_estudiantes[i], sizeof(int));
        write(pipes_to_child[i][1], notas[i], num_estudiantes[i] * sizeof(float));
        close(pipes_to_child[i][1]);
        
        int status;
        waitpid(pids[i], &status, 0);
        
        read(pipes_from_child[i][0], &promedios[i], sizeof(float));
        close(pipes_from_child[i][0]);
    }
    
    float promedio_general = 0.0;
    int mejor_seccion = 0;
    float mejor_promedio = 0.0;
    
    for (i = 0; i < num_secciones; i++) {
        promedio_general += promedios[i];
        
        if (i == 0 || promedios[i] > mejor_promedio) {
            mejor_promedio = promedios[i];
            mejor_seccion = i;
        }
    }
    
    promedio_general /= num_secciones;
    
    for (i = 0; i < num_secciones; i++) {
        printf("Promedio sección %d: %.2f\n", i + 1, promedios[i]);
    }
    
    printf("Promedio general: %.2f\n", promedio_general);
    printf("Mejor sección: %d con promedio %.2f\n", mejor_seccion + 1, mejor_promedio);
    
    for (i = 0; i < num_secciones; i++) {
        free(notas[i]);
    }
    free(notas);
    free(num_estudiantes);
    
    return 0;
}