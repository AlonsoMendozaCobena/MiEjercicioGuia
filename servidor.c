#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>

int contador; 
pthread_mutex_t mutex= PTHREAD_MUTEX_INITIALIZER;

void *AtenderCliente (void *socket)
{
	char peticion[512];
	char respuesta[512];
	int socket_conn;
	int ret;
	int *s;
	s= (int *) socket;
	socket_conn= *s;
	
	//int socket_conn = * (int *) socket;
	
	int terminar = 0;
	
	while(terminar==0)
	{
		// Ahora recibimos la peticion
		ret = read(socket_conn,peticion, sizeof(peticion));
		printf("Recibido\n");
		
		//Tenemos que añadirle la marca de fin de string
		peticion[ret]="\0";
		
		printf("Peticion: %s\n",peticion);
		
		//Vemos que quieren
		char *p = strtok(peticion,"/");
		int codigo = atoi(p);
		//Ya tenemos el codigo de la peticion
		char nombre[20];
		
		if(codigo !=0)
		{
			p = strtok(NULL,"/");
			
			strcpy(nombre,p);
			printf("Codigo: %d, Nombre: %s\n",codigo,nombre);
		}
		
		if(codigo ==0)
		   terminar = 1;
		else if(codigo==4)
			sprintf(respuesta, "%d",contador);
		else if (codigo ==1)
			sprintf(respuesta, "%d",strlen(nombre));
		else if (codigo == 2)
			if((nombre[0]=='M')||(nombre[0]=='S'))
			strcpy(respuesta,"SI");
			else
				strcpy(respuesta,"NO");
		else
		{
			p =strtok( NULL, "/");
			float altura = atof(p);
			if (altura > 1.70)
				sprintf(respuesta, "%s: eres alto", nombre);
			else
				sprintf(respuesta, "%s: eres bajo", nombre);
			
		}
		
		if(codigo !=0)
		{
			printf("Respuesta: %s\n", respuesta);
			
			write(socket_conn,respuesta, strlen(respuesta));
			
		}
		if((codigo==1)||(codigo==2)||(codigo==3))
		   pthread_mutex_lock( &mutex );
		   contador = contador+1;
		   pthread_mutex_unlock( &mutex);
	}
	
	close(socket_conn);
}

int main(int argc, char *argv[]) {
	contador = 0;
	int sock_conn, sock_listen;
	struct sockaddr_in serv_adr;
	
	
	//Abrimos el socket
	if((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("Error creando el socket");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	
	
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(9050);
	
	if(bind(sock_listen, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) < 0)
		printf("Error en el bind");
	
	if(listen(sock_listen, 3)<0)
		printf("Error en el listen");
	
	int i;
	int sockets[100];
	pthread_t thread;
	i=0;
	for(;;){
		printf("Escuchando \n");
		
		sock_conn = accept(sock_listen, NULL, NULL);
		printf("He recibido conexión\n");
		
		sockets[i] = sock_conn;
		// sock_conn es el socket que usaremos para este cliente.
		// Creamos un hilo y le decimos que tenemos que hacer
		
		pthread_create (&thread, NULL, AtenderCliente,&sockets[i]);
		i=i+1;
		
	}
	
	//for(i=0;i<5;i++)
	//{
	//	pthread_join(thread[i],NULL);
		
	//}
}
