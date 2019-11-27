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
  switch (j) {
  case -95:
    return 'a';
    break;
  case -87:
    return 'e';
    break;
  case -83:
    return 'i';
    break;
  case -77:
    return 'o';
    break;
  case -70:
    return 'u';
    break;
  }
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

int search(int file, char *string) {
  int final = lseek(file, 0, SEEK_END);
  lseek(file, 0, SEEK_SET);
  int j;
  char *buffer = malloc(50 * sizeof(char));
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
      // getchar();
      if (g >= final) {
        buffer[j] = '\0';
        break;
      }

    } while ((c != 44));
    lseek(file, 1, SEEK_CUR);
    buffer = realloc(buffer, (j + 1) * sizeof(char));

    for (int i = 0; i < (strlen(buffer) + 1); i++)
      buffer[i] = tolower(buffer[i]);
    // equals
    if (!strcmp(string, buffer)) {

      return 1;
    }
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
    save = search(PV, buffer);
    if (!save) {
      buffer = (char *)realloc(buffer, sizeof(char) * (strlen(buffer) + 1));
      int k = *j + 1;
      k = f;

      if ((out = (char **)realloc(out, sizeof(char *) * (k))) == NULL) {
        fprintf(stderr, "out: %s\n", strerror(errno));
      } else {
        for (int i = 0; i < (k); i++) {
          if ((out[i] = (char *)realloc(
                   out[i], sizeof(char) * (strlen(buffer) + 1))) == NULL)
            fprintf(stdout, "out[%d] = %s\n", i, strerror(errno));
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
  return f[t] != 't' && f[t - 1] != 'x' && f[t - 2] != 't' && f[t - 3] != '.'
             ? EXIT_FAILURE
             : EXIT_SUCCESS;
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
  while (l < final) {
    read(files[option], &c, sizeof(char));
    printf("%c", c);
    lseek(files[option], 0, SEEK_CUR);
    l++;
  }
  printf("\n\n-----------------\n\n");
  printf("%s->\n", F[option]);

  final = lseek(rep[option], 0, SEEK_END);
  l = lseek(rep[option], 0, SEEK_SET);
  while (l < final) {
    read(rep[option], &c, sizeof(char));
    printf("%c", c);
    lseek(rep[option], 0, SEEK_CUR);
    l++;
  }
}

char **addQuery(int *size) {
  char **string = NULL;
  char *aux = malloc(sizeof(char) * 100);
  printf("Introduzca su consulta( sin comas entre ellos ): \n");
   fgets(aux, 100, stdin);
  //char aux[] = "barco, casa, coche, gato, perro";
  int l = strlen(aux), i = 0;
  char *token = malloc((l + 1) * sizeof(char));
  char *aux1 = malloc((l + 1) * sizeof(char));
  const char *delim = " \n";

  int k = 0;
  strcpy(aux1, aux);
  token = strtok(aux, delim);
  while (token != NULL) {
    token = strtok(NULL, delim);
    k++;
  }

  char *token1 = strtok(aux1, delim);

  string = (char **)malloc((k) * sizeof(char *));
  while (token1 != NULL) {
    string[i] = (char *)malloc(sizeof(char) * (strlen(token1) + 1));

    strcpy(string[i], token1);
    token1 = strtok(NULL, delim);
    i++;
  }
  *size = k;
  for (int i = 0; i < k; i++)
    printf(">\"%s\"\n", string[i]);

  return string;
}

string operation(char **vocabulario, int t, char ***terms, int x, int *c) {
  int option, bool = 0, **VAR = NULL;
  int g = 0, g3 = 0, g1, *resultado = calloc(sizeof(int), 20);
  int aux;
  int op1, op2;
  int count = 0;
   string n;
   n.r = NULL;
   n.i = 0;
  char **Vocabulario = (char **)malloc(sizeof(char) * 1);

  int k = 0, h = 0;
  for (int k = 0; k < 1; k++)
    Vocabulario[k] = (char *)malloc(sizeof(char) * 20);

  for (int i = 0; i < x; i++) {
    for (int j = 0; j < c[i]; j++) {
      Vocabulario = (char **)realloc(Vocabulario, sizeof(char *) * (c[i] + k));
      for (int k1 = k; k1 < (c[i] + k); k1++)
        Vocabulario[k1] =
            (char *)malloc(sizeof(char) * (strlen(terms[i][j]) + 1));
      for (int u = 0; u < t; u++) {
        if (!strcmp(vocabulario[u], terms[i][j])) {
          bool = 1;
          break;
        } else
          bool = 0;
      }
      if (bool) {
        strcpy(Vocabulario[k], terms[i][j]);
        k++;
      }
    }
    if (bool)
      h += c[i];
  }

  for (int i = 0; i < h; i++) {
    for (int j = h - 1; j > i; j--) {
      if (!strcmp(Vocabulario[j], Vocabulario[i])) {
        for (int k = j + 1; k < h; k++)
          strcpy(Vocabulario[k - 1], Vocabulario[k]);
        h--;
        Vocabulario = (char **)realloc(Vocabulario, sizeof(char *) * h);
      }
    }
  }
  VAR = (int **)malloc(sizeof(int *) * (h));
  for (int i = 0; i < h; i++)
    VAR[i] = (int *)calloc(sizeof(int), c[i]);

  for (int i = 0; i < h; i++) {
    aux = 0;
    for (int j = 0; j < x; j++) {
      for (int k = 0; k < c[j]; k++) {
        if (!strcmp(Vocabulario[i], terms[j][k])) {
          VAR[i][aux] = j + 1;
          aux++;
          break;
        }
      }
    }
  }

  printf("\noperacion a realizar:: 1 -> AND, 2 -> OR, 3->NOT\n");
  scanf("%d", &option);
  printf("Introduzca los ficheros a operar por numero( espacio entre ellos)\n");
  for (int i = 0; i < h; i++) {
    printf("[ %d ] = %s\n", i + 1, Vocabulario[i]);
  }
  scanf("%d %d", &op1, &op2);
  op1--;
  op2--;

  int salida = 0;
  switch (option) {
  case 1:
    while (VAR[op1][g] != 0) {
      g1 = 0;
      while (VAR[op2][g1] != 0) {
        if (VAR[op1][g] == VAR[op2][g1]) {
          resultado[g3] = VAR[op1][g];
          g3++;
          break;
        };
        g1++;
      }
      g++;
    }

    salida = g3;
    break;
  case 2:
    g = 0;
    while (VAR[op1][g] != 0) {
      resultado[g] = VAR[op1][g];
      printf("[%d] = %d\n", resultado[g], VAR[op1][g]);
      g++;
      resultado = (int *)realloc(resultado, sizeof(int) * (g + 1));
    }

    g1 = g;
    g = 0;
    while (VAR[op2][g] != 0) {
      resultado[g1 + g] = VAR[op2][g];
      g++;
      resultado = (int *)realloc(resultado, sizeof(int) * (g1 + g + 1));
    }

    count = g + g1;

    for (int i = 0; i < count; i++) {
      for (int j = (count - 1); j > i; j--) {

        if (resultado[i] == resultado[j]) {
          for (int k = j; k < (count - 1); k++) {
            resultado[k] = resultado[k + 1];
          }
          count--;
          resultado = (int *)realloc(resultado, sizeof(int) * count);
        }
      }
    }
    salida = count;
    break;
  case 3:

    while (VAR[op1][g] != 0) {
      g1 = 0;
      while (VAR[op2][g1] != 0) {
        if (VAR[op1][g] != VAR[op2][g1]) {
          resultado[g3] = VAR[op1][g];
          g3++;
          break;
        } else if (VAR[op1][g] == VAR[op2][g1]) {
          resultado[g3] = 0;
          g3++;
          break;
        }
        g1++;
      }
      g++;
    }
    salida = g3;

    break;
  default:
    return n;
    break;
  }
 
  n.r = resultado;
  n.i = salida;
  return n;
}