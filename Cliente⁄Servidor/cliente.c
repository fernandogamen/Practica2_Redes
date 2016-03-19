#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <unistd.h>

#define PORT        	3500
#define SERVER_ADDR     "192.168.100.7"     /* localhost */
#define MAXBUF          2000 /* Máximo de bytes permitidos */

int main(){   
  int sockfd, read_size;
  struct sockaddr_in dest;
  char buffer[MAXBUF];
  int lectura;
  int i,j;

  /*Abrimos el socket*/
  if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
    perror("Error en el socket.\n");
    return 1;
  }

  //Inicializamos los datos para el servidor
  bzero(&dest, sizeof(dest));
  dest.sin_family = AF_INET;
  dest.sin_port = htons(PORT);
  if ( inet_aton(SERVER_ADDR, &dest.sin_addr.s_addr) == 0 ){
    perror("Dirección incorrecta. \n");
    return 1;
  }

  //Conectamos al servidor
  if ( connect(sockfd, (struct sockaddr*)&dest, sizeof(dest)) != 0 ){
    perror("Fallo de conexión. \n");
    return 1;
  }

  printf("Conectado. \n");

  strcpy(buffer,"Hola Jordi.");

  if(send(sockfd , buffer, strlen(buffer) ,0) < 0){
      puts("Envio del mensaje fallido");
      return 1;
  }

  printf("Envio de datos.\n");

  close(sockfd);
  return 0;
    
  if(lectura < 0){
    perror("Error de lectura por parte del servidor. \n");
    return 1;
  }
}
