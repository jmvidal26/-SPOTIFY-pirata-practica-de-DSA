#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "spotify.h"


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s archivo.txt\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        printf("No se pudo abrir el archivo.\n");
        return 1;
    }

    char linea[1024];
    ListaCanciones misPlaylists[20]; // Arreglo de estructuras
    int totalListas = 0;

    while (fgets(linea, sizeof(linea), f)) {
        // Ignorar líneas vacías
        if (strlen(linea) <= 1) continue;

        // Buscar el separador de nombre de lista
        char *ptrDosPuntos = strchr(linea, ':');
        if (ptrDosPuntos != NULL) {
            *ptrDosPuntos = '\0'; // Se corta la cadena para obtener el nombre
            char *nombreLista = linea;
            char *canciones = ptrDosPuntos + 1;

            // Inicializamos la estructura ListaCanciones en el arreglo
            inicializarLista(&misPlaylists[totalListas], nombreLista);

            // Procesamos las canciones de esa línea
            char *token = strtok(canciones, "-\n\r");
            while (token != NULL) {
                // Limpiar espacios
                while (*token == ' ') token++;
                char *end = token + strlen(token) - 1;
                while (end > token && *end == ' ') { *end = '\0'; end--; }

                if (strlen(token) > 0) {
                    SongNode *nuevo = crearNodoCancion(token);
                    // Insertar al final de la lista actual
                    if (misPlaylists[totalListas].inicio == NULL) {
                        misPlaylists[totalListas].inicio = nuevo;
                    } else {
                        SongNode *aux = misPlaylists[totalListas].inicio;
                        while (aux->next != NULL) aux = aux->next;
                        aux->next = nuevo;
                        nuevo->prev = aux;
                    }
                }
                token = strtok(NULL, "-\n\r");
            }
            if (totalListas < 20) {
                totalListas++;
            } else {
                printf("Demasiadas listas, ignorando '%s'.\n", nombreLista);
            }
        }
    }
    fclose(f);

    // Funcion provisional para probar Impresion y correcta carga de datos
    for(int i = 0; i < totalListas; i++) {
        printf("Lista: %s\n", misPlaylists[i].nombreLista);
        SongNode *curr = misPlaylists[i].inicio;
        while(curr) {
            printf("  - %s\n", curr->nombre);
            curr = curr->next;
        }
    }

    Reproductor miApp;
    miApp.actual = NULL;
    miApp.colaReproduccion = Newcola(); // Función en spotifycola.c
    miApp.historial = newStack();       // Función en spotifypila.c
    miApp.loopActivo = 0;
    miApp.shuffleActivo = 0;

    char bufferComando[150];
    
    printf("\n--- Spotify Pirata ---\n");
    srand(time(NULL));
    while (1) {

        printf("\n>> ");
        fgets(bufferComando, sizeof(bufferComando), stdin);
        bufferComando[strcspn(bufferComando, "\n")] = 0; // Limpiar el Enter

        if (strcmp(bufferComando, "exit") == 0) break;

        //Funcion que unifica next con play//

        if (strcmp(bufferComando, "next") == 0 || strcmp(bufferComando, "play") == 0) {
            char nombreSiguiente[100];
    
            // --- LÓGICA DEL LOOP --- //
            if (miApp.loopActivo && miApp.actual != NULL) {
                printf("\n>>> REPRODUCIENDO (Loop): %s\n", miApp.actual->nombre);
                continue; // El 'continue' hace que ignore el Descolar y vuelva al inicio del while
            }
            if (Descolar(miApp.colaReproduccion, nombreSiguiente)) {
                if (miApp.actual != NULL) Push(miApp.historial, miApp.actual->nombre);

                int encontrada = 0;
                for (int i = 0; i < totalListas; i++) { // BUSQUEDA GLOBAL
                    SongNode *aux = misPlaylists[i].inicio;
                    while (aux != NULL) {
                        if (strcmp(aux->nombre, nombreSiguiente) == 0) {
                            miApp.actual = aux;
                            encontrada = 1; break;
                        }
                        aux = aux->next;
                    }
                    if (encontrada) break;
                }
                printf("\n>>> REPRODUCIENDO: %s\n", miApp.actual->nombre);
            } else {
                printf("La cola está vacía.\n");
            }
        }

        // Comando Play con lista (Por ejemplo: play Favoritos) //
        else if (strncmp(bufferComando, "play ", 5) == 0) {
            char *nombreListaBuscada = bufferComando + 5;
            int listaOk = 0;

            for (int i = 0; i < totalListas; i++) {
                if (strcmp(misPlaylists[i].nombreLista, nombreListaBuscada) == 0) {
                    SongNode *aux = misPlaylists[i].inicio;
                    while (aux != NULL) {
                        Acolar(miApp.colaReproduccion, aux->nombre);
                        aux = aux->next;
                    }
                    printf("Lista '%s' cargada en la cola.\n", nombreListaBuscada);
                    listaOk = 1;
                    break;
                }
            }
            if (!listaOk) printf("Error: Lista no encontrada.\n");
        }

        // Función "add to queue" //

        if (strncmp(bufferComando, "add to queue ", 13) == 0) {
            char *nombreCancion = bufferComando + 13;
            int encontrada = 0;

            // SE RECORREN TODAS LAS LISTAS DISPONIBLES
            for (int i = 0; i < totalListas; i++) {
                SongNode *aux = misPlaylists[i].inicio;
                
                while (aux != NULL) {
                    if (strcmp(aux->nombre, nombreCancion) == 0) {
                        Acolar(miApp.colaReproduccion, aux->nombre);
                        printf("'%s' encontrada en la lista [%s] y añadida a la cola.\n", 
                                aux->nombre, misPlaylists[i].nombreLista);
                        encontrada = 1;
                        break; 
                    }
                    aux = aux->next;
                }
                if (encontrada) break; // Si ya la hallamos en una lista, dejamos de buscar en las demás
            }

            if (!encontrada) {
                printf("Error: La canción '%s' no existe en ninguna lista.\n", nombreCancion);
            }
        }

        
        //Función back//

        if (strcmp(bufferComando, "back") == 0) {
            char cancionPrevia[100];
            if (Pop(miApp.historial, cancionPrevia)) {
                int encontrada = 0;
                for (int i = 0; i < totalListas; i++) { // BUSQUEDA GLOBAL
                    SongNode *aux = misPlaylists[i].inicio;
                    while (aux != NULL) {
                        if (strcmp(aux->nombre, cancionPrevia) == 0) {
                            miApp.actual = aux;
                            encontrada = 1; break;
                        }
                        aux = aux->next;
                    }
                    if (encontrada) break;
                }
                printf("<<< Volviendo a: %s\n", miApp.actual->nombre);
            } else {
                printf("Historial vacío.\n");
            }
        }

        //Función Shuffle//

        if (strcmp(bufferComando, "shuffle") == 0) {
            if (VaciaCola(miApp.colaReproduccion)) {
                printf("La cola está vacía, no hay nada que barajar.\n");
            } else {
                //  Extraer todas las canciones a un arreglo temporal
                char temp[100][100];
                int n = 0;
                char nombreExtraido[100];
                while (Descolar(miApp.colaReproduccion, nombreExtraido) && n < 100) {
                    strcpy(temp[n], nombreExtraido);
                    n++;
                }
                if (n == 100 && !VaciaCola(miApp.colaReproduccion)) {
                    // Hay más de 100, re-encolar las 100 extraídas y error
                    for (int i = 0; i < 100; i++) {
                        Acolar(miApp.colaReproduccion, temp[i]);
                    }
                    printf("Demasiadas canciones en cola para barajar (máx 100).\n");
                } else {
                    //  Algoritmo de Fisher-Yates para barajar
                    for (int i = n - 1; i > 0; i--) {
                        int j = rand() % (i + 1);
                        char aux[100];
                        strcpy(aux, temp[i]);
                        strcpy(temp[i], temp[j]);
                        strcpy(temp[j], aux);
                    }
                    // Volver a meter las canciones barajadas en la cola
                    for (int i = 0; i < n; i++) {
                        Acolar(miApp.colaReproduccion, temp[i]);
                    }
                    printf("¡Cola de reproducción barajada aleatoriamente!\n");
                }
            }
        }

        //Función Loop//

        if (strcmp(bufferComando, "loop") == 0) {
            miApp.loopActivo = !miApp.loopActivo; // Cambia entre 0 y 1
            printf("Loop %s\n", miApp.loopActivo ? "ACTIVADO" : "DESACTIVADO");
        }

        // Función Limpiar Cola//
        if (strcmp(bufferComando, "clear queue") == 0) {
            LimpiarCola(miApp.colaReproduccion);
            printf("Cola de reproducción vaciada.\n");
        }

        // Función Limpiar Historial//
        if (strcmp(bufferComando, "clear history") == 0) {
            LimpiarPila(miApp.historial);
            printf("Historial de reproducción borrado.\n");
        }

        //Funcion Reset//

        if (strcmp(bufferComando, "reset") == 0) {
            LimpiarCola(miApp.colaReproduccion);
            LimpiarPila(miApp.historial);
            miApp.actual = NULL;
            miApp.loopActivo = 0;
            miApp.shuffleActivo = 0;
            printf("Sistema reiniciado. Todo limpio.\n");
        }        
    }
    LimpiarCola(miApp.colaReproduccion);
    LimpiarPila(miApp.historial);
    // Liberar las playlists
    for (int i = 0; i < totalListas; i++) {
        liberarLista(misPlaylists[i].inicio);
    }
    free(miApp.colaReproduccion);
    free(miApp.historial);
    return 0;
}
