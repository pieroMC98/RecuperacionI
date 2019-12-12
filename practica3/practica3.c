#include "practica3.h"

int validateTXT(char **par, int size) {
  for (int i = 0; i < size; i++) {
    if (checkText((char *)par[i]) == EXIT_FAILURE) {
      fprintf(stderr, "Bad extension\n");
      return (EXIT_FAILURE);
    }
  }
  return EXIT_SUCCESS;
}

int checkText(char *f) {
  int t = strlen(f) + 1;
  return f[t] != 't' && f[t - 1] != 'x' && f[t - 2] != 't' && f[t - 3] != '.'
             ? EXIT_FAILURE
             : EXIT_SUCCESS;
}

int *openFile(char **par, int size) {
  int *files = (int *)malloc(sizeof(int) * size);

  for (int i = 0; i < size; i++) {
    if ((files[i] = open(par[i], O_RDONLY | O_CREAT, 0644)) == -1)
      return NULL;
  }
  return files;
}

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

void imprimirlista(NODO **lista) {
  NODO *i = *lista;
  while (i != NULL) {
    printf("[%d] ->%s\n", i->i, i->string);
    i = i->link;
  }
}

NODO *normalizar(NODO *file, NODO *pv) {
  int i = 0, save = 1;
  char *buffer = NULL;
  // NODO *n_file = tokenizeFile(file);
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

  //imprimirlista(&out);
  return out;
}

int init(char **par, int size, int **files, int **rep) {
  *files = (int *)openFile(par, size);
  *rep = NULL;
  char **frep;
  NODO *nodo;

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