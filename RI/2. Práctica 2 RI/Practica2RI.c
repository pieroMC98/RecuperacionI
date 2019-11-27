// Víctor Andrés Vizán
// 25 de octubre del 2018
// Recuperación de la información --> Práctica 2

#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// Prototipos de las funciones que se van a usar en el programa.
void menu_consulta();
void consultar();
void consulta(int respuesta, char *termino, int n, int n1, int *array_resultado,
              FILE *normalizado[]);
int buscar_termino(FILE *normalizado, char termino[]);
void leer_cadena(char *cadena);
FILE *normalizar_documento();
char eliminar_tildes(char tilde);
int comprobar_tildes(char tilde);
char mayusculas_a_minusculas(char minuscula);
int eliminar_signos(char signo);
int eliminar_palabras_vacias(char palabras_vacias[],
                             int fichero_palabras_vacias);
void buscar_palabras_vacias(int fichero_original, int fichero_palabras_vacias,
                            int fichero_nuevo);

// Main
void main() { menu_consulta(); }

// Módulo que muestra un menú.
void menu_consulta() {
  int respuesta;

  do {

    printf("\n Menu");
    printf("\n 1. Normalizar documento.");
    printf("\n 2. Hacer consulta.");
    printf("\n 3. Salir del programa.");
    printf("\n Respuesta: ");
    scanf("%d", &respuesta);

    if (respuesta != 3) {
      switch (respuesta) {
      case 1:
        printf("\n Indique el nombre del documento\n");
        normalizar_documento();
        printf("\n Documento normalizado.\n\n");
        break;

      case 2:
        consultar();
        break;

      default:
        printf("\n La respuesta escogida no es válida.");
      }
    }
  } while (respuesta != 3);
}

// Módulo que inicializa valores y hace consultas.
void consultar() {
  char *token;

  char cadena[500]; // Permite guardar una cadena de 50 cadenas, cada una de 500
                    // caracteres como maximo.
  char s[2] = " ";

  // char cadena_2[50][500];
  char **cadena_2 = (char **)malloc(50 * sizeof(char *));
  for (int i = 0; i < 50; i++) {
    cadena_2[i] = (char *)malloc(500 * sizeof(char));
  }

  printf("\n ");

  int contador = 0;

  leer_cadena(cadena);

  token = strtok(cadena, s);

  while (token != NULL) {
    cadena_2[contador] = token;

    // printf (" %s\n", token);
    contador++; // Va a llevar la cuenta de las palabras que tiene la cadena
                // introducida.
    token = strtok(NULL, s);
  }

  char **termino = (char **)malloc(50 * sizeof(char *));
  for (int i = 0; i < 50; i++) {
    termino[i] = (char *)malloc(500 * sizeof(char));
  }

  int n;
  printf("\n Indique el número de documentos que va a utilizar para realizar "
         "la consulta: ");
  scanf("%d", &n);
  printf("\n");

  int *array_resultado = (int *)malloc(n * sizeof(int));
  for (int j = 0; j < n; j++) {
    array_resultado[j] = 1;
  }

  int ctr = 0;
  for (int i = 0; i < contador; i += 2) {
    if ((strcmp(cadena_2[i], "AND") != 0) && (strcmp(cadena_2[i], "OR") != 0)) {
      termino[ctr] = cadena_2[i];
      ctr++;
    }
  }

  FILE *normalizado[n];

  printf(" Introduzca los nombres de los documentos que va a utilizar el "
         "programa: \n");
  for (int i = 0; i < n; i++) // Normalización de los documentos
  {
    normalizado[i] = normalizar_documento();
  }

  int poy = 0;
  int contador_terminos = 0;

  // Primer término de la consulta (primera palabra).
  contador_terminos++;
  consulta(0, termino[poy], n, contador_terminos, array_resultado, normalizado);
  poy++;

  for (int i = 1; i < contador; i += 2) {
    if (strcmp(cadena_2[i], "AND") == 0) {
      contador_terminos = contador_terminos + 2;
      consulta(1, termino[poy], n, contador_terminos, array_resultado,
               normalizado);
      poy++;
    }

    else if (strcmp(cadena_2[i], "OR") == 0) {
      contador_terminos = contador_terminos + 2;
      consulta(2, termino[poy], n, contador_terminos, array_resultado,
               normalizado);
      poy++;
    }

    else if (strcmp(cadena_2[i], "NOT") == 0) {
      contador_terminos = contador_terminos + 2;
      consulta(3, termino[poy], n, contador_terminos, array_resultado,
               normalizado);
      poy++;
    }

    else {
      printf("\n La consulta no es válida ((AND - OR) --> opciones válidas).");
    }
  }

  printf("\n Resultado: {");
  for (int i = 0; i < n; i++) {
    if (array_resultado[i]) {
      printf(" d%d", i + 1);
    }
  }
  printf(" }\n\n");

  printf("\n\n");
}

// Módulo que lee una cadena aunque haya espacios entre los caracteres.
void leer_cadena(char *cadena) {
  printf("\n Introduzca la consulta: ");
  scanf(" %[^\n]", cadena); // Lee toda la cadena aunque haya espacios en
                            // blanco.		// Lee por teclado la cadena.
}

// Módulo en el que se realiza la consulta con los documentos y los términos
// indicados.
void consulta(int respuesta, char *termino, int n, int n1, int *array_resultado,
              FILE *normalizado[]) {
  int i; // Contador

  int operacion; // Variable para indicar el tipo de operación (OR/AND)
  int num;       // Variable para verificar el resultado de la operación.
  int num2;      // Variable para verificar el resultado de la operación.
  int j;         // Contador

  if (n1 > 1) // Si hay más de un término en la consulta se pregunta por el tipo
              // de operación
  {
    if (respuesta == 1) ///////////////// AND ///////////////////////
    {
      for (i = 0; i < n; i++) {
        num2 = 0;

        for (j = 0; j < n1; j++) {
          num = buscar_termino(normalizado[i], termino);
          num2 = num + num2;
          fseek(normalizado[i], 0, SEEK_SET);
        }

        if ((num2 > 0) && (array_resultado[i])) // 1 porque está en el array.
        {
          array_resultado[i] = 1;
        }

        else {
          array_resultado[i] = 0;
        }
      }
    }

    else if (respuesta == 2) ////////////////////// OR ////////////////////////
    {
      for (i = 0; i < n; i++) {
        num2 = 0;
        for (j = 0; j < n1; j++) {
          num = buscar_termino(normalizado[i], termino);
          num2 = num + num2;
          fseek(normalizado[i], 0, SEEK_SET);
        }

        if ((num2 > 0) || (array_resultado[i])) {
          array_resultado[i] = 1;
        }

        else {
          array_resultado[i] = 0;
        }
      }
    }

    else if (respuesta == 3) ////////////////////// NOT ////////////////////////
    {
      for (i = 0; i < n; i++) {
        num2 = 0;
        for (j = 0; j < n1; j++) {
          num = buscar_termino(normalizado[i], termino);
          num2 = num + num2;
          fseek(normalizado[i], 0, SEEK_SET);
        }

        if ((array_resultado[i]) &&
            (num2 > 0)) // Si esta en el primero, y en el segundo, es 0.
        {
          array_resultado[i] = 0;
        }

        else if ((array_resultado[i]) &&
                 (num == 0)) // Si está en el primero, y no en el segundo, se
                             // queda, y es 1.
        {
          array_resultado[i] = 1;
        }

        else // De cualquier otra forma, es 0.
        {
          array_resultado[i] = 0;
        }
      }
    }

    else // Opción no válida
    {
      printf(" La opcion escogida no es valida.\n");
      exit(1); // Se sale del programa (cambiar).
    }
  }

  else // Else por si solo se ha introducido un término.
  {
    for (i = 0; i < n; i++) {
      num2 = 0;

      for (j = 0; j < n1; j++) {
        num = buscar_termino(normalizado[i], termino);
        num2 = num + num2;
        fseek(normalizado[i], 0, SEEK_SET);
      }

      if (num2 > 0) {
        array_resultado[i] = 1;
      }

      else {
        array_resultado[i] = 0;
      }
    }
  }
}

// Módulo que normaliza cierto documento.
FILE *normalizar_documento() {
  int fichero_original, fichero_palabras_vacias, fichero_nuevo;
  char nombre[30];
  char palabras_vacias[] = "palabras_vacias.txt";

  // printf("Indique el nombre del documento\n");
  printf(" ");
  scanf("%s", nombre);
  // printf("\n\n");

  fichero_original = open(nombre, O_RDWR);
  fichero_palabras_vacias = open(palabras_vacias, O_RDWR);

  // Cambiar la extensión que tuviera el archivo por extensión .rep
  char rep[] = ".rep";
  int l = strlen(nombre); // Longitud del archivo
  for (int i = 0; i < 4; i++) {
    nombre[l - 1 - i] = rep[3 - i];
  }

  // strncat (nombre, ".rep", 4);

  fichero_nuevo = creat(nombre, 00600);

  // printf("%s\n\n",nombre);

  buscar_palabras_vacias(fichero_original, fichero_palabras_vacias,
                         fichero_nuevo);

  // Se cierran los ficheros abiertos anteriormente.
  close(fichero_original);
  close(fichero_palabras_vacias);
  close(fichero_nuevo);

  // Devolver el documento normalizado.
  FILE *normalizado = fopen(nombre, "r");
  return (normalizado);
}

// Módulo que busca un término dentro de un documento.
int buscar_termino(FILE *normalizado, char termino[]) {
  char doc[80];

  int t1;
  int t2;
  int contador = 0;

  while (feof(normalizado) == 0) {
    fgets(doc, 80, normalizado);

    for (int i = 0; i < strlen(doc); i++) {

      if (termino[0] == doc[i]) {

        t1 = 0;
        t2 = i;

        while ((termino[t1] == doc[t2]) && (t2 < strlen(doc)) &&
               (t1 != strlen(termino))) {
          t1++;
          t2++;

          if (t1 == strlen(termino)) {
            contador++;
          }
        }
      }
    }
  }

  if (contador > 0) {
    return 1;
  }

  else {
    return 0;
  }

  getchar();
}

// Módulo que elimina la tilde de una letra si esa letra lleva tilde.
// Devuelve la letra sin tilde si esta la lleva.
char eliminar_tildes(char tilde) {
  if (tilde == 0xffffff81) {
    tilde = 'a';
  }

  else if (tilde == 0xffffff89) {
    tilde = 'e';
  }

  else if (tilde == 0xffffff8d) {
    tilde = 'i';
  }

  else if (tilde == 0xffffff93) {
    tilde = 'o';
  }

  else if (tilde == 0xffffff9a) {
    tilde = 'u';
  }

  else if (tilde == 0xffffffa1) {
    tilde = 'a';
  }

  else if (tilde == 0xffffffa9) {
    tilde = 'e';
  }

  else if (tilde == 0xffffffad) {
    tilde = 'i';
  }

  else if (tilde == 0xffffffb3) {
    tilde = 'o';
  }

  else if (tilde == 0xffffffba) {
    tilde = 'u';
  }

  return tilde;
}

// Módulo que comprueba si una letra lleva tilde.
int comprobar_tildes(char tilde) {
  if (tilde == 0xffffffc3) {
    return 1;
  }

  else {
    return 0;
  }
}

// Módulo que devuelve en minúscula el carácter introducido en mayúscula.
// /////////// Cambiar módulo a nuevo módulo. Cambiarlo también en el prototipo.
char mayusculas_a_minusculas(char minuscula) { return tolower(minuscula); }

// Módulo que quita los signos de puntuación en el texto si hubiera alguno.
int eliminar_signos(char signo) {
  if ((signo == '!') || (signo == '?')) {
    return 1;
  }

  else if (((int)signo == 173) || ((int)signo == 168)) {
    return 1;
  }

  else if ((signo == ',') || (signo == '.') || (signo == ';') ||
           (signo == ':') || (signo == '"')) {
    return 1;
  }

  else {
    return 0;
  }
}

// Módulo que quita las palabras vacías que haya en el texto usando un documento
// auxiliar en el que se encuentran esas palabras vacías.
int eliminar_palabras_vacias(char palabras_vacias[],
                             int fichero_palabras_vacias) {
  int j = 0;
  int tam;
  char aux;
  char buffer_aux[50];

  tam = lseek(fichero_palabras_vacias, 0, SEEK_END);

  lseek(fichero_palabras_vacias, 0, SEEK_SET);

  for (int i = 0; i < tam; i++) {
    memset(buffer_aux, '\0', 50);
    read(fichero_palabras_vacias, &aux, sizeof(char));

    while ((aux != ' ') && (aux != '\n') && (j < tam)) {
      buffer_aux[j] = aux;
      j++;

      read(fichero_palabras_vacias, &aux, sizeof(char));
      i++;
    }

    j = 0;

    if (!(strcmp(palabras_vacias, buffer_aux))) {
      return 1;
    }
  }

  return 0;
}

// Busca las palabras vacias en el texto.
void buscar_palabras_vacias(int fichero_original, int fichero_palabras_vacias,
                            int fichero_nuevo) {
  char buffer[50];
  char aux = 'a';
  int i = 0;
  int control = 0;
  int tam;

  tam = lseek(fichero_original, 0, SEEK_END);

  lseek(fichero_original, 0, SEEK_SET);

  for (int j = 0; j < tam; j++) {
    read(fichero_original, &aux, sizeof(char));
    i = 0;

    while ((aux != ' ') && (aux != '\n') && (j < tam)) {
      if (comprobar_tildes(aux) == 1) {
        read(fichero_original, &aux, sizeof(char));
        j++;

        aux = eliminar_tildes(aux);
      }
      control = eliminar_signos(aux);

      if (control == 0) {
        aux = mayusculas_a_minusculas(aux);
        buffer[i] = aux;
        i++;
      }

      else {
        control = 0;
      }

      if (j < tam) {
        read(fichero_original, &aux, sizeof(char));
        j++;
      }
    }

    if (eliminar_palabras_vacias(buffer, fichero_palabras_vacias) == 0) {
      write(fichero_nuevo, buffer, i * sizeof(char));

      if (j < tam) {
        write(fichero_nuevo, &aux, sizeof(char));
      }
    }

    memset(buffer, '\0', 50);
  }
}
