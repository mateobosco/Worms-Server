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
	this->enviar=true;

	for (int i=0; i < MAXJUG; i++){
		vector_clientes[i]=0;
	}
}


Servidor::~Servidor() {
	delete this->listener;
	SDL_DestroyMutex(mutex);
}

Socket* Servidor::getSocket(){
	return listener;
}

void Servidor::setPaqueteInicial(char paquete[MAX_PACK]){
	memcpy(this->paqueteInicial, paquete, MAX_PACK);
}

void Servidor::actualizarPaquete(char paquete[MAX_PACK]){
	this->enviar=true;
	//SDL_LockMutex(mutex);
	memcpy(this->paqueteEnviar, paquete, MAX_PACK);
	//SDL_UnlockMutex(mutex);
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
		}
		Cliente* cliente = new Cliente(sockCliente);
		if(validarCliente(cliente)==0){
			printf("Cliente existente");
		}
		//todo validar cliente
		this->runEnviarInfoInicial(cliente);
		conexion_t par;
		par.cliente = cliente;
		par.servidor = this;
		SDL_Thread* enviar = SDL_CreateThread(runSendInfo,"enviar",(void*)&par);
		if(enviar ==NULL){
			//log error todo
		}
		SDL_Thread* recibir = SDL_CreateThread(runRecvInfo,"recibir",(void*)&par);
		if(recibir == NULL){
			//log errror todo
		}
		this->clientes[this->cantClientes] = cliente;
		this->cantClientes++;
		this->vector_clientes[cantClientes-1] = 1; // TODO ponerle un nombre / id de jugador
		printf("Cantidad de clientes aceptados: %d\n",this->cantClientes);
		return EXIT_SUCCESS;
	}else{
		return EXIT_FAILURE;
	}
}

int Servidor::validarCliente(Cliente* cliente){
	int i;
	for(i =0;i<this->cantClientes ;i++){
		int compare_name = strcmp(this->clientes[i]->getNombre(),cliente->getNombre());
		if(compare_name == 0) return 0;
	}
	return 1;


}

int Servidor::runEnviarInfo(Cliente* cliente){

	while(true){
		if (this->enviar == false){
			continue;
		}
		SDL_Delay(25);
		char envio[MAX_PACK];
		//SDL_LockMutex(this->mutex);
		memcpy(envio, this->paqueteEnviar, MAX_PACK);
		//SDL_UnlockMutex(this->mutex);
		int enviados = cliente->getSocket()->enviar(envio, MAX_PACK);
		//printf("envie %d bytes al cliente \n", enviados);
		//printf(" ------- DENTRO DEL ENVIAR DEL SERVIDOR ----------- \n");
		structPaquete* paqueteCiclo = (structPaquete*) envio;

		//printf(" Voy a enviar un paquete con %d figuras \n", paqueteCiclo->cantidad_figuras);
		//printf(" Voy a enviar un paquete con %d personajes \n", paqueteCiclo->cantidad_personajes);


		structFigura* vector = paqueteCiclo->vector_figuras;
		structFigura paqueteFigura = vector[0];
		b2Vec2 posicion = paqueteFigura.vector_vertices[2];

		//printf(" Envia estas posiciones: (%f, %f) \n ", posicion.x,posicion.y);
		//printf(" ------- SALGO DEL ENVIAR DEL SERVIDOR ----------- \n");



		//structInicial* inicial = (structInicial*) envio;
		if (enviados > 0){
			this->enviar = false;
		}
		else if(enviados == -1){ // no se pudo enviar
			printf("no se envio el paquete \n");

			//this->actualizarPaquete("nahueeeeee\n");//todo
		}
	}
		return EXIT_SUCCESS;
}

int Servidor::runEnviarInfoInicial(Cliente* cliente){
	SDL_Delay(25);
	//SDL_Delay(500);
	char envio[MAX_PACK];
	//SDL_LockMutex(this->mutex);
	memcpy(envio, this->paqueteInicial, MAX_PACK);
	//SDL_UnlockMutex(this->mutex);
	int enviados = cliente->getSocket()->enviar(envio, MAX_PACK);

	printf("envie %d bytes al cliente \n", enviados);
	//printf(" ------- DENTRO DEL ENVIAR DEL SERVIDOR ----------- \n");

	structPaquete* paqueteCiclo = (structPaquete*) envio;

	//printf(" Voy a enviar un paquete con %d figuras \n", paqueteCiclo->cantidad_figuras);
	//printf(" Voy a enviar un paquete con %d personajes \n", paqueteCiclo->cantidad_personajes);


	structFigura* vector = paqueteCiclo->vector_figuras;
	structFigura paqueteFigura = vector[0];
	b2Vec2 posicion = paqueteFigura.vector_vertices[2];
	
	//printf(" Envia estas posiciones: (%f, %f) \n ", posicion.x,posicion.y);
	//printf(" ------- SALGO DEL ENVIAR DEL SERVIDOR ----------- \n");


	//structInicial* inicial = (structInicial*) envio;
	if (enviados > 0){
		this->enviar = false;
	}
	if(enviados == -1){ // no se pudo enviar
		printf("no se envio el paquete \n");
	}
	return EXIT_SUCCESS;
}

int Servidor::runRecibirInfo(void* cliente){
	while(true){
		SDL_Delay(25);
		SDL_LockMutex(this->mutex);
		Cliente* client = (Cliente*) cliente;
		char paquete[MAX_PACK];
		memset(paquete, 0, MAX_PACK);
		int cantidad = client->getSocket()->recibir(paquete, MAX_PACK);

		if(cantidad >0){

			structEvento* evento = (structEvento*) paquete;
			void* novedad = malloc (sizeof (structEvento));
			memcpy(novedad, paquete, sizeof (structEvento)); //todo ver como determinar el tamaño del paquete

			if (this->paquetesRecibir.empty()) this->paquetesRecibir.push(novedad);
			structEvento* anterior = (structEvento*) this->paquetesRecibir.front();
			if (anterior->aleatorio != evento->aleatorio){
				this->paquetesRecibir.push(novedad);
			}
			SDL_Delay(25);


		}
		else if(cantidad ==0){
			printf("Cliente desconectado\n");
			client->setConexion(false);
			int i;
			for(i=0;i<this->cantClientes; i++){
				if((this->clientes[i]->getID() == client->getID()) && (strcmp(this->clientes[i]->getNombre(),client->getNombre())==0)){
					this->clientes[i]->setConexion(false); //ver si es necesario
				}
			}
			//ver si this->cantClientes--;
			//todo desconexion cliente
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
