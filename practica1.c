#include "practica1.h"

int eraseSign(int i){
     if( (i > 32 && i < 48) ) return 1;
return 0;
}

char eraseTilde(char j){
     if( j == -95 ) return  'a';
        else if( j == -87) return  'e';
            else if( j == -83) return  'i';
                else if( j == -77) return  'o';
                    else if( j == -70) return  'u';
return j;
}


int openFile(char *file){       
    int f;
    if( (f = open(file,  O_CREAT | O_RDWR , 0644 )) == -1){
        return EXIT_FAILURE;
    }
return f;
}

int writeFile(int f, char *buffer){
    int err;
    if( write( f,buffer, sizeof(char)*(strlen(buffer) + 1) ) == -1){
        err = errno;
        fprintf(stderr,"No se puede escribir en el fichero \"%d\":: %s\n",f,strerror(err));
        return EXIT_FAILURE;
    }
return EXIT_SUCCESS;
}

int readFile(int f, char *buffer, int *length){
    int tam = lseek(f,0,SEEK_END);
    int err = errno;
    
     lseek(f,0,SEEK_SET);
    *length = tam + 1;
    if( read(f,buffer,tam+1) == -1){
        fprintf(stderr,"Error lectura fichero:: %s\n",strerror(err));
        return EXIT_FAILURE;
    }
return EXIT_SUCCESS;
}

int changeLT(int i){
    if( i >= 65 && i <=90 ) return i + 32;
        else if( i >= 97 && i <= 122 ) return i - 32;
return i;
}

char** saveWord(int file,int x,int *j,int i,char **out){
    int f = lseek(file,0,SEEK_END);
            lseek(file,x,SEEK_SET);
    char *buffer = malloc((100)*sizeof(char)), c;     
    int save = 1;        

    if( i == f) {
        //salida = 1;
        return out;
    }else{
        while( (x + i) < f){
            read(file,&c,sizeof(char));   
            if( c != ' '){ 
                buffer[i] = c;
                i++;
            }else{      
            x = lseek(file,0,SEEK_CUR);
                buffer[x] = ' ';
            break;
            }
        } 

        for (int i = 0; i < 308; i++){
            //si ! -> palabra vacia 
            for (int i = 0; i < (strlen(buffer)+1); i++) buffer[i] = tolower(buffer[i]);
    
            if( !strcmp(palabrasVacias[i],buffer) ){            
            save = 0;
            break;
            }
        }
 
        if(save == 1){ 
                buffer = (char*)realloc(buffer,sizeof(char)*(i+1));
            
                if( (out = (char**)realloc(out,sizeof(char*)*(*j+1))) == NULL){
                        printf(" doesnt work\n");
                }else{
                        for (int i = 0; i < (*j+1); i++) out[i] = (char*)realloc(out[i],sizeof(char)*20);   
                }

                if( out[*j] == NULL)perror("se me ha matao");
                   strcpy(out[*j],buffer);            
                    
                buffer[*j + 1] = '\0';
                *j = *j +1;
        }
        
        saveWord(file, x, j,( (x + i) == f ? x + i : 0) ,out);
    }
}

int checkText(char *f){
    int t = strlen(f) + 1;
    if( f[t] != 't' && f[t-1] != 'x' && f[t-2] != 't' && f[t-3] != '.' ) return EXIT_FAILURE;
return EXIT_SUCCESS;
}

void menu(char** F,int *files,int *rep,int row){
    char c;
    int final, l;
    int option;

    printf("-------------------\n");
    for (int i = 1; i < row; i++) printf("file %d: \"%s\"\n",i,F[i]);       
         
    printf("\nElija archivo (1...%d) para su visualiazacion y forma normalizada\n",row-1);
    scanf("%d",&option);
    option -= 1;

    system("clear");
    printf("%s->\n",F[option]);

    final = lseek(files[option],0,SEEK_END);
    l     = lseek(files[option],0,SEEK_SET);
    while ( l <= final){
        read(files[option],&c,sizeof(char));
        printf("%c",c);
        lseek(files[option],0,SEEK_CUR);
        l++;
    }
    printf("\n\n-----------------\n\n");
    printf("%s->\n",F[option]);

    final = lseek(rep[option],0,SEEK_END);
    l     = lseek(rep[option],0,SEEK_SET);
    while ( l <= final){
        read(rep[option],&c,sizeof(char));
        printf("%c",c);
        lseek(rep[option],0,SEEK_CUR);
        l++;
    }
}
