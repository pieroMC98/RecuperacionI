#include "practica1.h"
/**
 * @brief Verifica si carácter es signo de puntuación
 *
 * @param i, si carácter recibido es signo de puntuación devuelve 1, sino
 * devuelve 0
 * @return int
 */
int eraseSign(int i) {
  if ((i > 32 && i < 48) || (i >= 58 && i <= 64))
    return 1;
  return 0;
}

/**
 * @brief Elimina vocal con tilde
 *
 * @param j carácter acentudado
 * @return j, carácter sin tilde
 */
char eraseTilde(char j) {
  if (j == -95)
    return 'a';
  else if (j == -87)
    return 'e';
  else if (j == -83)
    return 'i';
  else if (j == -77)
    return 'o';
  else if (j == -70)
    return 'u';
  return j;
}

/**
 * @brief Abre archivos
 *
 * @param file[]
 * @return int , si tuvo existo devuelve el descriptor, sino devuelve
 * EXIT_FAILURE.
 */
int openFile(char *file) {
  int f;
  if ((f = open(file, O_CREAT | O_RDWR, 0644)) == -1) {
    return EXIT_FAILURE;
  }
  return f;
}

/**
 * @brief  Comprueba si puede escribir un fichero.
 *
 * @param f, descriptor del archivo
 * @param buffer, tamaño del archivo
 * @return int, si puede escribir en el fichero devuelve EXIT_SUCCESS, sino
 * EXIT_FAILURE
 */
int writeFile(int f, char *buffer) {
  int err;
  if (write(f, buffer, sizeof(char) * (strlen(buffer) + 1)) == -1) {
    err = errno;
    fprintf(stderr, "No se puede escribir en el fichero \"%d\":: %s\n", f,
            strerror(err));
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

/**
 * @brief Cambia carácter de miúscula a mayúscula y viceversa
 *
 * @param i, carácter en entero ASCII
 * @return int, devuelve carácter cambiado
 */
int changeLT(int i) {
  if (i >= 65 && i <= 90)
    return i + 32;
  else if (i >= 97 && i <= 122)
    return i - 32;
  return i;
}

/**
 * @brief función recursiva que leer cada cadena del fichero y lo devuelve al
 * @file main.c
 *
 * @param file, descriptor del archivo.
 * @param x, puntero de la siguiente cadena a leer.
 * @param j, devuelve el tamaño de @param out
 * @param i, condición de salida de llamada recursiva @module saveWord
 * @param out, tupla de caracteres devuelvo al main
 * @return char**
 */

int busqueda(int file, char *string) {
  int final = lseek(file, 0, SEEK_END);
  lseek(file, 0, SEEK_SET);
  int j;
  char *buffer = malloc(10 * sizeof(char));
  char c;
  int g = 0;

  while ((g < final)) {
    j = 0;
    do {
      read(file, &c, sizeof(char));
      if (c == ',') {
        buffer[j] = '\0';
        break;
      }
      buffer[j] = c;
      g = lseek(file, 0, SEEK_CUR);
      j++;
    } while ((c != 44) && (g < 2071));
    lseek(file, 1, SEEK_CUR);

    buffer = realloc(buffer, (j) * sizeof(char));
    for (int i = 0; i < (strlen(buffer) + 1); i++)
      buffer[i] = tolower(buffer[i]);

    if (!strcmp(string, buffer))
      return 1;
  }
  return 0;
}

char **saveWord(int file, int PV, int x, int *j, int i, char **out) {
  int f = lseek(file, 0, SEEK_END);
  lseek(file, x, SEEK_SET);
  char *buffer = malloc((20) * sizeof(char)), c;
  int save = 1;

  if (i == f)
    return out;
  else {
    while ((x + i) < f) {
      read(file, &c, sizeof(char));
      if (c != ' ') {
        buffer[i] = c;
        i++;
      } else {
        x = lseek(file, 0, SEEK_CUR);
        buffer[i] = '\0';
        break;
      }
    }

    save = busqueda(PV, buffer);
    if (!save) {
      buffer = (char *)realloc(buffer, sizeof(char) * (strlen(buffer) + 1));
      if ((out = (char **)realloc(out, sizeof(char *) *(*j+1))) == NULL) {
        fprintf(stderr, "out: %s\n", strerror(errno));
      } else {
        for (int i = 0; i <(*j+1); i++){          
          if( (out[i] = (char *)realloc(out[i], sizeof(char) * (strlen(buffer) + 1))) == NULL) 
              fprintf(stdout, "out[%d] = %s\n",i,strerror(errno));
        }
        out[*j] = NULL;       
    
      }
        perror("out:");
      strcpy(out[*j - 1], buffer);
      *j = *j + 1;
    }
    saveWord(file, PV, x, j, ((x + i) == f ? x + i : 0), out);
  }
}

/**
 * @brief Comrprueba si fichero @param f es f.txt
 *
 * @param f, cadena simboliza fichero por @file main.c
 * @return int, si puede leer el fichero devuelve EXIT_SUCCESS, sino
 * EXIT_FAILURE
 */
int checkText(char *f) {
  int t = strlen(f) + 1;
 return f[t] != 't' && f[t - 1] != 'x' && f[t - 2] != 't' && f[t - 3] != '.' ? EXIT_FAILURE : EXIT_SUCCESS;
}

/**
 * @brief Interfaz de usuario en el que elige el documento a visualizar y su
 * forma normalizada.
 *
 * @param F, tupla de nombre de fichero introducidos.
 * @param files, tupla de descriptores de ficheros .txt
 * @param rep, tupla de descriptores de ficheros normalizados .rep
 * @param row, numero de ficheros
 */
void menu(char **F, int *files, int *rep, int row) {
  char c;
  int final, l;
  int option;

  printf("-------------------\n");
  for (int i = 1; i < row; i++)
    printf("file %d: \"%s\"\n", i, F[i]);

  printf(
      "\nElija archivo (1...%d) para su visualiazacion y forma normalizada\n",
      row - 1);
  scanf("%d", &option);
  option -= 1;

  system("clear");
  printf("%s->\n", F[option]);

  final = lseek(files[option], 0, SEEK_END);
  l = lseek(files[option], 0, SEEK_SET);
  while (l <= final) {
    read(files[option], &c, sizeof(char));
    printf("%c", c);
    lseek(files[option], 0, SEEK_CUR);
    l++;
  }
  printf("\n\n-----------------\n\n");
  printf("%s->\n", F[option]);

  final = lseek(rep[option], 0, SEEK_END);
  l = lseek(rep[option], 0, SEEK_SET);
  while (l <= final) {
    read(rep[option], &c, sizeof(char));
    printf("%c", c);
    lseek(rep[option], 0, SEEK_CUR);
    l++;
  }
}
