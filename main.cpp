#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include<cstdlib>
#include<iomanip>
#include <iostream>
#include <ctime>
#include <vector>
#include <fstream>

#define MAX 80
#define ARCHIVO_USUARIOS "usuarios.dat"
#define TECLA_ENTER 13
#define TECLA_BACKSPACE 8
#define LONGITUD 5
#define MAX_INTENTOS 3

using std::cout; using std::ofstream;
using std::endl; using std::string;
using std::cin;

/* Estructura de cuentas de usuario */
struct usuario {
	char nombre[MAX];
	char password[MAX];
};

typedef struct usuario Usuario;

/* Funciones de men� */
void menuInicial();
void menuListarUsuarios();
void menuRegistrarUsuario();
void menuIniciarSesion();
void menuSistema();
int mostrarbitacora();

/* Funciones que manipulan el archivo de usuarios */
char insertarUsuario(Usuario usuario);
char existeUsuario(char nombreUsuario[], Usuario *usuario);
Usuario *obtenerUsuarios(int *);
char logear(char nombreUsuario[], char password[]);

int leerLinea(char *cad, int n);
void leerClave(char *password);
char hora();

char usuarioglob;

using namespace std;
class persona
{
	private:
		string clientes,vendedores,marcas,productos,lineas,id;
	public:
		void menu();
		void insert();
		void display();
		void modify();
		void search();
		void delet();
};

char linea[MAX];
int main() {
	menuInicial();
	return 0;
}

void menuInicial() {
	char repite = 1;
	int opcion = -1;

	do {
		system("cls");
		printf("\n\t\t\tMENU INICIAL\n");
		printf("\t\t\t============\n");
		printf("\n\t\t[1]. Ver clientes registrados\n");
		printf("\t\t[2]. Registrar nuevo cliente\n");
		printf("\t\t[3]. Ingresar al sistema\n");
		printf("\t\t[0]. Salir\n");
		printf("\n\t\tIngrese su opcion: [ ]\b\b");
		leerLinea(linea, MAX);
		sscanf(linea, "%d", &opcion);

		switch (opcion) {
			case 1:
				menuListarUsuarios();
				break;

			case 2:
				menuRegistrarUsuario();
				break;

			case 3:
				menuIniciarSesion();
				break;

			case 0:
				repite = 0;
				break;
		}

	} while (repite == 1);
}

void menuRegistrarUsuario() {
	Usuario usuario;
	char nombreUsuario[MAX];
	char respuesta[MAX];
	char repite = 1;

	do {
		system("cls");
		printf("\n\t\t\tREGISTRAR Clientes\n");
		printf("\t\t\t=================\n");
		printf("\n\tIngrese nombre : ");
		leerLinea(linea, MAX);
		sscanf(linea, "%s", nombreUsuario);

		/* Se verifica que el nombre de usuario no exista */
		if (!existeUsuario(nombreUsuario, &usuario)) {
			strcpy(usuario.nombre, nombreUsuario);

			printf("\tIngrese la clave: ");
			leerLinea(usuario.password, MAX);

			/* Se inserta el usuario en el archivo de usuarios */
			if (insertarUsuario(usuario)) {
				printf("\n\tEl cliente  fue registrado satisfactoriamente!\n");
				hora();
			    ofstream file_out;

			    file_out <<"se registro exitosamente :   "<<nombreUsuario;

			} else {
				printf("\n\tOcurrio un error al registrar el cliente\n");
				printf("\nInt�ntelo mas tarde\n");
			}
		} else {
			printf("\n\tEl el asiento \"%s\" ya ha sido registrado \n", usuario.nombre);
			printf("\tNo puede registrar  clientes con el mismo nombre.\n");
		}

		printf("\n\tDesea seguir registrando c? [S/N]: ");
		leerLinea(respuesta, MAX);

		if (!(strcmp(respuesta, "S") == 0 || strcmp(respuesta, "s") == 0)) {
			repite = 0;
		}

	} while (repite == 1);
}

void menuListarUsuarios() {
	int numeroUsuarios = 0;
	Usuario *usuarios;
	int i;

	system("cls");
	usuarios = obtenerUsuarios(&numeroUsuarios); /* Retorna un vector din�mico de usuarios */

	if (numeroUsuarios == 0) {
		printf("\n\tNo existe ningun usuario registrado!\n");

	} else {
		printf("\n\t\t    ==> LISTADO DE CLIENTES REGISTRADOS <==\n");
		printf(" ------------------------------------------------------------------------------\n");
		printf("%10s%25s%25s\n", "#", "NOMBRE", "PASSWORD");
		printf(" ------------------------------------------------------------------------------\n");

		/* Se recorre el vector din�mico de productos */
		for (i = 0; i < numeroUsuarios; i++) {
			printf("%10d%25s%25s\n", (i + 1), usuarios[i].nombre, usuarios[i].password);
		}
		printf(" ------------------------------------------------------------------------------\n");
	}

	// Se libera la memoria asignada al vector din�mico
	free(usuarios);
	usuarios = NULL;
	getchar();
}

void menuIniciarSesion() {
	char nombreUsuario[MAX];
	char password[MAX];
	int intento = 0;
	int loginExitoso = 0;

	do {
		system("cls");
		printf("\n\t\t\tINGRESAR AL SISTEMA\n");
		printf("\t\t\t===================\n");

		printf("\n\t\tUSUARIO: ");
		leerLinea(linea, MAX);
		sscanf(linea, "%s", nombreUsuario);

		printf("\t\tCLAVE: ");
		leerClave(password);

		if (logear(nombreUsuario, password)) {
			loginExitoso = 1;
		} else {
			printf("\n\n\t\tUsuario y/o password incorrectos");
			intento++;
			getchar();
		}
	} while (intento < MAX_INTENTOS && loginExitoso == 0);

	if (loginExitoso == 1) {
		menuSistema();

	} else {
		printf("\n\tHa sobrepasado el numero maximo de intentos permitidos\n");
		getchar();
	}
}

void menuSistema() {
	system("cls");
	printf("\n     ======================================================================\n");
	printf("\t\t\t     BIENVENIDO AL SISTEMA\n");
	printf("\t\t    Programacion 2022 | arlethe Mendoza 9959-21-1160\n");
	printf("     ======================================================================\n");

	persona estudiante;
	estudiante.menu();

	getchar();
}

/* Retorna 1 si se registr� el usuario en el archivo correctamente */
char insertarUsuario(Usuario usuario) {
	FILE *archivo;
	char insercion = 0;

	/* Abre el archivo en modo de a�adidura, para agregar datos al final. Si no existe es creado*/
	archivo = fopen(ARCHIVO_USUARIOS, "ab");

	if (archivo == NULL) {
		insercion = 0;

	} else {
		/* Registra el struct usuario en el archivo */
		fwrite(&usuario, sizeof(usuario), 1, archivo);
		insercion = 1;

		/* Cierra el archivo */
		fclose(archivo);
	}

	return insercion;
}

/* Retorna 1 si existe el nombre de usuario. Retorna el usuario buscado si existe */
char existeUsuario(char nombreUsuario[], Usuario* usuario) {
	FILE *archivo;
	char existe;

	/* Abre el archivo en modo de lectura */
	archivo = fopen(ARCHIVO_USUARIOS, "rb");

	if (archivo == NULL) {
		existe = 0;

	} else {
		existe = 0;

		/* Lee secuencialmente del archivo de usuarios */
		fread(&(*usuario), sizeof(*usuario), 1, archivo);
		while (!feof(archivo)) {
			if (strcmp((*usuario).nombre, nombreUsuario) == 0) {
				/* Encuentra un usuario del archivo con el nombre de usuario buscado */
				existe = 1;
				break;
			}

			fread(&(*usuario), sizeof(*usuario), 1, archivo);
		}

		/* Cierra el archivo*/
		fclose(archivo);
	}

	return existe;
}

Usuario *obtenerUsuarios(int *n) {
	FILE *archivo;
	Usuario usuario;
	Usuario *usuarios; /* Vector din�mico de usuarios */
	int i;

	/* Abre el archivo en modo lectura */
	archivo = fopen(ARCHIVO_USUARIOS, "rb");

	if (archivo == NULL) { /* Si no se pudo abrir el archivo, el valor de archivo es NULL */
		*n = 0; /* No se pudo abrir. Se considera n = 0 */
		usuarios = NULL;

	} else {

		fseek(archivo, 0, SEEK_END); /* Posiciona el cursor al final del archivo */
		*n = ftell(archivo) / sizeof(Usuario); /* # de usuarios almacenados en el archivo. (# de registros) */
		usuarios = (Usuario *)malloc((*n) * sizeof(Usuario)); /* Se asigna memoria para todos los usuarios almacenados en el archivo */

		/* Se recorre el archivo secuencialmente */
		fseek(archivo, 0, SEEK_SET); /* Posiciona el cursor al principio del archivo */
		fread(&usuario, sizeof(usuario), 1, archivo);
		i = 0;
		while (!feof(archivo)) {
			usuarios[i++] = usuario;
			fread(&usuario, sizeof(usuario), 1, archivo);
		}

		/* Cierra el archivo */
		fclose(archivo);
	}

	return usuarios;
}

/* Retorna 1 o 0 si el usuario y password son correctos para un usuario en particular */
char logear(char nombreUsuario[], char password[]) {
	FILE *archivo;
	char logeoExitoso;
	Usuario usuario;

	/* Abre el archivo en modo de lectura */
	archivo = fopen(ARCHIVO_USUARIOS, "rb");

	if (archivo == NULL) {
		logeoExitoso = 0;

	} else {
		logeoExitoso = 0;

		/* Lee secuencialmente del archivo de usuarios */
		fread(&usuario, sizeof(usuario), 1, archivo);
		while (!feof(archivo)) {
			if (strcmp(usuario.nombre, nombreUsuario) == 0 && strcmp(usuario.password, password) == 0) {
				/* Encuentra un usuario del archivo con el nombre de usuario y password buscado */
				logeoExitoso = 1;
					hora();
				    ofstream file_out;
				    file_out <<usuario.nombre<<" se logueo";

				break;
			}

			fread(&usuario, sizeof(usuario), 1, archivo);
		}

		/* Cierra el archivo*/
		fclose(archivo);
	}

	return logeoExitoso;
}

int leerLinea(char *cad, int n)
{
	int i, c;

	/* 1 COMPROBACI�N DE DATOS INICIALES EN EL BUFFER */
	c = getchar();
	if (c == EOF) {
		cad[0] = '\0';
		return 0;
	}

	if (c == '\n') {
		i = 0;
	} else {
		cad[0] = c;
		i = 1;
	}

	/* 2. LECTURA DE LA CADENA */
	for (; i < n - 1 && (c = getchar()) != EOF && c != '\n'; i++) {
		cad[i] = c;
	}
	cad[i] = '\0';

	/*3. LIMPIEZA DEL BUFFER */
	if (c != '\n' && c != EOF) /* es un caracter */
		while ((c = getchar()) != '\n' && c != EOF);

	return 1;
}

void leerClave(char *password) {
	char caracter;
	int i = 0;

	while (caracter = getch()) {
		if (caracter == TECLA_ENTER) {
			password[i] = '\0';
			break;

		} else if (caracter == TECLA_BACKSPACE) {
			if (i > 0) {
				i--;
				printf("\b \b");
			}

		} else {
			if (i < MAX) {
				printf("*");
				password[i] = caracter;
				i++;
			}
		}
	}
}
void persona::menu()
{
    int choice;
	char x;
	do
    {
	system("cls");

	cout<<"\t\t\t-------------------------------"<<endl;
	cout<<"\t\t\t |   SISTEMA DE MARCA |"<<endl;
	cout<<"\t\t\t-------------------------------"<<endl;
	cout<<"\t\t\t 1. Ingrese marca"<<endl;
	cout<<"\t\t\t 2. Nombre de la marca"<<endl;
	cout<<"\t\t\t 3. Codigo de marca"<<endl;
	cout<<"\t\t\t 4. Exit"<<endl;

	cout<<"\t\t\t-------------------------------"<<endl;
	cout<<"\t\t\tOpcion a escoger:[1/2/3/4/]"<<endl;
	cout<<"\t\t\t-------------------------------"<<endl;
	cout<<"Ingresa tu Opcion: ";
    cin>>choice;

    switch(choice)
    {
    case 1:
    	do
    	{
    		insert();
    		cout<<"\n\t\t\t Ingrese marca (Y,N): ";
    		cin>>x;
		}while(x=='y'||x=='Y');
		break;
	case 2:
		display();
		break;
	case 3:
		modify();
		break;
	case 4:
		search();
		break;
	case 5:
		delet();
		break;
	case 6:
		exit(0);
	default:
		cout<<"\n\t\t\t Opcion invalida...Por favor prueba otra vez..";
	}
	getch();
    }while(choice!= 5);
}
void persona::insert()
{
	system("cls");
	fstream file;
	cout<<"\n------------------------------------------------------------------------------------------------------------------------";
	cout<<"\n------------------------------------------------- detalles---------------------------------------------"<<endl;
	cout<<"\t\t\tIngresa Linea       : ";
	cin>> marcas;
	cout<<"\t\t\tIngresa Nombre de marca   : ";
	cin>>lineas;
	cout<<"\t\t\tIngresa codigo de marca  : ";
	cin>>productos;

	file.open("ParticipantRecord.txt", ios::app | ios::out);
	file<<std::left<<std::setw(15)<< clientes<<std::left<<std::setw(15)<< productos<<std::left<<std::setw(15)<< marcas <<std::left<<std::setw(15)<< lineas <<std::left<<std::setw(15)<< vendedores<< "\n";
	file.close();
	hora();

	ofstream file_out;

	file_out <<"el ultimo alogueado ingreso un nuevo con nombre cliente:  "<<clientes;


}
void persona::display()
{
	system("cls");
	fstream file;
	int total=0;
	cout<<"\n-------------------------Tabla de Detalles de Vendedores -------------------------"<<endl;
	file.open("ParticipantRecord.txt",ios::in);
	if(!file)
	{
		cout<<"\n\t\t\tNo hay informaci�n...";
		file.close();
	}
	else
	{
		file >>clientes >> marcas>> lineas >> productos >> vendedores;
		while(!file.eof())
		{
			total++;
			cout<<"\t\t\t Nombre del vendedor: "<<vendedores<<endl;
			cout<<"\t\t\t Nombre del cliente: "<<clientes<<endl;
			cout<<"\t\t\t COdigos del producto : "<<productos<<endl;
			cout<<"\t\t\t Linea del articulo  : "<<lineas<<endl;
			cout<<"\t\t\t Tipo de producto"<<marcas<<endl;
			file >>clientes >> vendedores>> marcas >> productos >>linea;
		}
		if(total==0)
		{
			cout<<"\n\t\t\tNo hay informacion...";
		}
	}
	file.close();
}
void persona::modify()
{
	system("cls");
	fstream file,file1;
	string participant_id;
	int found=0;
	cout<<"\n-------------------------Modificacion de lineas -------------------------"<<endl;
	file.open("ParticipantRecord.txt",ios::in);
	if(!file)
	{
		cout<<"\n\t\t\tNo hay informacion..,";
		file.close();
	}
	else
	{
		cout<<"\n Ingrese Id  de la marca que quiere modificar: ";
		cin>>participant_id;
		file1.open("Record.txt",ios::app | ios::out);
			file >>id>> marcas >> productos >> lineas >> vendedores>> clientes;
		while(!file.eof())
		{
			if(participant_id!=id)
			{
			 file1<<std::left<<std::setw(15)<< id <<std::left<<std::setw(15)<< vendedores <<std::left<<std::setw(15)<<std::setw(15)<< marcas <<std::left<<std::setw(15)<< lineas <<std::left<<std::setw(15)<< productos << "\n";
			}
			else

			cout<<"\t\t\tIngrese codigo de la  marca: ";
				cin>>id;
				cout<<"\t\t\tIngrese nombremarca: ";
				cin>>clientes;
				cout<<"\t\t\tIngrese telefono del vendedor : ";
				cin>>vendedores;
				cout<<"\t\t\tIngrese linea: ";
				cin>>marcas;
				cout<<"\t\t\tIngrese producto: ";
				cin>>lineas;
			file1<<std::left<<std::setw(15)<< id <<std::left<<std::setw(15)<< vendedores <<std::left<<std::setw(15)<<std::setw(15)<< marcas <<std::left<<std::setw(15)<< lineas <<std::left<<std::setw(15)<< productos << "\n";
				found++;
				hora();

	    		ofstream file_out;

    			file_out <<"el ultimo usuario logueado modifico el registro con id  "<<id;

			}
			file >> id >> clientes >> vendedores>> marcas >> lineas;

		}
		file1.close();
		file.close();
		remove("ParticipantRecord.txt");
		rename("Record.txt","ParticipantRecord.txt");
	}
void persona::search()
{
	system("cls");
	fstream file;
	int found=0;
	file.open("ParticipantRecord.txt",ios::in);
	if(!file)
	{
		cout<<"\n-------------------------Datos de la Persona buscada------------------------"<<endl;
		cout<<"\n\t\t\tNo hay informacion...";
	}
	else
	{
		string participant_id;
		cout<<"\n-------------------------Datos de linea buscada------------------------"<<endl;
		cout<<"\nIngrese Id de la Persona que quiere buscar: ";
		cin>>participant_id;
		file >> id >> vendedores>> clientes >> linea >> productos>>marcas;
		while(!file.eof())
		{
			if(participant_id==id)
			{
				cout<<"\n\n\t\t\tNombre de Persona: "<<vendedores<<endl;
				cout<<"\t\t\t Marcas elegidas por la Persona: "<<clientes<<endl;
				cout<<"\t\t\t Linea elegida : "<<linea<<endl;
				cout<<"\t\t\t productos elegidos : "<<productos <<endl;
				cout<<"\t\t\t Marcas : "<<marcas <<endl;
				found++;
			}
			file >>clientes>> vendedores >> linea >> productos>> marcas;
		}
		if(found==0)
		{
			cout<<"\n\t\t\t Persona no encontrada...";
		}
		file.close();
	}
}
void persona::delet()
{
	system("cls");
	fstream file,file1;
	string participant_id;
	int found=0;
	cout<<"\n-------------------------Detalles Persona a Borrar-------------------------"<<endl;
	file.open("ParticipantRecord.txt",ios::in);
	if(!file)
	{
		cout<<"\n\t\t\tNo hay informacion...";
		file.close();
	}
	else
	{
		cout<<"\n Ingrese el Nombre  de la Persona que quiere borrar: ";
		cin>>participant_id;
		file1.open("Record.txt",ios::app | ios::out);
		file >>clientes>> vendedores >> linea >> productos>> marcas;
		while(!file.eof())
		{
			if(participant_id!= id)
			{
file1<<std::left<<std::setw(15)<< id <<std::left<<std::setw(15)<< vendedores <<std::left<<std::setw(15)<<std::setw(15)<< marcas <<std::left<<std::setw(15)<< lineas <<std::left<<std::setw(15)<< productos << "\n";
			}
			else
			{
				found++;
				hora();

    			ofstream file_out;

    			file_out <<"el ultimo cliente logueado elimino el registro con clientes  "<<clientes;
				cout << "\n\t\t\tBorrado de informacion exitoso";


			}
			file >>clientes>> vendedores >> linea >> productos>> marcas;

		}
		if(found==0)
		{
			cout<<"\n\t\t\Nombre de Persona no encontrado...";
		}

		remove("ParticipantRecord.txt");
		rename("Record.txt","ParticipantRecord.txt");
		file1.close();
		file.close();
	}
}
char hora()
{
/* fecha/hora actual basado en el sistema actual */
time_t now = time(0);

/* Objeto de una estructura tm con fecha/hora local */
tm * time = localtime(&now);

vector<string> mes;
mes.push_back("1");
mes.push_back("2");
mes.push_back("3");
mes.push_back("4");
mes.push_back("5");
mes.push_back("6");
mes.push_back("7");
mes.push_back("8");
mes.push_back("9");
mes.push_back("10");
mes.push_back("11");
mes.push_back("12");

int year = 1900 + time->tm_year;

    ofstream file_out;

    file_out << endl<< time->tm_mday << "/" << mes[time->tm_mon] << "/" << year << "  "<< time->tm_hour << ":"
<< time->tm_min << ":" << time->tm_sec<<"  \t";

 return 0;
}
