// Víctor Andrés Vizán
// 11 de noviembre del 2018
// Recuperación de la información --> Práctica 3


#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


// Prototipos de as funciones que se van a usar en el programa.
void menu_consulta ();
int calcular_tf (FILE *normalizado, char termino[]);
int calcular_ni (int contador);
void consulta();
FILE *normalizar_documento ();
char eliminar_tildes (char tilde);
int comprobar_tildes (char tilde);
char mayusculas_a_minusculas (char minuscula);
int eliminar_signos (char signo);
int eliminar_palabras_vacias(char palabras_vacias[], int fichero_palabras_vacias);
void buscar_palabras_vacias (int fichero_original, int fichero_palabras_vacias, int fichero_nuevo);


// Main.
void main()
{
	menu_consulta ();
}


// Módulo que muestra un menú con diferentes opciones.
void menu_consulta ()
{
	int respuesta;

	do
	{
		

		printf ("\n Menu");
		printf ("\n 1. Normalizar documento.");
		printf ("\n 2. Hacer consulta.");
		printf ("\n 3. Salir del programa.");
		printf ("\n Respuesta: ");
		scanf ("%d", &respuesta);

		if (respuesta != 3)
		{
			switch (respuesta)
			{
				case 1:
					printf("\n Indique el nombre del documento\n");
					normalizar_documento ();
					printf ("\n Documento normalizado.\n\n");
					break;

				case 2:
					consulta ();
					break;

				default:
					printf ("\n La respuesta escogida no es válida.");
			}
		}
	} while (respuesta != 3);
}


// Módulo que normaliza cierto documento.
FILE *normalizar_documento ()
{
	int fichero_original, fichero_palabras_vacias, fichero_nuevo;
	char nombre[30];
	char palabras_vacias[] = "palabras_vacias.txt";

	//printf("Indique el nombre del documento\n");
	printf (" ");
	scanf("%s",nombre);
	//printf("\n\n");

	fichero_original = open (nombre, O_RDWR);
	fichero_palabras_vacias = open (palabras_vacias, O_RDWR);

	// Cambiar la extensión que tuviera el archivo por extensión .rep
	char rep[]=".rep";
	int l = strlen (nombre);		// Longitud del archivo
	for(int i = 0; i < 4; i++)
	{
		nombre[l - 1 - i] = rep[3 - i];
	}

	// strncat (nombre, ".rep", 4);



	fichero_nuevo = creat (nombre, 00600);

	//printf("%s\n\n",nombre);

	buscar_palabras_vacias (fichero_original, fichero_palabras_vacias, fichero_nuevo);

	// Se cierran los ficheros abiertos anteriormente.
	close (fichero_original);
	close (fichero_palabras_vacias);
	close (fichero_nuevo);


	// Devolver el documento normalizado.
	FILE *normalizado = fopen (nombre, "r");
	return (normalizado);
}


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


// Módulo que devuelve en minúscula el carácter introducido en mayúscula.
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

void consulta ()
{
	int n;		// Número de documentos.
	int i;		// Contador.
	int j;		// Contador.
	int n1;		// Número de términos.
	

	printf ("\n Indique el número de documentos que va a utilizar para realizar la consulta: ");
	scanf ("%d", &n);



	FILE * normalizado [n];

	printf ("\n Indique el nombre de los documentos\n");
	for (i = 0; i < n; i++)	//Normalización de los documentos
	{
		normalizado[i] = normalizar_documento ();
	}


	printf ("\n Indique el número de términos que va a utilizar para realizar la consulta: ");
	scanf ("%d", &n1);

	char termino[n1][10000];//Array para almacenar los términos de la consulta
	printf ("\n Indique el término/s que va a utilizar en la consulta\n");
					
	for (i = 0; i < n1; i++)
	{
		printf (" ");
		scanf ("%s", termino[i]);		
	}

	printf ("\n");

	int doc[n][n1], ni[n1];
	double idf[n1], tf_idf[n][n1];

	printf("\n\n\n\n");
	printf ("			Matriz de frecuencia de términos\n\n");
	printf ("\t\t");
	for (i = 0; i < n1; i++)
	{
		printf ("t%i\t\t", i + 1);
	}

	printf ("\n");
	for (i = 0; i < n; i++)
	{
		printf (" d%d\t\t", i + 1);

		for (j = 0; j < n1; j++)
		{
			doc[i][j] = calcular_tf (normalizado[i], termino[j]);
			printf ("%d\t\t", doc[i][j]);
		}

		printf("\n");
	}

	for (i = 0; i < n1; i++)
	{
		ni[i] = 0;
	}

	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n1; j++)
		{
			if (doc[i][j] > 0)
			{
				ni[j] = ni[j] + 1;
			}

			else
			{
				ni[j] = ni[j] + 0;
			}
		}
	}

	printf (" ni\t\t");

	for (i = 0; i < n1; i++)
	{
		printf ("%d\t\t", ni[i]);
	}

	printf ("\n IDF\t\t");

	for (i = 0; i < n1; i++)
	{
		if (ni[i] == 0)
		{
			idf[i] = 0;
		}

		else
		{
			idf[i] = log10 ((double)n / (double)ni[i]);
		}
		
		printf ("%lf\t", idf[i]);
	}

	for (i = 0; i < n1; i++)
	{
		for (j = 0; j < n; j++)
		{
			tf_idf[j][i] = idf[i] * doc[j][i];
		}
	}
	
	int q[n1];

	for (i = 0; i < n1; i++)
	{
		q[i] = 1;
	}

	double abs[n];

	for (i = 0; i < n; i++)
	{
		abs[i] = 0;
		
		for (j = 0; j < n1; j++)
		{
			if (j % 2 == 0 && j <= n1 - 1)
			{
				abs[i] = abs[i] + sqrt (pow(tf_idf[i][j], 2) + pow(tf_idf[i][j + 1], 2));
			}			
		}	
	}

	double absq;

	for (i = 0; i < n1;i++)
	{
		if (i % 2 == 0)
		{
			absq = sqrt (pow (q[i],2) + pow (q[i + 1], 2));
		}
	}

	double sumatorioqd[n];

	for (i = 0; i < n; i++)
	{
		sumatorioqd[i] = 0;

		for (j = 0; j < n1; j++)
		{
			sumatorioqd[i] = sumatorioqd[i] + tf_idf[i][j];	
		}
	}

	printf("\n\n\n\n");
	printf ("\t\t\t\t\t Tabla final\n\n");


	double sim[n];

	printf ("\t\tq * d\t\t|d|\t\t|q|\t\tsim (q, d)\n");
	for (i = 0; i < n; i++)
	{
		sim[i] = sumatorioqd[i] / (absq * abs[i]);
		printf (" d%d", i + 1);
		printf ("\t\t%lf\t%lf\t%lf\t%lf\n", sumatorioqd[i], abs[i], absq, sim[i]);
	}

	printf("\n\n\n\n");
}


// Módulo que busca un término dentro de un documento.
int calcular_tf (FILE *normalizado, char termino[])
{
	char doc[80];

      	int i;
	int t1;
	int t2;
	int contador = 0;

	fseek (normalizado, 0, SEEK_SET);

	while (feof (normalizado) == 0)
	{		
		fgets (doc, 80, normalizado);

		for (i = 0; i < strlen (doc); i++)
		{

			if (termino[0] == doc[i])
			{
				t1 = 0;
				t2 = i;

				while ((termino[t1] == doc[t2]) && (t2 < strlen (doc)) && (t1 != strlen (termino)))
				{
					t1++;
					t2++;

					if (t1 == strlen (termino))
					{
						contador++;
					}
				}
			}
		}
	}

      	return contador;
}


int calcular_ni (int contador)
{
	int n;

	if (contador > 0)
	{
		n = 1;
	}

	return n;
}


int calcular_idf (int num_doc, int ni)
{
	int idf;

	idf = log (num_doc / ni);

	return idf;
}
