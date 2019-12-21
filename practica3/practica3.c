#include "practica3.h"
/**
 * @brief Comprueba parámetros nombre ficheros es .txt
 *
 * @param par - nombre ficheros introducido por parámetros
 * @param size - tamaño total de ficheros
 *
 * @return int - EXIT_SUCCESS si archivo es txt. EXIT_FAILURE caso contrario
 */

int validateTXT(char **par, int size) {
  for (int i = 0; i < size; i++) {
    if (checkText((char *)par[i]) == EXIT_FAILURE) {
      fprintf(stderr, "Bad extension\n");
      return (EXIT_FAILURE);
    }
  }
  return EXIT_SUCCESS;
}

/**
 * @brief Comprueba si fichero es .txt
 *
 * @param f - cadena fichero
 * @return int - EXIT_SUCCESS si archivo es txt. EXIT_FAILURE caso contrario
 */
int checkText(char *f) {
  int t = strlen(f) + 1;
  return f[t] != 't' && f[t - 1] != 'x' && f[t - 2] != 't' && f[t - 3] != '.'
             ? EXIT_FAILURE
             : EXIT_SUCCESS;
}

/**
 * @brief Abre los ficheros introducidos por parámetro
 *
 * @param par - puntero a cadena de carácter con nombre de los ficheros
 * @param size - tamaño totla de ficheros
 * @return int* - devuelve descriptores de los ficheros
 */
int *openFile(char **par, int size) {
  int *files = (int *)malloc(sizeof(int) * size);

  for (int i = 0; i < size; i++) {
    if ((files[i] = open(par[i], O_RDWR | O_CREAT, 0644)) == -1)
      return NULL;
  }
  return files;
}

/**
 * @brief Create a Rep object
 *
 * @param param - puntero a cadenas de caracteres
 * @param size - tamaño total de ficheros
 * @return char**
 */
char **createRep(char **param, int size) {
  char **rep = (char **)malloc(sizeof(char *) * size);
  int t = 0;
  for (int i = 0; i < size; i++) {
    rep[i] = (char *)malloc(sizeof(char) * (strlen(param[i]) + 1));
  }

  for (int i = 0; i < size; i++) {
    t = strlen(param[i]) + 1;
    strcpy(rep[i], param[i]);

    rep[i][t - 2] = 'p';
    rep[i][t - 3] = 'e';
    rep[i][t - 4] = 'r';
  }
  return rep;
}

/**
 * @brief Separa cadena de caracteres de un fichero, mediante una lista enlazada
 *
 * @param stopwords - descriptor del fichero
 * @return NODO* - devuelve lista enlazada
 */
NODO *tokenizeFile(int stopwords) {
  const char *delim = " , .";
  NODO *pv = NULL;
  int f = lseek(stopwords, 0, SEEK_END);
  char *buffer = malloc(sizeof(char) * f), *token;

  lseek(stopwords, 0, SEEK_SET);
  read(stopwords, buffer, sizeof(char) * f);

  token = strtok(buffer, delim);
  int cont = 1;
  while (token != NULL) {
    enlazar(&pv, token, cont);
    token = strtok(NULL, delim);
    cont++;
  }

  return pv;
}
/**
 * @brief crea un nodo nuevo
 *
 * @param nodo - nodo inicio
 * @param string - puntero a cadena a asignar a nuevo nodo
 * @param i - numero de nodo
 */

void enlazar(NODO **nodo, char *string, int i) {
  NODO *nuevo = (NODO *)malloc(sizeof(NODO));
  if (nuevo == NULL)
    perror("Memoria:");
  else {
    strcpy(nuevo->string, string);
    nuevo->i = i;
    nuevo->t = strlen(string) + 1;
    if (*nodo == NULL)
      nuevo->link = NULL;
    else
      nuevo->link = *nodo;
    *nodo = nuevo;
  }
}
/**
 * @brief Crea un consulta
 *
 * @return NODO* devuelve consulta
 */

NODO *addQuery() {
  NODO *query = NULL;
  const char *delim = " \n";
  char *token = NULL, aux[100], *string;
  int i = 0;
  printf("Introduzca una consulta\n");
  fgets(aux, 100, stdin);
  string = token = (char *)malloc(sizeof(char) * (strlen(aux) + 1));

  strcpy(string, aux);
  token = strtok(aux, delim);
  while (token != NULL) {
    enlazar(&query, token, i++);
    token = strtok(NULL, delim);
  }
  return query;
}
/**
 * @brief imprime lista enlazada
 *
 * @param lista - lista a imprimir
 */

void imprimirlista(NODO **lista) {
  NODO *i = *lista;
  while (i != NULL) {
    printf("[%d] ->%s\n", i->i, i->string);
    i = i->link;
  }
}
/**
 * @brief normaliza el fichero consultado
 *
 * @param file - lista de cadena de caracteres del fichero
 * @param pv - palabrasVacías necesario para normalizar el fichero
 * @return NODO* - devuelve lista de fichero normalizado
 */

NODO *normalizar(NODO *file, NODO *pv) {
  int i = 0, save = 1;
  NODO *aux = file, *out = NULL, *auxPv;
  while (aux != NULL) {
    save = 1;
    auxPv = pv;

    while (auxPv != NULL) {
      if (!strcmp(aux->string, auxPv->string)) {
        save = 0;
        break;
      }
      auxPv = auxPv->link;
    }

    if (save) {
      enlazar(&out, aux->string, i);
      i++;
    }
    aux = aux->link;
  }

  return out;
}
/**
 * @brief inicializa el programa
 *
 * @param par - puntero a cadena de caracteres
 * @param size - tamaño total de ficheros
 * @param files - descriptor de los ficheros .txt
 * @param rep - descriptr de los ficheros .rep
 * @return int - devuelve si el programa se ha iniciado con éxito
 */

int init(char **par, int size, int **files, int **rep) {
  *files = (int *)openFile(par, size);
  *rep = NULL;
  char **frep;

  if (files == NULL) {
    perror("files txt");
    return EXIT_FAILURE;
  }

  frep = (char **)createRep(par, size);

  if ((*rep = (int *)openFile(frep, size)) == NULL) {
    perror("files .rep");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
/**
 * @brief vector total de todas las listas enlazadas
 *
 * @param l - lista enlazada total pasada por referencia
 * @param file - lista enlazada por archivo pasada por valor
 * @param i - número asignado a la nueva lista
 */

void vector(NODO **l, NODO *file, int *i) {
  NODO *aux = file;
  while (aux != NULL) {
    enlazar(l, aux->string, (*i)++);
    aux = aux->link;
  }
}
/**
 * @brief busca cadena repetidas en la lista enlazada
 *
 * @param nodo - lista a procesar
 * @return NODO* - devuelve cadena repetida, NULL en caso contrario
 */

NODO *repeat(NODO *nodo) {
  NODO *i = nodo, *j = nodo;

  i = i->link;
  while (i != NULL) {
    j = nodo;
    while (j != NULL && i != j) {
      if (!strcmp(i->string, j->string))
        return i;
      j = j->link;
    }
    i = i->link;
  }
  return NULL;
}
/**
 * @brief genera la lista definitiva para procesar los ficheros
 *
 * @param nodo - devuelve nodo definitivo
 */

void E1(NODO **nodo) {
  NODO *aux = *nodo, *i = NULL;
  NODO *j = NULL;
  while (aux != NULL) {
    j = aux;
    while (j != NULL) {
      if ((i = repeat(aux)) != NULL) {
        deleteNodo(nodo, i);
      }
      j = j->link;
    }
    aux = aux->link;
  }
}
/**
 * @brief Elimina todo para su posterior procesamiento
 *
 * @param l - lista a procesar
 * @param p - nodo a eliminar de la lista
 */

void deleteNodo(NODO **l, NODO *p) {
  NODO *pre;
  if (*l == NULL)
    perror("lista:");
  else {
    if (p == *l)
      *l = (*l)->link;
    else {
      pre = *l;
      while (pre->link != p)
        pre = pre->link;
      pre->link = p->link;
    }
    free(p);
  }
}
/**
 * @brief genera el vector de términos por fichero.
 *
 * @param nodo - nodo del fichero
 * @param pv - nodo de las palabras vacías
 * @return int* devuelve array resultante
 */

int *vectorSpaceModel(NODO *nodo, NODO *pv) {
  NODO *aux = nodo, *i = pv;
  NODO *t = pv;
  int x = 0;

  while (t != NULL) {
    t = t->link;
    x++;
  }
  int *vector = (int *)calloc(x, sizeof(int)), j = 0;

  while (i != NULL) {
    aux = nodo;

    while (aux != NULL) {
      if (!strcmp(aux->string, i->string)) {

        vector[j] = 1;
      }
      aux = aux->link;
    }

    i = i->link;
    j++;
  }

  i = pv;
  return vector;
}
/**
 * @brief producto cartesiano de la consulta y el fichero ya procesado
 *
 * @param p - vector del fichero
 * @param q -vector de la consulta
 * @param l - tamaño del vector de la consulta
 * @return int - cevuelve sim(q,d)
 */

int product(int *p, int *q, int l) {
  int r = 0;
  for (int i = 0; i < l; i++) {
    r += p[i] * q[i];
  }
  return r;
}
/**
 * @brief programa final
 *
 * @param arg - tamaño de los ficheros
 * @param aux - lista enlazada de los documentos
 * @param files - descriptores de los ficheros
 * @param pv - descriptor del fichero de palabras vacías
 * @param l - nodo del vector resultante
 * @param r - puntero a array que contiene los vectores de términos de los
 * ficheros
 * @param lengt - tamaño del vector de términos
 * @param q - nodo de la consulta
 * @param Q - array que contiene los términos de la consulta.
 */

void final(int arg, NODO **aux, int *files, int pv, NODO **l, int **r,
           int *lengt, NODO **q, int **Q) {
  int t = 0;
  NODO *x;

  for (int i = 0; i < arg; i++) {
    aux[i] = normalizar(tokenizeFile(files[i]), tokenizeFile(pv));
    vector(l, aux[i], &t);
    E1(l);
  }

  *Q = (int *)vectorSpaceModel(*q, *l);
  x = *l;

  while (x != NULL) {
    (*lengt)++;
    x = x->link;
  }

  for (int i = 0; i < arg; i++) {
    r[i] = vectorSpaceModel(aux[i], *l);
    product(r[i], *Q, *lengt);
  }
}
/**
 * @brief muestra por pantalla el resultado del programa
 *
  * @param aux - lista enlazada de los documentos
  * @param lengt - tamaño del vector de términos
  * @param r - puntero a array que contiene los vectores de términos de los
 ficheros
  * @param Q - array que contiene los términos de la consulta.
 * @param l - nodo del vector resultante
  * @param arg - tamaño de los ficheros
 */

void screen(NODO **aux, int lengt, int **r, int *Q, NODO *l, int arg) {
  for (int i = 0; i < arg; i++) {
    printf("{\n\n");
    imprimirlista(&aux[i]);
    jump;
    imprimirlista(&l);
    printf("(");
    for (int j = 0; j < lengt; j++)
      printf(" %d ", r[i][j]);
    printf(")");

    printf(" X ");

    printf("(");
    for (int j = 0; j < lengt; j++)
      printf(" %d ", Q[j]);
    printf(")");

    printf("\nsim(d%d,q) = %d\n", i + 1, product(r[i], Q, lengt));
    printf("\n}\n");
    printf("-----------------------------------\n");
    jump;
    jump;
  }
}
/**
 * @brief Escribe en fichero .rep
 * 
 * @param rep - descriptores de los ficheros rep
 * @param nodo - array de nodos a escribir en ficheros 
 * @param arg - número total del .rep
 */
void writeRep(int *rep, NODO **nodo, int arg) {
  NODO **j = nodo;
  for (int i = 0; i < arg; i++) {
    lseek(rep[i], 0, SEEK_SET);
    while (j[i] != NULL) {
      j[i]->string[strlen(j[i]->string) + 1] = ' ';
      write(rep[i], j[i]->string, strlen(j[i]->string) + 1);
      j[i] = j[i]->link;
    }
  }
}
