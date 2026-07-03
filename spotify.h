#ifndef SPOTIFY
#define SPOTIFY

typedef struct song {
    char nombre[100];
    struct song *next;
    struct song *prev; 
} SongNode;

typedef struct nodec {
    char songc[100];
    struct nodec * next;
} NodoCola;

typedef struct {
    NodoCola * ini;
    NodoCola * fin;
} Cola;

typedef struct nodo_pila {
    char songp[100];
    struct nodo_pila *next;
} NodoPila;

typedef struct {
    NodoPila *tope;
} Pila;


typedef struct {
    char nombreLista[100];
    SongNode *inicio;
} ListaCanciones;

typedef struct {
    SongNode *actual;         // La canción que suena ahora 
    Cola *colaReproduccion;   //Cola
    Pila *historial;          //Pila
    int loopActivo;           // 0: apagado, 1: encendido
    int shuffleActivo;        // Para manejar el estado del reordenamiento 
} Reproductor;

//Funciones Cola//

Cola * Newcola();
int Acolar(Cola * c, char *s);
int Descolar(Cola *c, char * name);
int VaciaCola(Cola *c);
void LimpiarCola(Cola *c);

//Funciones Pila//

Pila *newStack(void);
void Push(Pila *p, char * name);
int Pop(Pila *p, char *name);
int VaciaPila(Pila *p);
void LimpiarPila(Pila *p);

//Funciones Listas//

SongNode* crearNodoCancion(char *nombre);
void inicializarLista(ListaCanciones *lc, char *nombre);
void liberarLista(SongNode *cabeza);

#endif 