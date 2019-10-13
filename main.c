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
    "a", "acuerdo", "adelante", "ademas", "además", "adrede", "ahi", "ahí", "ahora", "al", "alli", "allí", "alrededor", "antano", "antaño", "ante", "antes", "apenas", "aproximadamente", "aquel", "aquél", "aquella", "aquélla", "aquellas", "aquéllas", "aquello", "aquellos", "aquéllos", "aqui", "aquí", "arriba", "abajo", "asi", "así", "aun", "aún", "aunque",
    "b", "bajo", "bastante", "bien", "breve", 
    "c", "casi", "cerca", "claro", "como", "cómo", "con", "conmigo", "contigo", "contra", "cual", "cuál", "cuales", "cuáles", "cuando", "cuándo", "cuanta", "cuánta", "cuantas", "cuántas", "cuanto", "cuánto", "cuantos", "cuántos",
    "d", "de", "debajo", "del", "delante", "demasiado", "dentro", "deprisa", "desde", "despacio", "despues", "después", "detras", "detrás", "dia", "día", "dias", "días", "donde", "dónde", "dos", "durante",
    "e", "el", "él", "ella", "ellas", "ellos", "en", "encima", "enfrente", "enseguida", "entre", "es", "esa", "ésa", "esas", "ésas", "ese", "ése", "eso", "esos", "ésos", "esta", "está", "ésta", "estado", "estados", "estan", "están", "estar", "estas", "éstas", "este", "éste", "esto", "estos", "éstos", "ex", "excepto",
    "f", "final", "fue", "fuera", "fueron",
    "g", "general", "gran",
    "h", "ha", "habia", "había", "habla", "hablan", "hace", "hacia", "han", "hasta", "hay", "horas", "hoy",
    "i", "incluso", "informo", "informó",
    "j", "junto",
    "k",
    "l", "la", "lado", "las", "le", "lejos", "lo", "los", "luego",
    "m", "mal", "mas", "más", "mayor", "me", "medio", "mejor", "menos", "menudo", "mi", "mí", "mia", "mía", "mias", "mías", "mientras", "mio", "mío", "mios", "míos", "mis", "mismo", "mucho", "muy",
    "n", "nada", "nadie", "ninguna", "no", "nos", "nosotras", "nosotros", "nuestra", "nuestras", "nuestro", "nuestros", "nueva", "nuevo", "nunca",
    "o", "os", "otra", "otros",
    "p", "pais", "paìs", "para", "parte", "pasado", "peor", "pero", "poco", "por", "porque", "pronto", "proximo", "próximo", "puede",
    "q", "qeu", "que", "qué", "quien", "quién", "quienes", "quiénes", "quiza", "quizá", "quizas", "quizás",
    "r", "raras", "repente",
    "s", "salvo", "se", "sé", "segun", "según", "ser", "sera", "será", "si", "sí", "sido", "siempre", "sin", "sobre", "solamente", "solo", "sólo", "son", "soyos", "su", "supuesto", "sus", "suya", "suyas", "suyo",
    "t", "tal", "tambien", "también", "tampoco", "tarde", "te", "temprano", "ti", "tiene", "todavia", "todavía", "todo", "todos", "tras", "tu", "tú", "tus", "tuya", "tuyas", "tuyo", "tuyos", 
    "u", "un", "una", "unas", "uno", "unos", "usted", "ustedes", 
    "v", "veces", "vez", "vosotras", "vosotros", "vuestra", "vuestras", "vuestro", "vuestros", 
    "w", 
    "x", 
    "y", "ya", "yo", 
    "z"
}; 

int main(int argc, char const *argv[]){
    int inicial = strlen(argv[1]) + 1;
        inicial = 50;
    char c;
    char* s = malloc(inicial*sizeof(char));
    char *s1 = malloc(inicial*sizeof(char));
    char** out =(char**) malloc(5*sizeof(char*));
        for (int i = 0; i < 5; i++)out[i] =(char*) malloc(20*sizeof(char));

    int *files = malloc((argc-1)*sizeof(char)),
        t,
        *rep = malloc((argc-1)*sizeof(char)),
        tout;
    
    int reallocation = 0;
    int final, u, l;
    int length;

    //files iterator
    for (int i = 1; i < argc; i++) {
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
    
   
    for (int i = 0; i < argc -1 ; i++){ 
        length = lseek(files[i],0, SEEK_END);
        
        if( readFile(files[i],s, &length) == EXIT_FAILURE) exit(EXIT_FAILURE);
         lseek(files[i],0, SEEK_SET);
 
        reallocation = 0;
        out = (char**) saveWord(files[i],0,&reallocation,0,out);
 
        u = 1;
        for (int i1 = 0; i1 < reallocation; i1++)  u += strlen(out[i1]);  

        lseek(rep[i], 0, SEEK_SET);
            for (int j = 0; j < reallocation; j++){
                tout = strlen(out[j]);
                strcpy(s1,out[j]);
        
                for (int k = 0; k <= tout; k++){
                    if( s1[k] == ' ') break;

                    while( eraseSign(s1[k]) ){
                        for (int z = k; z < tout; z++) s1[z] = s1[z+1];                        
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

    menu((char**)argv,files,rep,argc);
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

