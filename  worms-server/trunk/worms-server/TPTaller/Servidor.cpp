#include "Servidor.h"

typedef struct conexion_t{
	Cliente* cliente;
	Servidor* servidor;
}conexion_t;

Servidor::Servidor(int maxCon){
	this->puerto = PUERTO;
	this->cantidadMaxConexiones = maxCon;
	this->cantClientes = 0;
	this->maxFD = 0;
	this->listener = new Socket(NULL, PUERTO);
	this->mutex = SDL_CreateMutex();
	memset(this->paqueteEnviar, 0, MAX_PACK);
	strcpy(paqueteEnviar, "facuuuuuuuu\n");

	for (int i=0; i < MAXJUG; i++){
		vector_clientes[i]=0;
	}
}


Servidor::~Servidor() {
	delete this->listener;
	SDL_DestroyMutex(mutex);
}

Socket* Servidor::getSocket(){
	return this->listener;
}

void Servidor::actualizarPaquete(char paquete[MAX_PACK]){
	SDL_LockMutex(mutex);
	memcpy(this->paqueteEnviar, paquete, MAX_PACK);
	SDL_UnlockMutex(mutex);
}

void* Servidor::desencolarPaquete(){
	if(!this->paquetesRecibir.empty()){
		void* paquete = this->paquetesRecibir.front();
		this->paquetesRecibir.pop();
		return paquete;
	}
	else{
		return NULL;
	}
}

int Servidor::escucharConexiones(){
	//printf("Entra en escucharConexiones\n");
	int es = this->listener->escuchar(this->cantidadMaxConexiones);
	return es;
}

int runSendInfo(void* par){
	conexion_t* cliente_servidor = (conexion_t*) par;
	Cliente* cliente = cliente_servidor->cliente;
	Servidor* servidor = cliente_servidor->servidor;
	return servidor->runEnviarInfo(cliente);
}

int runRecvInfo(void* par){
	conexion_t* cliente_servidor = (conexion_t*) par;
	Cliente* cliente = cliente_servidor->cliente;
	Servidor* servidor = cliente_servidor->servidor;
	servidor->runRecibirInfo(cliente);
	return 0;
}

int Servidor::aceptarConexiones2(){



	return 0;
}

int Servidor::validarSocket(Socket* sock){
	int fdSock = sock->getFD();
	int i;
	for(i=0; i< this->cantClientes; i++){
		int fd = this->clientes[i]->getSocket()->getFD();
		if(fd != fdSock) return 0;
		else return 1;
	}

}

int Servidor::aceptarConexiones(){
	Socket* sockCliente = this->listener->aceptar();

	//Se crea un cliente y un thread asociado a el y se invoca el método run.
	if(sockCliente != NULL){
		while(validarSocket(sockCliente) == 1){
			printf("ASigno mismo fd : %d\n",sockCliente->getFD());
			delete sockCliente;
			sockCliente = this->listener->aceptar();
		}
		Cliente* cliente = new Cliente(sockCliente->getFD());
		printf("fd del socket creado desde accept: %d\n",cliente->getSocket()->getFD());
		conexion_t par;
		par.cliente = cliente;
		par.servidor = this;
		SDL_Thread* enviar = SDL_CreateThread(runSendInfo,"enviar",(void*)&par);
		if(enviar ==NULL){
			//log error todo
		}
//		int thread_1 = 0;
//		SDL_WaitThread(enviar, &thread_1);
		SDL_Thread* recibir = SDL_CreateThread(runRecvInfo,"recibir",(void*)&par);
		if(recibir == NULL){
			//log errror todo
		}
//		int thread_2 = 0;
//		SDL_WaitThread(recibir, &thread_2);
		this->clientes[this->cantClientes] = cliente;
		this->cantClientes++;
//		this->vector_clientes[cantClientes-1] = 1; // TODO ponerle un nombre / id de jugador
		printf("Cantidad de clientes aceptados: %d\n",this->cantClientes);
		delete sockCliente;
		return EXIT_SUCCESS;
	}else{
		return EXIT_FAILURE;
	}
}

int Servidor::runEnviarInfo(Cliente* cliente){

	while(true){
		char envio[MAX_PACK];
		SDL_LockMutex(this->mutex);
		memcpy(envio, this->paqueteEnviar, MAX_PACK);
		SDL_UnlockMutex(this->mutex);
		int enviados = cliente->getSocket()->enviar(envio, MAX_PACK);
		if(enviados > 0){
			//SDL_Delay(5000);
			this->actualizarPaquete("nahueeeeee\n");//todo
		}
		else if(enviados == -1){
			printf("Error del servidor al enviar al cliente\n");
			break;
		}
	}
	return EXIT_SUCCESS;
}

//int Servidor::enviarInformacion(Socket* sock, char* data, size_t longData){
//	return sock->enviar(data,longData);
//}

int Servidor::runRecibirInfo(void* cliente){
	while(true){
		SDL_LockMutex(this->mutex);
		Cliente* client = (Cliente*) cliente;
		char paquete[MAX_PACK];
		memset(paquete, 0, MAX_PACK);
		int cantidad = client->getSocket()->recibir(paquete, MAX_PACK); //todo ver tamanio

		if(cantidad >0){
			void* novedad = malloc (sizeof (structEventos));
			memcpy(novedad, paquete, sizeof (structEventos)); //todo ver como determinar el tamaño del paquete
			this->paquetesRecibir.push(novedad);
			printf("Recibí: %s del cliente",paquete);
		}
		else if(cantidad ==0){
			printf("Cliente desconectado\n");
			break;
		}
		else if(cantidad ==-1){
			printf("Error al recibir información del cliente\n");
			break;
		}
		SDL_UnlockMutex(this->mutex);
	}
	return EXIT_SUCCESS;
}

//void Servidor::recibirInformacion(Cliente* cliente, char* data, size_t tamanio){
//	cliente->getSocket()->recibir(data,tamanio);
//
//}

//int aceptarConex(void* servidor){
//	Servidor* serv = (Servidor*) servidor;
//	while(serv->getCantidadClientes() < serv->getCantidadMaxConexiones()){
//		printf("Thread de aceptar conexiones\n");
//		serv->aceptarConexiones();
//	}
//	return 0;
//}

int Servidor::runEscucharConexiones(){
	int conexiones;
	try{
		printf("Escuchar conexiones primera vez\n");
		conexiones = this->getSocket()->EnlazarYEscuchar(this->cantidadMaxConexiones);

	}catch(exception &e){
		printf("No pudo escuchar primera vez\n");
		close(this->listener->getFD());
		return EXIT_FAILURE;
		//loguear error
	}
	int i = 0;
	while(true){
		while((this->cantClientes < this->cantidadMaxConexiones)){

			//printf("ciclo numero: %d \n",i);
			//SDL_Thread* aceptar = SDL_CreateThread(aceptarConex,"aceptar",(void*)this);
			conexiones = this->escucharConexiones();
			if (conexiones == -1){
				printf("Error al escuchar conexiones \n");
				break;
			}
			i++;

		}
		if(this->cantClientes == this->cantidadMaxConexiones){
			printf("break\n");
			break;
		}
	}
	return EXIT_SUCCESS;

}


int Servidor::getCantidadMaxConexiones(){
	return this->cantidadMaxConexiones;
}

int Servidor::getCantidadClientes(){
	return this->cantClientes;
}


int* Servidor::getVectorClientes(){
	return vector_clientes;
}
