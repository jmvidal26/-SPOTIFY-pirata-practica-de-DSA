#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "spotify.h"

Cola * Newcola(){
    Cola * c = (Cola *) malloc (sizeof(Cola));
    if (c == NULL ) {
        printf("No se pudo almacenar memoria para la cola");
        return NULL;
    }
    c->ini = NULL;
    c->fin = NULL;
    return c;
}

int Acolar(Cola * c, char *s){ // Cambiado a char *s
    if (c == NULL || s == NULL){return 0;}
    NodoCola * nc = (NodoCola *) malloc (sizeof(NodoCola));
    if (nc == NULL){return 0;}

    strncpy(nc->songc, s, 99); // Usamos strncpy por seguridad
    nc->songc[99] = '\0';
    nc->next = NULL;

    if (c->ini == NULL){
        c->ini = nc;
    } else {
        c->fin->next = nc;
    }
    c->fin = nc;
    return 1;
}

int VaciaCola(Cola *c) {
    return (c == NULL || c->ini == NULL);
}


int Descolar(Cola *c, char * name){
    if (c == NULL || c->ini == NULL){return 0;}
    NodoCola * aux = c->ini;
    c->ini = aux->next;
    if (c->fin == aux){
        c->fin = NULL;
    }
    strcpy(name, aux->songc);
    free (aux);
    return 1;
}

void LimpiarCola(Cola *c) {
    char temp[100];
    while (!VaciaCola(c)) {
        Descolar(c, temp); 
    }
}
