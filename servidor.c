#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>



int main(int argc, char *argv[]) {
	int sock_conn, sock_listen, ret;
	struct sockaddr_in serv_adr;
	char peticion[512];
	char respuesta[512];
	
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
	
	for(i=0;i<5;i++){
		printf("Escuchando \n");
		
		sock_conn = accept(sock_listen, NULL, NULL);
		printf("He recibido conexión\n");
		
		int terminar = 0;
		
		while(terminar ==0)
		{
			
			ret = read(sock_conn, peticion, sizeof(peticion));
			printf("Recibido\n");
			
			peticion[ret]='\0';
			
			printf("Peticion: %s\n",peticion);
			
			
			char *p = strtok(peticion, "/");
			int codigo = atoi(p);
			char nombre[20];
			
			if(codigo!=0)
			{
				p = strtok(NULL, "/");
				
				strcpy(nombre,p);
				printf("Codigo: %d, Nombre: %s\n", codigo, nombre);
			}
			if(codigo ==0)
				terminar =1;
			
			else if(codigo == 1)
				sprintf(respuesta,"%d",strlen(nombre));
			else if (codigo ==2)		
				if((nombre[0]=='M') || (nombre[0]=='S'))
				strcpy(respuesta, "SI");
				else
					strcpy(respuesta, "NO");
				else
				{
					p = strtok(NULL,"/");
					float altura = atof(p);
					if (altura > 1.70)
						sprintf(respuesta, "%s: eres alto", nombre);
					else
						sprintf(respuesta, "%s: eres bajo", nombre);
				}
			
			if(codigo!=0)
			{
				printf("Respuesta: %s\n", respuesta);
				
				write(sock_conn,respuesta,strlen(respuesta));
			}
		}
		close(sock_conn);
	
	}
	
	return 0;
}

