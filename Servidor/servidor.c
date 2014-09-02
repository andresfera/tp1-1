#include "servidor.h"
#include "cliente.h"

//VARIABLES GLOBALES
//Struct que guardara los datos de los contactos
struct agen{
	char nombre[100];
	char ip[100];
	char puerto[100];
};
//Lista de Usuarios
struct agen usuarios[num];
int ultimoUser = 0; //Determina el tamaño del struct

//Resibe el nombre del emisor y revisa que este registrado(esté dentro del arreglo de structs)
//Retorna 1: si no es usuario. Y 0: si es usuario
int verificaUsuario(char name[]){
	printf("%d",ultimoUser );
	int i=0;
	while (i!= ultimoUser){
		int comp;
		printf("%s %s", usuarios[i].nombre,name);
		comp = strcmp(usuarios[i].nombre,name);//compara el nombre con cada uno de los structs en e arreglo
		if(comp==0){
			return 0;
			}
		i++;
		}
	return 1;
	}

/*Funcion que permite el envio del mensaje
recibe el emiso, el receptor y el msj en un arreglo
llama a la fn cliente con (nombre emisor, ip receptor, puerto receptor, msj)*/
void enviar_mjs(char* arvg[]){
	printf("si entró env_msj()");
	int band= verificaUsuario(arvg[1]);
	if (band==0){
		char* paqt_envio[num];
		strcpy(paqt_envio[0],arvg[0]);//nombre del emisor
		int i;
		while (i!= ultimoUser){
			int comp;
			comp = strcmp(usuarios[i].nombre,arvg[1]);//compara el nombre del receptor con cada uno de los structs en e arreglo
			if(comp==0){
				strcpy(paqt_envio[1],usuarios[i].ip);//ip del receptor
				strcpy(paqt_envio[2],usuarios[i].puerto);//puerto del receptor
				break;
			}
		}
		strcpy(paqt_envio[3], arvg[2]);
		printf("si entró2 %s %s %s %s", paqt_envio[0],paqt_envio[1],paqt_envio[2],paqt_envio[3]);
		cliente(paqt_envio);// cliente envia el msj
	}
}

//INICIA EL CANAL DE ESCUCHA
int servidor (char* argv[]){
	printf("escuchando...");
	int socket_conex; //ID del socket de conexion
	//boolean para verificar la conexion, largo de la direccion IP
	//largo del mensaje
	int conexion, clienteAddrLen, msjLen; 
	struct sockaddr_in clienteAddr, servidorAddr;
	char msj[num]; //mensaje recivido
//-----------------------------------------------------------------------------------
//crea el socket
	socket_conex = socket(AF_INET, SOCK_STREAM, 0); //Pide el socket TCP/IP
	
	//conversion de char a dirrecion de ip (bytes de red)
	servidorAddr.sin_family = AF_INET; 
	servidorAddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servidorAddr.sin_port = htons(atoi(argv[0])); 

	//Union del socket con esta direccion
	if(bind(socket_conex, (struct sockaddr *) &servidorAddr, sizeof(servidorAddr))<0){
		perror("Error. No se puede conectar al puerto");
		exit(1);
	}
//-----------------------------------------------------------------------------------
//Ciclo infinito donde el puerto se matiene escuchando
	while(1){
		listen(socket_conex,10); //Le indica al socket que espere conexiones
		clienteAddrLen= sizeof(clienteAddr);
		//verifica si la conexion con el socket es correcta
		conexion = accept(socket_conex, (struct sockaddr *) &clienteAddr, &clienteAddrLen);
		if(conexion <0){
			perror("Error. Conexion no aceptada");
			exit(1);
		}
		
		memset(msj,0x0,num); //inicia la linea de escucha

		msjLen = recv(conexion, msj, num, 0); // Espera que lleguen todos los datos
		if(msjLen < 0){
			perror("Error en la recepción de datos");
			exit(1);
		}
		// Cuando ya tiene todos los datos recibidos, los muestra en pantalla
		printf("\n\nNuevo msj de: %s",msj);
		// ----------------
		//VERIFICA Q SEA UN USUARIO REGISTRADO
		int band= verificaUsuario(msj);
		if (band==0){//si es usuario recibe normal(para enviar a otro usuario)
			printf("todo bless hasta aqui");
			char *envio[3];//arreglo que guarda el emisor, el receptor y el mensaje
			strcpy(envio[0], msj);//se guarda el emisor
			//Recibe el nombre del destinatario final
			msjLen = recv(conexion, msj, num, 0); // Espera que lleguen todos los datos
			if(msjLen < 0){
				perror("Error en la recepcion de datos");
				exit(1);
			}
			printf("\nNombre de receptor: %s\n", msj);	
			strcpy(envio[1], msj);//se guarda el receptor
			//Recibe los mensaje para redireccionar a otro usuario
			msjLen = recv(conexion, msj, num, 0); // Espera que lleguen todos los datos
			if(msjLen < 0){
				perror("Error en la recepcion de datos");
				exit(1);
			}
			printf("\nMensaje por redirigir: %s\n", msj);
			strcpy(envio[2], msj);//se guarda el mensaje para el receptor
			
			//se redirigen los msj*******
			printf("si entró %s %s %s", envio[0], envio[1], envio[2]);
			enviar_mjs(envio);
			
		}
		else{//no es un usiario
			//lo registra como un usuario.
			//Crea el struct en que lo va a guardar
			
			strcpy(usuarios[ultimoUser].nombre, msj);//Le asigna el nombre a un nuevo struct en el arreglo de usuarios
			printf("\nAgregando usuario: %s", msj);
			//Recibe los mensaje con la info para guardar usuario
			//primero la IP
			msjLen = recv(conexion, msj, num, 0); // Espera que lleguen todos los datos de la ip
			if(msjLen < 0){
				perror("Error en la recepcion de datos");
				exit(1);
			}
			
			printf("\nIP: %s\n", msj);
			strcpy(usuarios[ultimoUser].ip, msj);//Le asigna la ip a un nuevo struct en el arreglo de usuarios
			//Segundo el PUERTO
			msjLen = recv(conexion, msj, num, 0); // Espera que lleguen todos los datos
			if(msjLen < 0){
				perror("Error en la recepcion de datos");
				exit(1);
			}
			
			printf("\nPuerto: %s\n", msj);
			strcpy(usuarios[ultimoUser].puerto, msj);//Le asigna el puerto a un nuevo struct en el arreglo de usuarios
			//agrego el struct nuevo al arreglo de usuarios
			ultimoUser++;
		}
	}
	return 0;
}


//Inicia el programa servidos
int main(){
	printf(" ************* SERVIDOR ************* \nBitacora de eventos:\n");
	//Recupera la informacion del usuario, brindada en la configuración iniciar para habilitar el puerto y ponerlo a escuchar
	char leido[num],dato[num];
	char *puerto[1];
	
	//recorre el archivo
	FILE *doc = fopen("info_server.txt", "r");
	while(!feof(doc)){
		fscanf(doc,"%s",leido);
		strcpy(dato,leido);
		fscanf(doc,"%s",leido);
		strcpy(dato,leido);
		fscanf(doc,"%s",leido);
		strcpy(dato,leido);
	}
	
	puerto[0] = dato;
	printf("%s", puerto[0]);
	servidor(puerto);
	fclose(doc);
}
