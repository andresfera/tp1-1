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
#define LENGTH 512 


void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    /* Variable Definition */
    int sockfd; 
    struct sockaddr_in remote_addr;
    

    /* Get the Socket file descriptor */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "ERROR: Fallo al obtener la descripcion del socket! (errno = %d)\n",errno);
        exit(1);
    }

    /* Fill the socket address struct */
    remote_addr.sin_family = AF_INET; 
    remote_addr.sin_port = htons(PORT); 
    inet_pton(AF_INET, "192.168.217.130", &remote_addr.sin_addr); 
    bzero(&(remote_addr.sin_zero), 8);

    /* Try to connect the remote */
    if (connect(sockfd, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) == -1)
    {
        fprintf(stderr, "ERROR: Fallo conectando al host! (errno = %d)\n",errno);
        exit(1);
    }
    else 
        printf("[Client] Conectado al servidor en el puerto %d...ok!\n", PORT);

    /* Send File to Server */
    //if(!fork())
    //{
		
		char name[100];
        char* fs_name;
        printf("Digite la ruta donde se encuentra el archivo que desea enviar\n");
        scanf("%s",name);/*fs="/home/andres/Desktop/enviado.txt";;*/
        fs_name=name;
        char sdbuf[LENGTH]; 
        printf("[Client] Enviando al servidor");
        FILE *fs = fopen(fs_name, "r");
        if(fs == NULL)
        {
            printf("ERROR: Archivo %s no encontrado\n", fs_name);
            exit(1);
        }

        bzero(sdbuf, LENGTH); 
        int fs_block_sz; 
        while((fs_block_sz = fread(sdbuf, sizeof(char), LENGTH, fs)) > 0)
        {
            if(send(sockfd, sdbuf, fs_block_sz, 0) < 0)
            {
                fprintf(stderr, "ERROR: Fallo al enviar %s. (errno = %d)\n", fs_name, errno);
                break;
            }
            bzero(sdbuf, LENGTH);
        }
        close (sockfd);
        printf("Su archivo ha sido enviado!\n");

    
    printf("[Client] Conexion cerrada.\n");
    return (0);
}
