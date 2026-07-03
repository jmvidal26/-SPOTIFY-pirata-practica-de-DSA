#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "spotify.h"

SongNode* crearNodoCancion(char *nombre) {
    SongNode *nuevo = (SongNode*) malloc(sizeof(SongNode));
    if (nuevo != NULL) {
        strncpy(nuevo->nombre, nombre, 99);
        nuevo->nombre[99] = '\0';
        nuevo->next = NULL;
        nuevo->prev = NULL;
    }
    return nuevo;
}

void inicializarLista(ListaCanciones *lc, char *nombre) {
    strncpy(lc->nombreLista, nombre, 99);
    lc->nombreLista[99] = '\0';
    lc->inicio = NULL; 
}

void liberarLista(SongNode *cabeza) {
    while (cabeza != NULL) {
        SongNode *temp = cabeza;
        cabeza = cabeza->next;
        free(temp);
    }
}

