#include "practica1.h"

int eraseSign(int i){
     if( (i > 32 && i < 48) ) return ' ';
return i;
}

char eraseTilde(char j){
     if( j == -95 ) return  'a';
        else if( j == -87) return  'e';
            else if( j == -83) return  'i';
                else if( j == -77) return  'o';
                    else if( j == -70) return  'u';
return j;
}

int changeLT(int i){
    if( i >= 65 && i <=90 ) return i + 32;
        else if( i >= 97 && i <= 122 ) return i - 32;
return i;
}

int openFile(char *file){       
    int f;
    if( (f = open(file,  O_CREAT | O_RDWR , 0644 )) == -1){
        return EXIT_FAILURE;
    }
return f;
}

int writeFile(int f, char buffer){
    int err;
        if( write( f,&buffer, sizeof(char)) == -1){
            err = errno;
        fprintf(stderr,"No se puede escribir en el fichero \"%d\":: %s\n",f,strerror(err));
        return EXIT_FAILURE;
    }
return EXIT_SUCCESS;
}

int readFile(int f, char *buffer, int *length){
     int tam = lseek(f,0,SEEK_END);
     lseek(f,0,SEEK_SET);
    *length = tam + 1;
    if( read(f,buffer,tam+1) == -1){
        int err = errno;
        fprintf(stderr,"Error lectura fichero:: %s\n",strerror(err));
        return EXIT_FAILURE;
    }
return EXIT_SUCCESS;
}

char** saveWord(int file,int x,int *j,char **out){
    int f = lseek(file,0,SEEK_END);
    lseek(file,x,SEEK_SET);
    char *buffer = malloc((10)*sizeof(char)), c;
    int save = 1;
   
        
    int i = 0, salida = 0;

    while( (x + i) < f){
        read(file,&c,sizeof(char));   
        if( c != ' '){ 
            buffer[i] = c;
            i++;
        }else {          
            x = lseek(file,0,SEEK_CUR);
          break;
        }
    } 

    if( (x + i) == f ) salida = 1;


    for (int i = 0; i < 308; i++){
        //si ! -> palabra vacia 
        if( !strcmp(palabrasVacias[i],buffer) ){            
          // printf("-->%s\n",buffer);
           save = 0;
           break;
        }
    }
   // printf("j = %d \n",j);
       if(save == 1){ 
          /*  out = (char**)realloc(out,(j+1)*sizeof(char*));
           if( out == NULL) fprintf(stderr,"out:: %s",strerror(errno));
           for (int i = 0; i <= strlen(buffer); i++){               
            out[i] = (char*)realloc(out[i],(j+1)*sizeof(char));
            if( out[i] == NULL)  fprintf(stderr,"out:: %s",strerror(errno)); */
        
           strcpy(out[*j],buffer);
          printf(" cadena = %s\n",out[*j]);
           *j = *j +1;
           //printf(" %d \n",*j);
        }

    //size_t row = sizeof(out) / sizeof(out[0]);
    if( !salida ) saveWord(file, x, j,out);
 return out;
}

int checkText(char *f){
    int t = strlen(f) + 1;
    if( f[t] != 't' && f[t-1] != 'x' && f[t-2] != 't' && f[t-3] != '.' ) return EXIT_FAILURE;
return EXIT_SUCCESS;
}

void menu(char** F,char **F1,int row){
    printf("-------------------\n");
     for (int i = 1; i < row; i++)
     {
        printf("file %d: \"%s\"\n",i,F[i]);       
     }
     
    printf("Elija archivo (1...%d) para su visualiazacion y forma normalizada\n",row);
    int option;
    scanf("%d",&option);
    option -= 1;

    system("clear");
    printf("%s",F[option]);
    printf("-----------------");
    printf("%s",F[option]);
}

int stopWords(char *word,char **stopWords){
   for (int i = 0; i < 308; i++){
       if(strcmp(word, stopWords[i]) == 0) return 1;
   }
return 0;
}


