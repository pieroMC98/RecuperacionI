#if !defined(_PRACTICA3_H)
#define _PRACTICA3_H
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct nodo {
  char string[100];
  int t;
  int i;
  struct nodo *link;
} NODO;

int filetype(char **, int);
int checkText(char *);
int *openFile(char **, int);
int validateTXT(char **, int);
int init(char **par, int size, int **files, int **rep);
int *vectorSpaceModel(NODO *, NODO *);
char **createRep(char **, int);
NODO *addQuery();
NODO *tokenizeFile(int);
NODO *normalizar(NODO *, NODO *);
NODO *repeat(NODO *);
void vector(NODO **, NODO *, int *);
void imprimirlista(NODO **lista);
void enlazar(NODO **, char *, int);
void deleteNodo(NODO **, NODO *);
void E1(NODO **nodo);
int product(int *, int *, int);

void final(int arg, NODO **aux, int *files, int pv, NODO **l, int **r,
           int *lengt, NODO **q, int **Q);
void screen(NODO **aux, int lengt, int **r, int *Q, NODO *l, int arg);
void writeRep(int *, NODO **, int);

#define ER(i) printf("-----------------<%d>\n", i)
#define jump printf("\n")
#endif // _PRACTICA3_H
