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


int Servidor::validarSocket(int sock){
	int fdSock = sock;
	int i;
	int salida = 0;
	for(i=0; i< this->cantClientes; i++){
		int fd = this->clientes[i]->getSocket()->getFD();
		if(fd != fdSock) salida = 0;
		else return 1;
	}

	return salida;
}

int Servidor::aceptarConexiones(){
	int sockCliente = this->listener->aceptar();
	//Se crea un cliente y un thread asociado a el y se invoca el método run.
	if(sockCliente > 0){
		while(validarSocket(sockCliente) == 1){
			close(sockCliente);
			sockCliente = this->listener->aceptar();
			printf("ASigno mismo fd : %d\n",sockCliente);
		}
		Cliente* cliente = new Cliente(sockCliente);
		printf("fd del socket creado desde accept: %d\n",sockCliente);
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
		if(enviados == 0){
			printf("Actualizar paquete \n");
			this->actualizarPaquete("nahueeeeee\n");//todo
		}
		else if(enviados == -1){
			printf("Error del servidor al enviar al cliente\n");
			break;
		}
		SDL_Delay(5000);
	}
	return EXIT_SUCCESS;
}


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
		}
		else if(cantidad ==0){
			printf("Cliente desconectado\n");
			break;
		}
		else if(cantidad ==-1){
			printf("Error al recibir información del cliente\n");
			break;
		}
		SDL_Delay(5000);
		SDL_UnlockMutex(this->mutex);
	}
	return EXIT_SUCCESS;
}


int Servidor::runEscucharConexiones(){
	int conexiones;
	try{
		conexiones = this->getSocket()->EnlazarYEscuchar(this->cantidadMaxConexiones);

	}catch(exception &e){
		close(this->listener->getFD());
		return EXIT_FAILURE;
		//loguear error
	}
	while(true){
		while((this->cantClientes < this->cantidadMaxConexiones)){
			conexiones = this->escucharConexiones();
			if (conexiones == -1){
				printf("Error al escuchar conexiones \n");
				return EXIT_FAILURE;
			}

		}
		if(this->cantClientes > this->cantidadMaxConexiones){
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
