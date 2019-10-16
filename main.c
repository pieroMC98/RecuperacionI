#include "practica1.h"

int main(int argc, char const *argv[]){
    int inicial = 50, Menu = 0;
    char c;
    char *s1 = malloc(inicial*sizeof(char));
    char **out =(char**) calloc((T),sizeof(char*));
        for (int i = 0; i < (T); i++)out[i] =(char*) calloc(20,sizeof(char));
    int *files = malloc((argc-1)*sizeof(char)),
        t,
        *rep = malloc((argc-1)*sizeof(char)),
        tout;

    int reallocation = 0;
    int final, u, l;
    int palabrasVacias = open("palabrasVacias",O_RDONLY, 0644);
    if( palabrasVacias == EXIT_FAILURE){
        perror("palabras vacias");
        return EXIT_FAILURE;
    }

    //busqueda(palabrasVacias, "ana");
    //files iterator

    for (int i = 1; i < argc; i++) {
         if( checkText((char*)argv[i]) == EXIT_FAILURE){
            fprintf(stderr,"Bad extension\n");
            exit(EXIT_FAILURE);

        }else{
            if( (files[i-1] = open( ((char*)argv[i]) ,O_RDONLY, 0644)) == -1 ){
                fprintf(stderr,"Error: %s",strerror(errno));
                return EXIT_FAILURE;
            }
            t = strlen(argv[i]) + 1;
        
            char *secondFile = malloc(t*sizeof(char));
        printf(" %d --> %s\n",i,argv[i]);
            strcpy(secondFile, (char*)argv[i]);
            secondFile[t-2] = 'p';
            secondFile[t-3] = 'e';
            secondFile[t-4] = 'r'; 

            rep[i-1] = open( secondFile,O_CREAT | O_RDWR, 0644);            
            if( rep[i-1] == -1){
                fprintf(stderr,"Error: %s",strerror(errno));
                return EXIT_FAILURE;
            }
        }        
    }

    for (int i = 0; i < argc -1 ; i++){ 
        reallocation = 1;
        out = (char**) saveWord(files[i],palabrasVacias,0,&reallocation,0,out);  
        reallocation--;
        u = 1;
        for (int i1 = 0; i1 < reallocation; i1++)
              u += strlen(out[i1]); 
        

        lseek(rep[i], 0, SEEK_SET);
            for (int j = 0; j < reallocation; j++){               
                tout = strlen(out[j]);
                strcpy(s1,out[j]);
               // printf("\ncoping s1 = %s | out[%d] = %s\n\n",s1,j,out[j]);
                for (int k = 0; k <= tout; k++){
                    if( s1[k] == ' ') break;
                    
                    if( eraseSign(s1[tout-1]) ) s1[tout-1] = '\0';

                    while( eraseSign(s1[k]) ){
                        for (int z = k; z < tout; z++) 
                            s1[z] = s1[z+1]; 
                        tout--;
                        s1 = (char*) realloc(s1,sizeof(char)*tout);
                    }
 
                     s1[k] = changeLT( s1[k] ); 
                        
                     if( out[j][k] == -61 ){
                            s1[k] = eraseTilde(s1[k+1]);                              
                            for (int z = k+1; z < tout; z++) s1[z] = s1[z+1];                            
                            tout--;
                            s1 = (char*) realloc(s1,sizeof(char)*tout); 
                    }   
                 
                }     
                if( writeFile(rep[i],s1) == EXIT_FAILURE ) exit(EXIT_FAILURE);
              
            }

            final = lseek(rep[i],0,SEEK_END);
            l     = lseek(rep[i],0,SEEK_SET);

            while ( l <= final){
                read(rep[i],&c,sizeof(char));
                 if( c == 0){
                    c = 32;
                    lseek(rep[i],-1,SEEK_CUR);
                    write(rep[i],&c,sizeof(char));
                } 
                lseek(rep[i],0,SEEK_CUR);
                l++;
            }
    }

    do
    {
        system("clear");
        menu((char**)argv,files,rep,argc);
        printf("\n---------\n-1 :: exit()\n");
        scanf("%d",&Menu);
        Menu = Menu == -1 ? -1 : 0;
    } while ( !Menu );
    
    int err;
    for (int i = 0; i < argc - 1; i++){
        
        if(close(files[i]) == -1 || close(rep[i]) == -1){
            err = errno;
            fprintf(stderr,"file %d: %s",i,strerror(err));
            exit(EXIT_FAILURE);
        }
        
    }
printf("\n\n");
free(s1); 
return EXIT_SUCCESS;
}

