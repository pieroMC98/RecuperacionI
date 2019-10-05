#include "practica1.h"

// Usando el lenguaje de programación C el alumno deberá construir un programa que
// permita la normalización documentos de texto. Las características del sistema son las
// siguientes:
// a) Cada documento es un fichero de texto puro con extensión .txt
// b) Hay un fichero de “palabras vacías” que deberá usarse para preprocesar cada
// documento con el fin de obtener su representación normalizada.
// c) Cada documento normalizado es representado mediante una serie de términos en
// un fichero de texto de igual nombre con extensión .rep
// d) La normalización del documento consistirá en eliminar las palabras vacías, convertir
// mayúsculas a minúsculas y eliminar tildes y signos de puntuación, admiración e
// interrogación.
// e) El programa dispondrá de un menú para ver la lista de documentos, visualizar uno
// de ellos en su versión original y en su versión limpia y normalizada..
// Se deberán entregar todos los ficheros utilizados, incluyendo el programa fuente en C, y
// un informe en formato PDF sobre los criterios de diseño e implementación de cada aspecto del
// desarrollo de la práctica. Consultar al profesor sobre el formato del informe. 
 const char *palabrasVacias[] = {
    "a", "acuerdo", "adelante", "ademas", "además", "adrede", "ahi", "ahí", "ahora", "al", "alli", "allí", "alrededor", "antano", "antaño", "ante", "antes", "apenas", "aproximadamente", "aquel", "aquél", "aquella", "aquélla", "aquellas", "aquéllas", "aquello", "aquellos", "aquéllos", "aqui", "aquí", "arriba", "abajo", "asi", "así", "aun", "aún", "aunque", "b", "bajo", "bastante", "bien", "breve", "c", "casi", "cerca", "claro", "como", "cómo", "con", "conmigo", "contigo", "contra", "cual", "cuál", "cuales", "cuáles", "cuando", "cuándo", "cuanta", "cuánta", "cuantas", "cuántas", "cuanto", "cuánto", "cuantos", "cuántos", "d", "de", "debajo", "del", "delante", "demasiado", "dentro", "deprisa", "desde", "despacio", "despues", "después", "detras", "detrás", "dia", "día", "dias", "días", "donde", "dónde", "dos", "durante", "e", "el", "él", "ella", "ellas", "ellos", "en", "encima", "enfrente", "enseguida", "entre", "es", "esa", "ésa", "esas", "ésas", "ese", "ése", "eso", "esos", "ésos", "esta", "está", "ésta", "estado", "estados", "estan", "están", "estar", "estas", "éstas", "este", "éste", "esto", "estos", "éstos", "ex", "excepto", "f", "final", "fue", "fuera", "fueron", "g", "general", "gran", "h", "ha", "habia", "había", "habla", "hablan", "hace", "hacia", "han", "hasta", "hay", "horas", "hoy", "i", "incluso", "informo", "informó", "j", "junto", "k", "l", "la", "lado", "las", "le", "lejos", "lo", "los", "luego", "m", "mal", "mas", "más", "mayor", "me", "medio", "mejor", "menos", "menudo", "mi", "mí", "mia", "mía", "mias", "mías", "mientras", "mio", "mío", "mios", "míos", "mis", "mismo", "mucho", "muy", "n", "nada", "nadie", "ninguna", "no", "nos", "nosotras", "nosotros", "nuestra", "nuestras", "nuestro", "nuestros", "nueva", "nuevo", "nunca", "o", "os", "otra", "otros", "p", "pais", "paìs", "para", "parte", "pasado", "peor", "pero", "poco", "por", "porque", "pronto", "proximo", "próximo", "puede", "q", "qeu", "que", "qué", "quien", "quién", "quienes", "quiénes", "quiza", "quizá", "quizas", "quizás", "r", "raras", "repente", "s", "salvo", "se", "sé", "segun", "según", "ser", "sera", "será", "si", "sí", "sido", "siempre", "sin", "sobre", "solamente", "solo", "sólo", "son", "soyos", "su", "supuesto", "sus", "suya", "suyas", "suyo", "t", "tal", "tambien", "también", "tampoco", "tarde", "te", "temprano", "ti", "tiene", "todavia", "todavía", "todo", "todos", "tras", "tu", "tú", "tus", "tuya", "tuyas", "tuyo", "tuyos", "u", "un", "una", "unas", "uno", "unos", "usted", "ustedes", "v", "veces", "vez", "vosotras", "vosotros", "vuestra", "vuestras", "vuestro", "vuestros", "w", "x", "y", "ya", "yo", "z"
    }; 
int main(int argc, char const *argv[]){
     int inicial = strlen(argv[1]) + 1,k = 0;
        inicial = 50;
    char* s = malloc(inicial*sizeof(char)), *aux = malloc(10*sizeof(char)),*aux1 = malloc(10*sizeof(char));
    char *s1 = malloc(inicial*sizeof(char));
    char** out =(char**) malloc(10*sizeof(char*));
        for (int i = 0; i < 10; i++)out[i] =(char*) malloc(20*sizeof(char));
    int *files = malloc((argc-1)*sizeof(char)),t, *rep = malloc((argc-1)*sizeof(char));
    int reallocation = 0;

    for (int i = 1; i < argc; i++)
    {
         if( checkText((char*)argv[i]) == EXIT_FAILURE){
            fprintf(stderr,"Bad extension\n");
            exit(EXIT_FAILURE);
        }else{
            if( (files[i-1] = openFile((char*)argv[i])) == EXIT_FAILURE ){
                fprintf(stderr,"Error: %s",strerror(errno));
                return EXIT_FAILURE;
            }
            t = strlen(argv[i]) + 1;
        
            char *secondFile = malloc(t*sizeof(char));
            strcpy(secondFile, (char*)argv[i]);
            secondFile[t-2] = 'p';
            secondFile[t-3] = 'e';
            secondFile[t-4] = 'r'; 

            rep[i-1] = openFile( secondFile );
        }        
    }
    
    int length;
    int j = 0,x;
      for (int i = 0; i < argc -1 ; i++){      
         if( readFile(files[i],s, &length) == EXIT_FAILURE) exit(EXIT_FAILURE);
        
           if( (s = (char*) realloc(s,(length+1)*sizeof(char))) == NULL || (s1 = (char*)realloc(s1,(length+1)*sizeof(char))) == NULL){
               fprintf(stderr,"Error realloc: %s",strerror(errno));
               exit(EXIT_FAILURE);
           }
            out =  saveWord(files[i],0,&reallocation,out);
            out = realloc(out,reallocation*sizeof(char*));

            size_t row = sizeof out / sizeof *out;
            size_t column = sizeof(out[0])/row;
            //row = column = 8; 
           printf(" size = %ld | s = %ld\n\n",sizeof(out),sizeof(out[0]));
           
            for (int i = 0; i < row; i++){
                printf(" %d = %s\n",i,out[i]);
            }
            
            k = i;
            int g = 0;
             int fi =  lseek(rep[k], 0, SEEK_SET);
            for (int i = 0; i < 3; i++){

                if( i != 0)lseek(rep[k],g,SEEK_CUR);
                for (int j = 0; out[i][j] != '\0'; j++){
                    if(  ( x = eraseSign( out[i][j] )) != out[i][j] ) s1[j] = x;
                        else s1[j] = changeLT( out[i][j] ); 
                        
                    if( out[i][j] == -1 ){
                            s1[j] = eraseTilde(out[i][j+1]);
                        /*   int k = j+1;
                            while ( s[k] != '\0' ){
                                s[k] = s[k+1];
                                k++;
                            }     */
                    }    

                  // printf("%c \n",s1[j]);
                   printf(" s1 = %c // %d | %d \n",s1[j],j,g);
                     if( writeFile(rep[k],s1[j]) == EXIT_FAILURE ) exit(EXIT_FAILURE);
                   g =  lseek(rep[k],1,SEEK_SET);
                   
                }
                g =  lseek(rep[k],1,SEEK_CUR);
                writeFile(rep[k],' ');
                ER(i);
               /*      
               sprintf(aux1," %s",s1);
               //printf(" cadena es %s | %s\n",s1, out[i]);

               strcat(aux,aux1); */
               //printf("%s",aux);

            }
           char c ;
           int final = lseek(rep[k],0,SEEK_END);
             lseek(rep[k],0,SEEK_SET);
             int l = 0;
            while ( l <= final){
               read(rep[k],&c,sizeof(char));
               printf("%c",c);
                lseek(rep[k],0,SEEK_CUR);
                l++;
            }
           
          /* j = 0;
            while (s[j] != '\0'){            
                
                if(  ( x = eraseSign( s[j] )) != s[j] )  s1[j] = x;
                    else s1[j] = changeLT( s[j] ); 
                    
                if( s[j] == -61) {
                    s1[j] = eraseTilde(s[j+1]);
    
                    int k = j+1;
                    while ( s[k] != '\0' ){
                         s[k] = s[k+1];
                        k++;
                    }                    
                } 
     
                j++;
            } */
        //if( writeFile(rep[i],s1) == EXIT_FAILURE ) exit(EXIT_FAILURE);
      }

    int err;
    for (int i = 0; i < argc - 1; i++){
        
        if(close(files[i]) == -1 || close(rep[i]) == -1){
            err = errno;
            fprintf(stderr,"file %d: %s",i,strerror(err));
            exit(EXIT_FAILURE);
        }
        
    }
        printf("\n== : %s\n",s1);

    //printf("############\n");
    //menu((char**)argv,argv,argc);
        free(s);
        free(s1); 
    return EXIT_SUCCESS;
}

