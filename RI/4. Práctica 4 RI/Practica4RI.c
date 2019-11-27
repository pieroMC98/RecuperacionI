// Víctor Andrés Vizán
// 71040252-Z
// 12 de diciembre del 2018
// Recuperación de la información --> Práctica 4 --> Modelo probabilístico


// Librerías que se van a usar en el programa
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <math.h>


// Estructuras.
struct par {
	double termino;
	int posicion;
};

typedef struct par PAR;


// Prototipos de las funciones que se van a usar en el programa.
void mostrar_menu ();
char eliminar_tildes (char tilde);
int comprobar_tildes (char tilde);
char mayusculas_a_minusculas (char minuscula);
int eliminar_signos (char signo);
int eliminar_palabras_vacias(char palabras_vacias[], int fichero_palabras_vacias);
void buscar_palabras_vacias (int fichero_original, int fichero_palabras_vacias, int fichero_nuevo);
void normalizar_documento (char *nombre);
void consulta();
int busqueda_archivo(char termino[], char *documento);
void ordenar (int n, PAR *a);


// Main.
void main ()
{
	mostrar_menu ();		// Se muestra el menu con las diferentes opciones.
}


// Módulo que muestra un menú.
void mostrar_menu ()
{
	int respuesta;
	char nombre_documento[100];
	
	do
	{
		printf ("\n Menu");
		printf ("\n 1. Normalizar documento.");
		printf ("\n 2. Hacer consulta.");
		printf ("\n 3. Salir del programa.");
		printf ("\n Respuesta: ");
		scanf("%d", &respuesta);
		
		if (respuesta != 3)
		{
			switch (respuesta)
			{
			case 1:
				printf("\n Indique el nombre del documento: \n");
				printf (" ");
				scanf ("%s", nombre_documento);
				normalizar_documento (nombre_documento);
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


void normalizar_documento (char *nombre)
{
	int fichero_original, fichero_palabras_vacias, fichero_nuevo;
	//char nombre[30];
	char palabras_vacias[] = "palabras_vacias.txt";

	//printf("Indique el nombre del documento\n");
	//printf (" ");
	//scanf("%s",nombre);
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
	//FILE *normalizado = fopen (nombre, "r");
	//return (normalizado);
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


void consulta()
{
	int docs_totales;
	int termi_totales;
	
	
	printf (" Introduzca el total de documentos que se van a usar: ");
	scanf ("%d",&docs_totales);

	char documentos[docs_totales][50];
	
	printf(" Introduzca los nombres de los documentos: \n");
	for(int i=0; i<docs_totales; i++)
	{
		printf (" ");
		scanf("%s", documentos[i]);
		normalizar_documento (documentos[i]);
	}

	printf (" Introduzca el total de terminos de la consulta: ");
	scanf ("%d", &termi_totales);
	char terminos[termi_totales][50];

	printf (" Introduzca los terminos de la consulta: \n");
	for (int j = 0; j < termi_totales; j++)
	{
		//printf(" Introduce el termino %d a buscar\n", j+1);
		printf (" ");
		scanf("%s", terminos[j]);
	}

// ------------------------------------------------------------------------------------------------------ //

	printf ("\n\n\n");
	int doc[docs_totales][termi_totales], ni[termi_totales];
	double idf[termi_totales], tf_idf[docs_totales][termi_totales];
	printf("--------------- Frecuencia de los terminos ----------------\n\n");
	printf("\t");
	for(int i = 0; i < termi_totales; i++)
	{
		printf ("t%i\t", i + 1);
	}
	printf("\n");

	for(int i=0;i<docs_totales;i++){
		printf("d%d\t",i+1);
		for(int j=0;j<termi_totales;j++){
			doc[i][j] = busqueda_archivo(terminos[j], documentos[i]);
			printf(" %d\t",doc[i][j]);
		}//for
		printf("\n");
	}//
	//printf("\n");
	for(int i=0;i<termi_totales;i++){
		ni[i]=0; //inicializacion de la matriz a 0
	}
	for(int i=0;i<docs_totales;i++){ //recorremos todos los documentos
		for(int j=0;j<termi_totales;j++){ //recorremos todos los terminos
			if(doc[i][j]>0){
				ni[j]=ni[j]+1;
			}else{
				ni[j]=ni[j]+0;
			}
		}
	}
	//printf("\t");
	//for(int i=0;i<termi_totales;i++){
	//	printf("t%i\t",i+1);
	//}
	//printf("\n");
	printf("ni\t");
	for(int i=0;i<termi_totales;i++){
		printf(" %d\t",ni[i]);
	}
	//printf("\n");
	//printf("\n");
	//printf("********IDF DE LOS TERMINOS**********\n\n");
	for(int i=0;i<termi_totales;i++){
		//printf("t%i\t\t",i+1);
	}
	//printf("\n");
	
	for(int i=0;i<termi_totales;i++){
		if(ni[i]==0){
			idf[i]=0; //inicializacion de los idf
		}else{
			idf[i]=log10((double)(docs_totales)/(double)ni[i]);		// -ni[i]
		}
		
		//printf("%lf\t",idf[i]);
	}
	printf("\n");
	printf("\n");
	for(int i=0;i<termi_totales;i++){
		for(int j=0;j<docs_totales;j++){
			tf_idf[j][i]= idf[i]*doc[j][i];
		}
	}
	int q[termi_totales];
	for(int i=0;i<termi_totales;i++){
		q[i]=1;
	}
	//printf("********IDF*TF********* \n");
	for(int i=0;i<termi_totales;i++){
		//printf("\tt%i\t",i+1);
	}
	printf("\n");
	for(int i=0;i<docs_totales;i++){
		//printf("d%d\t",i+1);
		for(int j=0;j<termi_totales;j++){
			//printf("%lf\t",tf_idf[i][j]);
		}
		//printf("\n");
	}

// ------------------------------------------------------------------------------------------------------ //


	// Se hacen las suposiciones.
	printf ("\n\n");
	printf ("----------- Tabla de suposiciones ---------- \n");
	printf ("	 ni		ci = log (N - ni) / ni \n");
	for (int i = 0; i < termi_totales; i++)
	{
		printf ("t%i\t", i + 1);
		printf (" %d\t\t", ni[i]);
		printf ("%f", idf[i]);
		printf ("\n");
	}
	printf ("\n\n\n");


// ------------------------------------------------------------------------------------------------------ //

	//printf("\n");
	double abs[docs_totales];
	//printf("***********VALORES ABSOLUTOS DE CADA DOCUMENTO*************\n");
	for(int i=0;i<docs_totales;i++){
		abs[i]=0;		
		//printf("d%d\t",i+1);
		int j;
		for(j=0;j<termi_totales;j++){
			if(j%2==0 && j<=termi_totales-1){
				abs[i]=abs[i]+sqrt(pow(tf_idf[i][j],2)+pow(tf_idf[i][j+1],2));
			}
		}
		//printf("%lf\n",abs[i]);
	}

// ------------------------------------------------------------------------------------------------------ //

	double absq;
	//printf("\n");
	//printf("-----------VALOR ABSOLUTO DE LA CONSULTA------------\n");
	int i;
	for(i=0;i<termi_totales;i++){
		if(i%2==0){
			absq=sqrt(pow(q[i],2)+pow(q[i+1],2));
		}
	}
	//printf("%lf\n",absq);
	//printf("\n");

// ------------------------------------------------------------------------------------------------------ //

	double sumatorioqd[docs_totales];
	//printf("--------------[qxd]------------- \n");
	for (int i=0;i<docs_totales;i++){
		//printf("d%d\t",i+1);
		sumatorioqd[i]=0;
		for(int j=0;j<termi_totales;j++){
			sumatorioqd[i]=sumatorioqd[i]+tf_idf[i][j];	
		}
		//printf("%lf\n",sumatorioqd[i]);
	}

// ------------------------------------------------------------------------------------------------------ //

	printf("\n");
	double sim[docs_totales];
	for(i=0;i<docs_totales;i++){
		//printf("Sum=%lf   |q|=%lf   |d%d|=%lf\n",sumatorioqd[i], absq, i+1, abs[i]);
		printf(" sim(q,d%d) =",i+1);
		
		sim[i]=sumatorioqd[i]/(absq*abs[i]);
		printf(" %lf\n\n",sim[i]);
	}

// ------------------------------------------------------------------------------------------------------ //

	// Para no machacar las similitudes de sim.
	PAR sim2[docs_totales];
	for (int i = 0; i < docs_totales; i++)
	{
		sim2[i].termino = sim[i];
		sim2[i].posicion = i + 1;
	}

	// Se ordenan las similitudes.
	ordenar (docs_totales, sim2);
/*
	printf ("\n\n Solucion:  {");
	for (int i = docs_totales - 1; i > -1; i--)
	{
		printf (" %lf", sim2[i].termino);
	}
	printf (" }\n\n\n");
*/
	printf ("\n\n Solucion:  {");
	for (int i = docs_totales - 1; i > -1; i--)
	{
		printf (" d%d", sim2[i].posicion);
	}
	printf (" }\n\n\n");
}


// Modulo que ordena un array.
void ordenar (int n, PAR *a)
{
	PAR t;

	if (n >= 2)
	{
		for (int j = n + 1; j > 2; j--)
		{
			for (int i = 0; i < j - 2; i++)
			{
				if (a[i].termino > a[i + 1].termino)
				{
					t = a[i];
					a[i] = a[i + 1];
					a[i + 1] = t;
				}
			}
		}
	}
}


int busqueda_archivo (char *termino, char *documento)
{
	int j = 0;
	int tam;
	char aux;
	char buffer_aux[50];
	int doc;
	int contador = 0;

	doc = open (documento, O_RDWR);
	lseek (doc, 0, SEEK_SET);
	tam = lseek (doc, 0, SEEK_END);
	lseek (doc, 0, SEEK_SET);

	for (int i = 0; i < tam; i++)
	{
		memset (buffer_aux, '\0', 50);
		read (doc, &aux, sizeof (char));
		
		while ((aux != ' ') && (aux != '\n') && (j < tam))
		{
			buffer_aux[j] = aux;
			j++;
			
			read (doc, &aux, sizeof (char));
			i++;
		}
		
		j = 0;
		
		if (!(strcmp (termino, buffer_aux)))
		{
			//close(doc);	
			contador++;
		}
	}

	close(doc);

	return contador;
}

