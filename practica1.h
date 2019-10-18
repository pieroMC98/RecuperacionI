#ifndef _primeraPractica_H
    #define _primeraPractica_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <ctype.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <string.h>
    #include <errno.h>
    int openFile    (char*);
    int writeFile   (int,char*);
    int eraseSign   (int);
    int changeLT    (int);
    int checkText   (char*);
    void menu       (char**,int*,int*,int);
    char eraseTilde (char);
    char** saveWord (int file,int,int,int*,int,char**);    
    int busqueda    (int file, char* string );


   #define ER(i) printf("<-------------------<%d>\n",i); 
   #define T 2
#endif // !_primeraPractica_H

