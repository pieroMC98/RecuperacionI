#ifndef _primeraPractica_H
#define _primeraPractica_H
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
typedef struct c {
  int *r;
  int i;
} string;
int openFile(char *);
int writeFile(int, char *);
int eraseSign(int);
int changeLT(int);
int checkText(char *);
void menu(char **, int *, int *, int);
char eraseTilde(char);
char **saveWord(int file, int, int, int *, int, char **);
int search(int file, char *string);
char **addQuery(int *);
string operation(char **, int, char ***, int, int *);


#define ER(i) printf("<-------------------<%d>\n", i);
#define T 2
#endif // !_primeraPractica_H
