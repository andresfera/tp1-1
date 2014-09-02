#include "servidor.h"

int servidor (char *argv[]){
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
		conexion= accept(socket_conex, (struct sockaddr *) &clienteAddr, &clienteAddrLen);
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
		printf("\033[01;32m\n\nNuevo msj de: %s",msj);
		// ----------------

		//Recibe los mensajes enviados
		msjLen = recv(conexion, msj, num, 0); // Espera que lleguen todos los datos
		if(msjLen < 0){
			perror("Error en la recepcion de datos");
			exit(1);
		}
		printf("\n%s\n\033[01;37m", msj);
		
	}
	return 0;
}
