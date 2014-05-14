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
	strcpy(paqueteEnviar, "facuuuuuuuu");

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
	printf("Entra en escucharConexiones\n");
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

int Servidor::aceptarConexiones(){
	Socket* sockCliente = this->listener->aceptar();
	//Se crea un cliente y un thread asociado a el y se invoca el método run.
	if(sockCliente != NULL){
		printf("entre en sockCliente!=NULL");
		Cliente* cliente =new Cliente(sockCliente->getFD());
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
		this->sockClientes[this->cantClientes] = sockCliente;
		this->cantClientes++;
		this->vector_clientes[cantClientes-1] = 1; // TODO ponerle un nombre / id de jugador
		printf("Cantidad de clientes aceptados: %d\n",this->cantClientes);
		delete sockCliente;
		return EXIT_SUCCESS;
	}else{
		return EXIT_FAILURE;
	}
}

int Servidor::runEnviarInfo(Cliente* cliente){

	while(true){
		this->enviarInformacion(cliente->getSocket(),this->paqueteEnviar, sizeof(this->paqueteEnviar)); //todo
		SDL_Delay(5000);
		this->actualizarPaquete("nahueeeeee");
	}
	return EXIT_SUCCESS;
}

int Servidor::enviarInformacion(Socket* sock, void* data, size_t longData){
	return sock->enviar(data,longData);
}

int Servidor::runRecibirInfo(void* cliente){
	while(true){
		//mutex block
		SDL_LockMutex(this->mutex);
		Cliente* client = (Cliente*) cliente;
		char paquete[MAX_PACK];
		memset(paquete, NULL, MAX_PACK);
		this->recibirInformacion(client, paquete, MAX_PACK); //todo ver tamanio
		void* novedad = malloc (sizeof (structEventos));
		memcpy(novedad, paquete, sizeof (structEventos)); //todo ver como determinar el tamaño del paquete
		this->paquetesRecibir.push(novedad);
		SDL_UnlockMutex(this->mutex);
		//mutex desbloquear
	}
	return EXIT_SUCCESS;
}

void Servidor::recibirInformacion(Cliente* cliente, char* data, size_t tamanio){
	cliente->getSocket()->recibir(data,tamanio);

}

int aceptarConex(void* servidor){
	Servidor* serv = (Servidor*) servidor;
	while(serv->getCantidadClientes() < serv->getCantidadMaxConexiones()){
		printf("Thread de aceptar conexiones\n");
		serv->aceptarConexiones();
	}
	return 0;
}

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
	int ciclo = 0;
	int i = 0;
	while(true){
		while((this->cantClientes < this->cantidadMaxConexiones)){

			printf("ciclo numero: %d \n",i);
			SDL_Thread* aceptar = SDL_CreateThread(aceptarConex,"aceptar",(void*)this);
			conexiones = this->escucharConexiones();
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
