#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "spotify.h"

Pila *newStack(void){
    Pila * s = (Pila *) malloc (sizeof(Pila));
    if (s == NULL){
        return NULL;
    }
    s->tope= NULL;
    return s;
}


void Push(Pila *p, char * name){
    if (!p) {return;}
    NodoPila * n = (NodoPila *) malloc (sizeof(NodoPila));
    if (n == NULL){
        return;
    }

    strncpy(n->songp, name, 99);
    n->songp[99] = '\0';
    n->next = p->tope;
    p->tope = n;
    return;
}


int VaciaPila(Pila *p){
    return (p == NULL || p->tope == NULL);
}


int Pop(Pila *p, char *name){
    if (VaciaPila(p)) return 0;
    NodoPila * aux = p->tope;
    p->tope = aux->next;
    strcpy(name, aux->songp);
    free(aux);
    return 1;
}

void LimpiarPila(Pila *p) {
    char temp[100];
    while (!VaciaPila(p)) {
        Pop(p, temp); 
    }
}