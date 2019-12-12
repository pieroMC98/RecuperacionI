#include "practica3.h"
int main(int argc, char const *argv[]) {
  int *files = NULL, *filesREP = NULL;
  NODO *nodo, *file;
  char **param = (char **)argv;
  int arg = argc - 1;

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

  int pv = open("palabrasVacias", O_RDONLY, 0777);
  nodo = tokenizeFile(pv);
  // imprimirlista(&nodo);
  
  file = tokenizeFile(files[0]);
  NODO *n = normalizar(file,nodo);
  imprimirlista(&n);

  return EXIT_SUCCESS;
}
