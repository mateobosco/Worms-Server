#include "Cliente.h"

int Cliente::cant_clientes = 0;

Cliente::Cliente(int fd){
	this->enviarpaquete = true;
	this->socket_cl = new Socket(PUERTO,fd);
	memset(paquete_enviar, 0, MAX_PACK);
	memset(paquete_recibir, 0, MAX_PACK);
	this->mutex = SDL_CreateMutex();
	this->id = Cliente::cant_clientes;
	Cliente::cant_clientes++;
	this->paqueteInicial = NULL;
	this->conectado = false;
	this->activo = false;
//	this->id = 1;// VER COMO GENERAR EL ID
}

Cliente::Cliente(const char *name, const char *ip_sv, const char *puerto){
	strncpy(this->name_client, name, MAX_NAME_USER - 1);
	this->socket_cl = new Socket(ip_sv, puerto);
	memset(paquete_enviar, 0, MAX_PACK);
	memset(paquete_recibir, 0, MAX_PACK);
	this->mutex = SDL_CreateMutex();
	this->id = Cliente::cant_clientes;
	Cliente::cant_clientes++;
	this->paqueteInicial = NULL;
	this->conectado = false;
	this->activo = false;
}

Cliente::~Cliente(){
	delete this->socket_cl;
	SDL_DestroyMutex(mutex);
}

Socket* Cliente::getSocket(){
	return this->socket_cl;
}


int Cliente::getID(){
	return this->id;
}

char* Cliente::getPaquete(){
	SDL_LockMutex(this->mutex);
	char* buffer = new char[MAX_PACK];
	memcpy(buffer, this->paquete_recibir, MAX_PACK);
	SDL_UnlockMutex(this->mutex);
	return buffer;
}

structInicial* Cliente::getPaqueteInicial(){
	return this->paqueteInicial;
}

void Cliente::setConexion(bool estado){
	this->conectado = estado;
}

int runSendInfoCliente(void* cliente){
	Cliente* clien = (Cliente*) cliente;
	clien->runEnviarInfo();
	return EXIT_SUCCESS;
}

int runRecvInfoCliente(void* cliente){
	Cliente* client = (Cliente*) cliente;
	client->runRecibirInfo();
	return EXIT_SUCCESS;
}

int Cliente::conectar(){
	if ( this->socket_cl->conectar() != EXIT_SUCCESS) {
		//loguear error todo
		return EXIT_FAILURE;
	}
	this->conectado = true;
	printf("Conecte cliente %s con servidor. num de fd es: %d\n", this->name_client,this->socket_cl->getFD());
	SDL_Thread* recibirDelServidor = SDL_CreateThread(runRecvInfoCliente, "recibirServidor",(void*)this);
	if(recibirDelServidor == NULL){
		//ver que hacer
		//loguear error todo
		return EXIT_FAILURE;
	}
	SDL_Thread* enviarAlServidor = SDL_CreateThread(runSendInfoCliente,"enviarServidor",(void*)this);
	if(enviarAlServidor == NULL){
		//ver que hacer
		//loguear error todo
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int Cliente::runEnviarInfo(){
	while(true){
		//se bloquea mutex
		if ( enviarpaquete == false){
			continue;
		}
		SDL_Delay(25);
		char buffer[MAX_PACK];
		SDL_LockMutex(this->mutex);
		memcpy(buffer, this->paquete_enviar, MAX_PACK);
		int enviados = this->enviar(buffer, MAX_PACK); //todo
		if (enviados >= 0){
			enviarpaquete = false; /*printf("Voy a enviar: %s al servidor\n",buffer)*/;
		}
		else if(enviados == -1){
			printf("Error al enviar info cliente a servidor\n");
			//break;
		}
		//Se desbloquea
		SDL_UnlockMutex(this->mutex);
	}
	return EXIT_SUCCESS;
}


//solo envia info al servidos a través del thread
int Cliente::enviar(char* mensaje, size_t longData){
	return this->socket_cl->enviar(mensaje, longData);
}


int Cliente::runRecibirInfo(){
	int contador = 0;
	while(true){
		SDL_Delay(25);
		char buffer[MAX_PACK];
		//char* buffer = (char*) malloc(sizeof(char) * MAX_PACK);
		//memset(buffer, 0, MAX_PACK);
		int recibidos = this->socket_cl->recibir(buffer, MAX_PACK);
		printf("recibi %d bytes", recibidos);
		if (recibidos > 0){
			contador++;
			//SDL_Delay(2000);
			//SDL_LockMutex(this->mutex);
			memcpy(this->paquete_recibir, buffer, MAX_PACK); //todo ver como determinar el tamaño del paquete
			if (contador > 2){
				printf(" --------- ENTRA EN RECIBIR CLIENTE ------------ \n");
				//structPaquete* paquete = new structPaquete;
				structPaquete* paquete = (structPaquete*) buffer;
				structFigura* vector = paquete->vector_figuras;
				//int cantidad = paquete->cantidad_figuras;
				//printf("CANTIDAD DE FIGURAS %d \n",cantidad);
				structFigura paqueteFigura = vector[0];
				b2Vec2 posicion = paqueteFigura.vector_vertices[2];
				printf("posicion de la figura : (%f,%f) \n",posicion.x,posicion.y);
				structPersonaje* vector2 = paquete->vector_personajes;
				//structPersonaje paquetitox = vector2[0];
				//if (paquetitox.conectado == 1){
				//	printf ( " RECIBIO CONECTADO = TRUEEE \n");
				//}
				printf(" --------- SALE DE RECIBIR CLIENTE ------------ \n");

			}
			//SDL_UnlockMutex(this->mutex);
			if (contador == 1){
				structInicial* buffer2 = (structInicial*) buffer;
				this->paqueteInicial  = buffer2;
				printf("nivel del agua en recibir info es %f \n", buffer2->nivel_agua);
				printf("path de paquetito %s  \n", buffer2->cielo);
			}
		}
		else if(recibidos ==0){
			printf("Servidor desconectado \n");
			break; //corta xq si se desconecta no tiene que recibir mas
			//VER QUE HACER SI SE DESCONECTA EL SERVIDOR todo
		}
		else if (recibidos == -1){
			printf("Error al recibir \n");
		}
	}
	return EXIT_SUCCESS;
}


void Cliente::actualizarPaquete(structEvento* evento){
	this->enviarpaquete=true;
	memcpy( this->paquete_enviar, evento, sizeof(structEvento));
}

char* Cliente::getNombre(){
	return this->name_client;
}

void Cliente::setNombre(char *name){
	SDL_LockMutex(this->mutex);
	strncpy(this->name_client, name, MAX_NAME_USER-1);
	SDL_UnlockMutex(this->mutex);
}

void Cliente::enviarNombre(){
	this->socket_cl->enviar(this->name_client, MAX_NAME_USER);
}

void Cliente::setActivo(){
	this->activo = true;
}

void Cliente::resetActivo(){
	this->activo = false;
}

bool Cliente::getActivo(){
	return this->activo;
}
