#include "practica3.h"

int main(int argc, char const *argv[]) {
  int *files = NULL, *filesREP = NULL, **vectors = NULL;

  char **param = (char **)argv;
  int arg = argc - 1;
  int lengt = 0;
  int **r = (int **)malloc(sizeof(int *) * arg);
  NODO *q = addQuery();
  int *Q = NULL;

  vectors = (int **)malloc(sizeof(int) * arg);
  for (int i = 0; i < arg; i++)
    vectors[i] = (int *)malloc(sizeof(int) * 10);

  int pv = open("palabrasVacias", O_RDONLY, 0777);

  NODO **aux = (NODO **)malloc(arg * sizeof(NODO *)), *l = NULL;
  for (int i = 0; i < arg; i++)
    aux[i] = NULL;

  if (argc < 2) {
    printf("Parametros insuficientes\n");
    exit(1);
  }
  for (int i = 0; i < argc - 1; i++)
    strcpy(param[i], argv[i + 1]);

  if (validateTXT((char **)param, arg) == EXIT_FAILURE) {
    perror("ficheros con extension incorrecta\n");
    return EXIT_FAILURE;
  }

  if (init((char **)argv, arg, &files, &filesREP) == EXIT_FAILURE) {
    perror("Error al iniciar el programa\n");
    return EXIT_FAILURE;
  }

  final(arg, aux, files, pv, &l, r, &lengt, &q, &Q);
  screen(aux, lengt, r, Q, l, arg);
  writeRep(filesREP, aux, arg);
  
  for (int i = 0; i < arg; i++) {
    close(files[i]);
    close(filesREP[i]);
  }

  return EXIT_SUCCESS;
}
