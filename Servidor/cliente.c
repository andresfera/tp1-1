#include "cliente.h"

int cliente(char*argv[]){
	char *puerto, *ip;//variables para la dirrecion IP y # de puerto
	puerto = argv[2];
	ip = argv[1];
	
	printf("si entró3 %s %s %s %s", argv[0],argv[1], argv[2], argv[3]);
	int socket_conex; // ID del socket de conexion
	int conexion; //boolean para verificar la conexion
	struct sockaddr_in receptorAddr;  // Estructura para la direccion del mensaje

//-----------------------------------------------------------------------------------
//Crea el socket
	socket_conex = socket(AF_INET, SOCK_STREAM, 0); //Pide el socket tcp/ip
	receptorAddr.sin_family = AF_INET; // Construccion de direccion del receptor
	
	//conversion de char a dirrecion de ip (bytes de red)
	inet_pton(AF_INET,ip,&(receptorAddr.sin_addr.s_addr));
	receptorAddr.sin_port= htons(atoi(puerto));

//-----------------------------------------------------------------------------------
//Conexion con el socket 
	conexion = connect(socket_conex, (struct sockaddr *) &receptorAddr, sizeof(receptorAddr));
	if(conexion <0){ 
		perror("Error de conexion" );
		exit(1);
	}

	conexion = send(socket_conex,argv[0],strlen(argv[0])+1,0); 
	//Verifica la conexion con el socket, para el envio de mensajes
	if(conexion <0){  
		perror("Falló la conexion. Lo sentimos msj no enviado"); 
		close(socket_conex);
		exit(1);
	}

	char msj[num]; //mensaje del usuario (emisor)
	strcpy(msj,argv[3]);
	/*
	printf("\033[01;33mMsj por enviar: ");//Solicita al usuario el mensaje que desea enviar
	scanf(" %[^\n]", msj);	//Lee el mensaje ingresado y lo guarda en la variable mensaje*/
	
	//Proceso para enviar el mensaje
	conexion = send(socket_conex, msj, strlen(msj)+1, 0);
	if(conexion <0){ 
		perror("\033[01;37mFalló la conexion. Lo sentimos msj no enviado"); 
		close(socket_conex);
		exit(1);
	}
	else{
		printf("\033[01;37m\nEnviado\n");
	}
	exit(1);
} 
