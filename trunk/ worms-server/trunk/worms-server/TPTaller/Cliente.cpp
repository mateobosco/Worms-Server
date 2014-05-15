#include "Cliente.h"

int Cliente::cant_clientes = 0;

Cliente::Cliente(int fd){
	this->name_client = NULL;
	this->socket_cl = new Socket(PUERTO,fd);
	memset(paquete_enviar, 0, MAX_PACK);
	memset(paquete_recibir, 0, MAX_PACK);
	this->mutex = SDL_CreateMutex();
	id = Cliente::cant_clientes;
	Cliente::cant_clientes++;
//	this->id = 1;// VER COMO GENERAR EL ID
}

Cliente::Cliente(const char *name, const char *ip_sv, const char *puerto){
	this->name_client = name;
	this->socket_cl = new Socket(ip_sv, puerto);
	memset(paquete_enviar, 0, MAX_PACK);
	memset(paquete_recibir, 0, MAX_PACK);
	this->mutex = SDL_CreateMutex();
	id = Cliente::cant_clientes;
	Cliente::cant_clientes++;
}

Cliente::~Cliente(){
	//close(this->socket_cl->getFD());
	delete this->socket_cl;
	SDL_DestroyMutex(mutex);
}

Socket* Cliente::getSocket(){
	return this->socket_cl;
}

int runSendInfoCliente(void* cliente){
	Cliente* clien = (Cliente*) cliente;
	clien->runEnviarInfo();
	return EXIT_SUCCESS;
}

int runRecvInfoCliente(void* cliente){
	printf("Entro al recibir del cliente");
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
	printf("Uno\n");
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
		this->enviar(buffer, MAX_PACK); //todo
		//this->enviarInformacion(this->socket_cl,this->paqueteEnviar,sizeof(this->paqueteEnviar)); //todo
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
	while(true){
		char *buffer = new char[MAX_PACK];
		memset(buffer, 0, MAX_PACK);
		if (this->socket_cl->recibir(buffer, MAX_PACK) > 0){
			SDL_LockMutex(this->mutex);
			memcpy(this->paquete_recibir, buffer, MAX_PACK); //todo ver como determinar el tamaño del paquete
			SDL_UnlockMutex(this->mutex);
			printf("pack data recibida: %s\n", buffer);
		}
		delete[] buffer;
	}
	return EXIT_SUCCESS;
}


//En mensaje se almacena la información recibida.
//Retorna -1 si hubo error y sino la cantidad de bytes del mensaje recibido.
//int Cliente::recibir(char* mensaje, int longDataMax){
//	int retorno;
//	char buffer[MAX_PACK];
//	retorno = this->socket_cl->recibir(buffer,longDataMax);
//	return retorno;
//}

int Cliente::run(){
	return 0;
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
