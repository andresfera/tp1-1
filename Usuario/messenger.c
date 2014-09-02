//Inclucion de librerias necesarias para el programa

#include "servidor.h"
#include "cliente.h"
#include <stdio.h>
#include <string.h>
#include <errno.h> //errno
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <unistd.h>



//Struct que guardara los datos de los contactos
struct usuario{
	char nombre[50];
	char ip[15];
	char relleno2[4];
	char puerto[4];

};


struct usuario agenda;

//##################Funcion para obtener el nombre de usuario##################
int obtenerNombre(){
	printf("Indique su nombre de usuario: ");
	scanf("%s",agenda.nombre);
	
	return 0;
	}
	
//#######################Funcion para obtener el puerto###################
int obtenerPuerto(){

	FILE *archivo = fopen("acuna.txt","r");
	fscanf(archivo,"%s",agenda.puerto);
	fclose(archivo);
	return 0;
}

//####################Funcion para obtener la IP######################
int obtenerIP(){
    FILE *f;
    char line[100] , *p , *c;
    f = fopen("/proc/net/route" , "r");
     
    while(fgets(line , 100 , f)){
        p = strtok(line , " \t");
        c = strtok(NULL , " \t"); 
        if(p!=NULL && c!=NULL){
            if(strcmp(c , "00000000") == 0){
                break;}}}
     
    //cual familia se require , AF_INET o AF_INET6
    int fm = AF_INET;
    struct ifaddrs *ifaddr, *ifa;
    int family , s;
    char host[NI_MAXHOST];
    if (getifaddrs(&ifaddr) == -1){
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }
   
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next){
        if (ifa->ifa_addr == NULL){
            continue;
        }
        family = ifa->ifa_addr->sa_family;
        if(strcmp( ifa->ifa_name , p) == 0){
            if (family == fm){
                s = getnameinfo( ifa->ifa_addr, (family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6) , host , NI_MAXHOST , NULL , 0 , NI_NUMERICHOST);
                if (s != 0){
                    printf("getnameinfo() failed: %s\n", gai_strerror(s));
                    exit(EXIT_FAILURE);
                } 
                strcpy(agenda.ip,host);   //Se guarda el IP a la agenda  
		}}}
    freeifaddrs(ifaddr);
    return 0;
}
int contacto(){
	//Obtener el nombre de usuario
	obtenerNombre();
	
	//Obtener el puerto del txt
	obtenerPuerto();
	
	//Obtener la IP del sistema
	obtenerIP();
	

	
	//Impresiones de prueba
	printf("Bienvenido: %s \n",agenda.nombre);
	printf("Su puerto establecido en acuna.txt es: %s \n",agenda.puerto);
	printf("La IP asignada en su computadora: %s \n", agenda.ip);
	
	return 0;
	}
//Struct que guardara los datos de los contactos
struct agen{
	char nombre[100];
	char ip[100];
	char puerto[100];
};

struct agen agend[num];
int indice = 0; //Determina el tamaño del struct

/*Al iniciar el programa revisa el archivo contactos.txt
si este tiene información la guarda en el struct agend, que
representa la agenda de contactos del usuario*/
void contactos(){
	FILE *archivo = fopen("contactos.txt","r");
	if(archivo != NULL){
		int i = 0;
		char leido[100];
		while(!feof(archivo)){
			fscanf(archivo,"%s",leido);
			strcpy(agend[i].nombre,leido);
			fscanf(archivo,"%s",leido);
			strcpy(agend[i].ip,leido);
			fscanf(archivo,"%s",leido);
			strcpy(agend[i].puerto,leido);
			i++;
		}
		indice = i;
	}
}

/*Solicita la informacion de un contacto nuevo y la guarda 
en el struct agend*/
void agregarContacto(){

	char datos[num];

	printf("\033[01;37m\n ***** Menu: Agregar contacto ***** \n");
	printf("\na. Indique el usuario del contacto: ");
	scanf("%s",datos);
	strcpy(agend[indice].nombre,datos);
	printf("b. Indique la ip del contacto: ");
	scanf("%s",datos);	
	strcpy(agend[indice].ip,datos);
	printf("c. Indique el puerto del contacto: ");
	scanf("%s",datos);
	strcpy(agend[indice].puerto,datos);
	indice++;
	printf("\n \033[1;31m Contacto agregado exitosamente \n\033[01;37m");
	
}

//Muestra al usuario la lista de contactos
void verContacto(){
	int size = 0;
	while(size!=indice){
		printf("\nNombre %s",agend[size].nombre);
		printf("\nIP %s",agend[size].ip);
		printf("\nPuerto %s\n",agend[size].puerto);
		size++;
	}
}

/*Antes de cerrar el programa guarda los datos de
todos los contactos en el archivo contactos.txt*/
void actualizarAgenda(){
	FILE *archivo = fopen("contactos.txt","r");
	int size = 0;
	char lectura[num];
	while(!feof(archivo)){
		fscanf(archivo,"%s",lectura); // lee cada linea del archivo
		size++; // determina el tamanno del archivo
	}
	fclose(archivo);
	size = size / 3;

	//Guarda los datos de contactos que estan en el struct
	// en el archivo contactos.txt
	FILE *doc = fopen("contactos.txt","a");
	while(size!=indice){
		char datos[num];

		fprintf(doc, "%s ",agend[size].nombre);
		fprintf(doc, "%s ",agend[size].ip);
		fprintf(doc, "%s\n",agend[size].puerto);
		size++;
	}
	fclose(doc);
	exit(0);	
}


/*Funcion que permite el envio del mensaje
Solicita al usuario el contacto y mensaje a enviar*/
void enviarmjs(){
	char *datoserv[3];
	int i=0;

	//Pide al usuario el contacto al que se enviara el mensaje
	printf("\033[01;37m\nMensaje para: ");
	scanf("%s", contacto);

	while(i!=indice){
		int comp;
		comp = strcmp(agend[i].nombre,contacto);
		if(comp==0){
			datoserv[0] = agenda.nombre;
			datoserv[1] = agend[i].ip;
			datoserv[2] = agend[i].puerto;
			cliente(datoserv);// cliente envia el msj
			break;
		}
		i++;
	}
	if(i==indice){printf("Contacto no existe");}
	
}

/*Recupera la informacion del usuario, brindada en la configuración iniciar
para habilitar el puerto y ponerlo a escuchar*/
void recibirmjs(){
	char leido[num],dato[num];
	char *puerto[1];
	
	//recorre el archivo
	FILE *doc = fopen("informacion.txt", "r");
	while(!feof(doc)){
		fscanf(doc,"%s",leido);
		strcpy(dato,leido);
		fscanf(doc,"%s",leido);
		strcpy(dato,leido);
		fscanf(doc,"%s",leido);
		strcpy(dato,leido);
	}
	
	puerto[0] = dato;
	servidor(puerto);
	fclose(doc);
}

//Menu de opciones a escoger
void opciones(int opc){
	if(opc == 1) enviarmjs();
	else if(opc ==2) verContacto();
	else if(opc ==3) agregarContacto();
	else if(opc ==4){
		actualizarAgenda();} // cierra el programa
	else{
		printf("\033[1;31m Ingreso una opcion incorrecta \n\033[01;37m"); 
		printf("Ingrese la opcion deseada: ");	
		scanf("%d", &opc);
		opciones(opc);	// vuelve a llamar a la funcion opciones
	}
}

//Muestra el menu principal del programa.
void menuPrincipal(){
	int opcion;

	printf("\033[01;37m\n **** Menu principal ****\n\n");
	printf("1. Enviar mensaje\n");
	printf("2. Ver amigos\n");
	printf("3. Agregar amigos\n");
	printf("4. Salir\n");
	printf("\n Ingrese el número de la opcion deseada: ");
	scanf("%d",&opcion); // lee la opcion que el usuario ingreso y lo guarda en la variable opcion
	opciones(opcion);	
}




void *client(void *var){
	int enviar = 0;
	while(enviar ==0)
		recibirmjs();
}

void *serv(void *var){
	int recibir = 0;
	while(recibir ==0)
		menuPrincipal();
}

//Funcion principal
int main(){
	contactos();
	
	printf(" ************* BIENVENIDO(A) A GOOGLE CHAT ************* \n\n");
	contacto();
	int process = fork();
	
	
	if(process>=0){
		if(process==0){
			pthread_t servidor;
			char *msj = "servidor";
			pthread_create(&servidor,NULL,serv,(void*)msj);
			pthread_join(servidor,NULL);
		}
		else{
			pthread_t cliente;
			char *msj1 = "cliente";
			pthread_create(&cliente,NULL,client,(void*)msj1);
			pthread_join(cliente,NULL);
		}
	}
	return 0;	
}
