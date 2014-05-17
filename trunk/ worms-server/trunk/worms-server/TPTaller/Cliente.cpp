#include "Cliente.h"

// Constructor de main_server()
Cliente::Cliente(int fd){
	this->name_client = NULL;
	this->socket_cl = new Socket(PUERTO,fd);
	memset(paquete_enviar, 0, MAX_PACK);
	memset(paquete_recibir, 0, MAX_PACK);
	this->mutex = SDL_CreateMutex();
}

// Constructor de main_client()
Cliente::Cliente(const char *name, const char *ip_sv, const char *puerto){
	this->name_client = name;
	this->socket_cl = new Socket(ip_sv, puerto);
	memset(paquete_enviar, 0, MAX_PACK);
	memset(paquete_recibir, 0, MAX_PACK);
	this->mutex = SDL_CreateMutex();
}

Cliente::~Cliente(){
	delete this->socket_cl;
	SDL_DestroyMutex(mutex);
}

Socket* Cliente::getSocket(){
	return this->socket_cl;
}

int runSendInfoCliente(void* cliente){
	Cliente* client = (Cliente*) cliente;
	client->runEnviarInfo();
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
		char buffer[MAX_PACK];
		SDL_LockMutex(this->mutex);
		memcpy(buffer, this->paquete_enviar, MAX_PACK);
		int enviados = this->getSocket()->enviar(buffer, MAX_PACK); //todo
		if (enviados >= 0) printf("Voy a enviar: %s al servidor\n",buffer);
		else if(enviados == -1){
			printf("Error al enviar info cliente a servidor\n");
			break;
		}
		SDL_Delay(5000);
		SDL_UnlockMutex(this->mutex);
	}
	return EXIT_SUCCESS;
}

int Cliente::runRecibirInfo(){
	while(true){
		char buffer[MAX_PACK];
		//char *buffer = new char[MAX_PACK];
		memset(buffer, 0, MAX_PACK);
		int recibidos = this->socket_cl->recibir(buffer, MAX_PACK);
		if (recibidos > 0){
			SDL_LockMutex(this->mutex);
			memcpy(this->paquete_recibir, buffer, MAX_PACK); //todo ver como determinar el tamaño del paquete
			SDL_UnlockMutex(this->mutex);
		}
		else if(recibidos ==0){
			printf("Servidor desconectado \n");
			break;
		}
		else if (recibidos == -1){
			printf("Error\n");
		}
		SDL_Delay(5000);
		//delete[] buffer;
	}
	return EXIT_SUCCESS;
}


int Cliente::getID(){
	return EXIT_SUCCESS;
}

char* Cliente::getPaquete(){
	SDL_LockMutex(this->mutex);
	char* buffer = new char[MAX_PACK];  //ver delete todo
	memcpy(buffer, this->paquete_recibir, MAX_PACK);
	SDL_UnlockMutex(this->mutex);
	return buffer;
}

const char* Cliente::getNombre(){
	return this->name_client;
}
