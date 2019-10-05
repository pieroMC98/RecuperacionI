#ifndef _primraPractica_H
    #define _primraPractica_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <ctype.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <string.h>
    #include <errno.h>
    int openFile    (char*);
    int writeFile   (int,char);
    int eraseSign   (int);
    int changeLT    (int);
    int readFile    (int f, char *buffer,int*);
    int checkText   (char*);
    void menu       (char**,char**,int);
    char eraseTilde (char);
    int stopWords(char *,char**);
    char** saveWord(int file,int,int*,char**);

   #define ER(i) printf("<------------------->%d\n",i); 
   extern const char *palabrasVacias[];

#endif // !_primraPractica_H

