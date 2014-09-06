#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <signal.h>
#include <ctype.h>          
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 20000 
#define BACKLOG 5
#define LENGTH 512 

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main ()
{
    /* Defining Variables */
    int sockfd; 
    int nsockfd; 
    int sin_size; 
    struct sockaddr_in addr_local; /* client addr */
    struct sockaddr_in addr_remote; /* server addr */
    char revbuf[LENGTH]; // Receiver buffer

    /* Get the Socket file descriptor */
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    {
        fprintf(stderr, "ERROR: Fallo al obtener la descripcion del socket. (errno = %d)\n", errno);
        exit(1);
    }
    else 
        printf("[Server] Obtuvo exitosamente la descripcion del socket.\n");

    /* Fill the client socket address struct */
    addr_local.sin_family = AF_INET; // Protocol Family
    addr_local.sin_port = htons(PORT); // Port number
    addr_local.sin_addr.s_addr = INADDR_ANY; // AutoFill local address
    bzero(&(addr_local.sin_zero), 8); // Flush the rest of struct

    /* Bind a special Port */
    if( bind(sockfd, (struct sockaddr*)&addr_local, sizeof(struct sockaddr)) == -1 )
    {
        fprintf(stderr, "ERROR: Fallo al vincular con el puerto. (errno = %d)\n", errno);
        exit(1);
    }
    else 
        printf("[Server] Vinculado al puerto %d exitosamente.\n",PORT);

    /* Listen remote connect/calling */
    if(listen(sockfd,BACKLOG) == -1)
    {
        fprintf(stderr, "ERROR: Fallo al ewcuchar el puerto. (errno = %d)\n", errno);
        exit(1);
    }
    else
        printf ("[Server] Escuchando el puerto %d exitosamente.\n", PORT);

    int success = 0;
    while(success == 0)
    {
        sin_size = sizeof(struct sockaddr_in);

        /* Wait a connection, and obtain a new socket file despriptor for single connection */
        if ((nsockfd = accept(sockfd, (struct sockaddr *)&addr_remote, &sin_size)) == -1) 
        {
            fprintf(stderr, "ERROR: Obteniendo el descriptor del socket. (errno = %d)\n", errno);
            exit(1);
        }
        else 
            printf("[Server] El servidor se conecto desde %s.\n", inet_ntoa(addr_remote.sin_addr));

        /*Receive File from Client */
        char* fr_name = "/home/andres/Desktop/recibido/recibido.txt";
        FILE *fr = fopen(fr_name, "a");
        if(fr == NULL)
            printf("Archivo %s no se puede abrir en el servidor.\n", fr_name);
        else
        {
            bzero(revbuf, LENGTH); 
            int fr_block_sz = 0;
            while((fr_block_sz = recv(nsockfd, revbuf, LENGTH, 0)) > 0) 
            {
                int write_sz = fwrite(revbuf, sizeof(char), fr_block_sz, fr);
                if(write_sz < fr_block_sz)
                {
                    error("File write failed on server.\n");
                }
                bzero(revbuf, LENGTH);
                if (fr_block_sz == 0 || fr_block_sz != 512) 
                {
                    break;
                }
            }
            if(fr_block_sz < 0)
            {
                if (errno == EAGAIN)
                {
                    printf("recv() timed out.\n");
                }
                else
                {
                    fprintf(stderr, "recv() failed due to errno = %d\n", errno);
                    exit(1);
                }
            }
            printf("Recibido desde el cliente!\n");
            fclose(fr); 
       
        }

        /* Call the Script 
        system("cd ; chmod +x script.sh ; ./script.sh");*/

           success = 1;
            close(nsockfd);
            printf("[Server] Conexion con el cliente cerrada.\n");
            while(waitpid(-1, NULL, WNOHANG) > 0);
        
    }
}
