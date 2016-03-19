#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //escritura
#include<pthread.h> //para hilos , conexión con lpthread
#include<unistd.h>
 
//Definimos el puerto, numero total de clientes aceptados por el servidor y máximo de bytes
#define PORT 3500
#define CLIENTES 10
#define MAXBUF 2000

int clienteNum = 0;
int clientesMenos = 10;

//La funcion de hilos
void *connection_handler(void *);
 
int main(int argc , char *argv[]){
  
    //Atributos para la conexión
    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;
    int numcliente = 0;
    
    int i,j;
     
    //Creando el socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("No se pudo crear el socket. \n");
    }
    puts("Socket creado. \n");
     
    //Prepara la estructura sockaddr_in
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //Mensaje de error
        perror("Fallo de union. Error. \n");
        return 1;
    }
    puts("Union hecha. \n");
     
    //Escuchando
    listen(socket_desc , 3);
     
    //Aceptar y espera de conexión
    puts("Esperando conexiones... \n");
    c = sizeof(struct sockaddr_in);
    
     
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c))){
      puts("Conexión aceptada. \n");
         
      pthread_t sniffer_thread;
      new_sock = malloc(1);
      *new_sock = client_sock;
         
      if( pthread_create( &sniffer_thread , NULL ,  connection_handler, (void*) new_sock) < 0){
        perror("No se pudo crear el hilo.\n");
        return 1;
      }
    }
         
      //Uniremos los hilos, no tendremos que terminar antes que el hilo
      puts("Manejador aceptado.");
    	
    	numcliente++;
    	
    	if (numcliente > CLIENTES){
    	  printf("Número de clientes alcanzado. Desconectando cliente.\n");
    	  close(client_sock);
    	}
         
      if (client_sock < 0)
        {
            perror("Accept falló.\n");
            return 1;
        }
         
        return 0;
}

/*
 * Se manejará la conexión con cada cliente
 * */
void *connection_handler(void *socket_desc){
    //Obtener la descripción del socket
    int sock = *(int*)socket_desc;
    int read_size;
    char *message , client_message[MAXBUF];
    
    recv(sock, client_message, MAXBUF, 0);

    printf("Resultado: %s\n",client_message);

    if(read_size == 0){
      puts("Cliente desconectado. \n");
  	
      clientesMenos--;
  	
      if(clientesMenos == 0){
    	  printf("No hay clientes conectados.");
    	  exit(0);
    	}
          //fflush(stdout);
    }
    else if(read_size == -1)
    {
          perror("Rescv falló. \n");
    }
           
    //Liberamos el socket.
    free(socket_desc);
       
    return 0;
}
