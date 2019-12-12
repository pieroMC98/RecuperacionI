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
char **createRep(char **, int);

int init(char **par, int size, int **files, int **rep);

NODO *tokenizeFile(int);
int addQuery();
void enlazar(NODO **, char *, int);
void imprimirlista(NODO **lista);
NODO* normalizar(NODO *, NODO *);
int* vectorSpaceModel(NODO*);

#define ER(i) printf("-----------------<%d>\n", i)
#endif // _PRACTICA3_H
