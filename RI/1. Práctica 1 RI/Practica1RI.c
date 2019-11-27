#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>		// Para la función tolower ().
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


// Prototipos de las funciones que se van a usar en el programa.
void mostrar_menu ();
char eliminar_tildes (char tilde);
int comprobar_tildes (char tilde);
char mayusculas_a_minusculas (char minuscula);
int eliminar_signos (char signo);
int eliminar_palabras_vacias(char palabras_vacias[], int fichero_palabras_vacias);
void buscar_palabras_vacias (int fichero_original, int fichero_palabras_vacias, int fichero_nuevo);
void visualizar_documento();
void visualizar_documento_normalizado (char *nombre);
void normalizar_documento ();


// Main.
void main ()
{
	char palabras_vacias[] = "stopwords.txt";	// Se guarda en una cadena el texto de las palabras vacías.

	mostrar_menu ();		// Se muestra el menú con las diferentes opciones.
}


// Se muestra un menú con las distintas opciones que el programa puede llevar a cabo.
void mostrar_menu ()
{
	int respuesta;

	do
	{
		// Se muestra un menú con distintas opciones de lo que puede hacer el programa.
		printf ("\n ¿Qué quiere hacer? ");
		printf ("\n 1 - Ver la lista de documentos.");
		printf ("\n 2 - Visualizar cierto documento en su versión original.");
		printf ("\n 3 - Visualizar cierto documento en su versión limpia y normalizada.");
		printf ("\n 4 - Acabar y salir del programa.\n\n  ");

		scanf ("%d", &respuesta);	// Se lee la respuesta introducida por teclado.

		if (respuesta != 4)
		{
			switch (respuesta)
			{
				case 1:
					system ("ls");
					break;

				case 2:
					visualizar_documento ();
					break;

				case 3:
					normalizar_documento ();
					break;

				default:
					printf ("\n La respuesta escogida no es válida.");
			}
		}

	} while (respuesta != 4);
}

//////////////////////////////		Falta la ü y la ñ		///////////////////////////////

// Módulo que elimina la tilde de una letra si esa letra lleva tilde.
// Devuelve la letra sin tilde si esta la lleva.
char eliminar_tildes (char tilde)
{
	if (tilde == 0xffffff81)
	{
		tilde = 'a';
	}

	else if (tilde == 0xffffff89)
	{
		tilde = 'e';
	}

	else if (tilde == 0xffffff8d)
	{
		tilde = 'i';
	}

	else if (tilde == 0xffffff93)
	{
		tilde = 'o';
	}

	else if (tilde == 0xffffff9a)
	{
		tilde = 'u';
	}

	else if (tilde == 0xffffffa1)
	{
		tilde = 'a';
	}

	else if (tilde == 0xffffffa9)
	{
		tilde = 'e';
	}

	else if (tilde == 0xffffffad)
	{
		tilde = 'i';
	}

	else if (tilde == 0xffffffb3)
	{
		tilde='o';
	}

	else if (tilde == 0xffffffba)
	{
		tilde = 'u';
	}

	return tilde;
}


// Módulo que comprueba si una letra lleva tilde.
int comprobar_tildes (char tilde)
{
	if (tilde == 0xffffffc3)
	{
		return 1;
	}

	else
	{
		return 0;
	}
}


// Módulo que devuelve en minúscula el carácter introducido en mayúscula.		/////////// Cambiar módulo a nuevo módulo.
// Cambiarlo también en el prototipo.
char mayusculas_a_minusculas (char minuscula)
{
	return tolower (minuscula);
}


// Módulo que quita los signos de puntuación en el texto si hubiera alguno.
int eliminar_signos (char signo)
{
	if ((signo == '!') || (signo == '?'))
	{
		return 1;
	}

	else if (((int) signo == 173) || ((int) signo == 168))
	{
		return 1;
	}

	else if ((signo == ',') || (signo == '.') ||(signo == ';') || (signo == ':') || (signo == '"'))
	{
		return 1;
	}

	else
	{
		return 0;
	}
}


// Módulo que quita las palabras vacías que haya en el texto usando un documento auxiliar en el que se encuentran esas palabras vacías.
int eliminar_palabras_vacias(char palabras_vacias[], int fichero_palabras_vacias)
{
	int j = 0;
	int tam;
	char aux;
	char buffer_aux[50];

	tam = lseek (fichero_palabras_vacias, 0, SEEK_END);

	lseek (fichero_palabras_vacias, 0, SEEK_SET);

	for (int i = 0; i < tam; i++)
	{
		memset (buffer_aux, '\0', 50);
		read (fichero_palabras_vacias, &aux, sizeof (char));

		while ((aux != ' ') && (aux != '\n') && (j < tam))
		{
			buffer_aux[j] = aux;
			j++;

			read (fichero_palabras_vacias, &aux, sizeof (char));
			i++;
		}

		j = 0;

		if (! (strcmp (palabras_vacias, buffer_aux)))
		{
			return 1;
		}
	}

	return 0;
}


// Busca las palabras vacias en el texto.
void buscar_palabras_vacias (int fichero_original, int fichero_palabras_vacias, int fichero_nuevo)
{
	char buffer[50];
	char aux = 'a';
	int i = 0;
	int control = 0;
	int tam;

	tam = lseek (fichero_original, 0, SEEK_END);

	lseek (fichero_original, 0, SEEK_SET);

	for (int j = 0; j < tam; j++)
	{
		read (fichero_original, &aux, sizeof (char));	
		i = 0;

		while ((aux != ' ') && (aux != '\n') && (j < tam))
		{
			if (comprobar_tildes (aux) == 1)
			{
				read (fichero_original, &aux, sizeof (char));	
				j++;

				aux = eliminar_tildes (aux);
			}
			control = eliminar_signos (aux);

			if (control == 0)
			{
				aux = mayusculas_a_minusculas (aux);
				buffer[i] = aux;
				i++;
			}

			else
			{
				control = 0;
			}

			if (j < tam)
			{
				read (fichero_original, &aux, sizeof (char));
				j++;
			}
		}

		if (eliminar_palabras_vacias (buffer, fichero_palabras_vacias) == 0)
		{
			write (fichero_nuevo, buffer, i * sizeof (char));

			if (j < tam)
			{
				write (fichero_nuevo, &aux, sizeof (char));
			}
		}

		memset (buffer, '\0', 50);
	}
}


// Modulo que visualiza el documento.
void visualizar_documento()
{
	int fichero_original;
	int tam;
	char nombre[30];
	char aux;

	printf("Introduzca el nombre del documento: \n");
	scanf("%s", nombre);
	printf("\n\n");

	fichero_original = open (nombre, O_RDWR);
	tam = lseek (fichero_original, 0, SEEK_END);
	lseek (fichero_original, 0, SEEK_SET);

	for (int i = 0; i < tam; i++)
	{
		read (fichero_original, &aux, sizeof (char));
		fprintf (stdout, "%c", aux);
	}

	close (fichero_original);		// Se cierra el fichero.
}


// Módulo que visualiza el documento limpio y normalizado.
void visualizar_documento_normalizado (char *nombre)
{
	int tam;
	int fichero_original;
	char aux;

	fichero_original = open (nombre, O_RDWR);
	tam = lseek (fichero_original, 0, SEEK_END);
	lseek (fichero_original, 0, SEEK_SET);

	for (int i = 0; i < tam; i++)
	{
		read (fichero_original, &aux, sizeof (char));
		fprintf (stdout, "%c", aux);
	}

	close (fichero_original);		// Se cierra el fichero.
}


// Módulo que normaliza cierto documento.
void normalizar_documento ()
{
	int fichero_original, fichero_palabras_vacias, fichero_nuevo;
	char nombre[30];
	char palabras_vacias[] = "stopwords.txt";

	printf("Indique el nombre del documento\n");
	scanf("%s",nombre);
	printf("\n\n");

	fichero_original = open (nombre, O_RDWR);
	fichero_palabras_vacias = open (palabras_vacias, O_RDWR);

	strncat (nombre, ".rep", 4);

	fichero_nuevo = creat (nombre, 00600);

	printf("%s\n\n",nombre);

	buscar_palabras_vacias (fichero_original, fichero_palabras_vacias, fichero_nuevo);

	// Se cierran los ficheros abiertos anteriormente.
	close (fichero_original);
	close (fichero_palabras_vacias);
	close (fichero_nuevo);

	visualizar_documento_normalizado (nombre);
}




