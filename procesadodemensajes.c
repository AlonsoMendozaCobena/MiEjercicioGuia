#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	char peticion[100];
	strcpy(peticion, "3/Miguel/47/1.78");
	
	int codigo;
	char nombre[20];
	int edad;
	float altura;
	
	char *p = strtok(peticion, "/");
	codigo = atoi(p);
	p = strtok(NULL,"/");
	strcpy(nombre,p);
	p = strtok(NULL,"/");
	edad = atoi(p);
	p = strtok(NULL,"/");
	altura = atof(p);
	
	printf("Codigo %d, Nombre %s, Edad: %d, altura: %f\n",codigo,nombre,edad,altura);
		
	char respuesta[100];
	
	if(edad>18)
		sprintf(respuesta, "%s es mayor de edad porque tiene %d años", nombre,edad);
	else
		sprintf(respuesta, "%s no es mayor de edad",nombre);
	
	printf("Respuesta: %s\n",respuesta);
	
}

