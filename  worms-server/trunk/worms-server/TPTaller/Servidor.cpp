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
	this->finalizar = false;
	this->clientesActivos = 0;
	for (int i=0; i < MAX_CANT_JUGADORES; i++){
		vector_clientes[i]=0;
		clientes[i] = NULL;
	}
}


Servidor::~Servidor() {
	this->finalizar = true;
	SDL_WaitThread(this->escuchar, 0);
	SDL_WaitThread(this->aceptar, 0);
	delete this->listener;
	SDL_DestroyMutex(mutex);
}

Socket* Servidor::getSocket(){
	return listener;
}

void Servidor::setPaqueteInicial(char paquete[MAX_PACK]){
	memcpy(this->paqueteInicial, paquete, sizeof(structInicial));
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
		if(this->clientes[i]){
			int fd = this->clientes[i]->getSocket()->getFD();
			if(fd != fdSock) salida = 0;
			else return 1;
		}
//		int fd = this->clientes[i]->getSocket()->getFD();
//		if(fd != fdSock) salida = 0;
//		else return 1;
	}
	return salida;
}

int Servidor::aceptarConexiones(){
	int sockCliente = this->listener->aceptar();
	//Se crea un cliente y un thread asociado a el y se invoca el método run.
	if(sockCliente > 0){
//		while(validarSocket(sockCliente) == 1){
//			close(sockCliente);
//			sockCliente = this->listener->aceptar();
//		}
		// Ver que pasa con la ID del cliente,
		// si ya está creado el usuario con ese nombre,
		// xq aumentaría en uno por algo que ya existe.
		// [Nahue: yo lo dejaría de usar ese atributo de clase]
		Cliente* cliente = new Cliente(sockCliente);

//		if(validarCliente(cliente)==0){
//			printf("Cliente existente");
//		}

		bool recibio_nombre = false;
		//SDL_LockMutex(this->mutex);
		while (!recibio_nombre){
			int bytes = this->recibirNombre(cliente);
			if(bytes > 0 ) recibio_nombre = true;
			if(bytes == 0) break;// TODO Verificar qué pasa si la # de Bytes es -1 o 0;
		}
		if(recibio_nombre){
			int posicion = this->checkNuevoCliente(cliente);
			if(posicion != -1){
				Cliente *cliente_viejo = this->clientes[posicion];
				if (cliente_viejo != NULL){
					cliente->setJugador(cliente_viejo->getJugador());
					cliente->getJugador()->conectar();
					cliente->setID(cliente_viejo->getID());
					delete cliente_viejo;
				} else{
					this->cantClientes++;
				}
				this->setAceptado(true);
				cliente->activar(); //TODO hay que arreglar esto
				if (this->runEnviarInfoInicial(cliente) <= 0 ) /*log Error todo */;
				conexion_t par;
				par.cliente = cliente;
				par.servidor = this;

				comThreads hilosCliente;
				hilosCliente.enviar = SDL_CreateThread(runSendInfo,"enviar",(void*)&par);
				if(hilosCliente.enviar ==NULL){
					//log error todo
				}
				printf("Se crea el hilo de recibir\n");
				hilosCliente.recibir = SDL_CreateThread(runRecvInfo,"recibir",(void*)&par);
				if(hilosCliente.recibir == NULL){
					//log error todo
				}
				cliente->setHilos(hilosCliente);

				this->clientes[posicion] = cliente;
				this->vector_clientes[posicion] = cliente->getID(); // TODO ponerle un nombre / id de jugador
				printf("Cantidad de clientes aceptados: %d\n",this->cantClientes);
				return EXIT_SUCCESS;
			}else {
				this->setAceptado(false);
				printf("Cliente Rechazado\n");
				this->runEnviarInfoInicial(cliente);
				delete cliente;
				return EXIT_FAILURE;
			}
			//SDL_UnlockMutex(this->mutex);
		}else {
			delete cliente;
			return EXIT_FAILURE;
		}
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
	while(!cliente->getNombre());
	while(cliente->getActivo()){
		if (this->enviar == false){
			continue;
		}
		SDL_Delay(25);
		char envio[MAX_PACK];
		char envio2[MAX_PACK];


		//SDL_LockMutex(this->mutex);
		memcpy(envio, this->paqueteEnviar, MAX_PACK);
		//SDL_UnlockMutex(this->mutex);
		structPaquete* paqueteCiclo = (structPaquete*) envio;
		paqueteCiclo->id=cliente->getID();
		memcpy(envio2, paqueteCiclo, MAX_PACK);
		int enviados = cliente->getSocket()->enviar(envio2, MAX_PACK);
		//printf("envie %d bytes al cliente \n", enviados);
		//printf(" ------- DENTRO DEL ENVIAR DEL SERVIDOR ----------- \n");

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
			cliente->desactivar();
			//this->actualizarPaquete("nahueeeeee\n");//todo
		}
		if(enviados == 0){
				printf("Cliente desconectado\n");
				cliente->desactivar();
					//this->clientesActivos--;
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
	if(enviados == 0){
		printf("Cliente desconectado\n");
		cliente->desactivar();
	}
	if(enviados == -1){ // no se pudo enviar
		printf("no se envio el paquete \n");
		cliente->desactivar();
	}
	return EXIT_SUCCESS;
}

int Servidor::runRecibirInfo(void* cliente){
	Cliente* client = (Cliente*) cliente;
	while(!client->getNombre());
	while(client->getActivo()){
		SDL_Delay(25);
		char paquete[MAX_PACK];
		memset(paquete, 0, MAX_PACK);
		int cantidad = client->getSocket()->recibir(paquete, MAX_PACK);
		if(cantidad >0){
			structEvento* evento = (structEvento*) paquete;
			void* novedad = malloc (sizeof (structEvento));
			//SDL_Lock(client->getMutex());
			memcpy(novedad, paquete, sizeof (structEvento)); //todo ver como determinar el tamaño del paquete
			if (this->paquetesRecibir.empty()) this->paquetesRecibir.push(novedad);
			structEvento* anterior = (structEvento*) this->paquetesRecibir.front();
			if (evento == NULL) continue;
			printf(" ENTRA EN RECIBIR INFO \n");
			if (anterior->aleatorio != evento->aleatorio){
				if (evento->click_mouse.x == -1 && evento->direccion==-9 && evento->click_mouse.y == -1 ){
					printf("NO ENCOLA \n");
				}
				//printf(" ENCOLAAAA ALGO \n");
				else{
					this->paquetesRecibir.push(novedad);
				}
			}
			int cantidad = (int) this->paquetesRecibir.size();
			printf("CANTIDAD DE PAQUETES EN LA COLA ES %d \n" ,cantidad);
			//SDL_UnlockMutex(client->getMutex());
		}
		else if(cantidad == 0){
			printf("Cliente desconectado\n");
			client->desactivar();
//			client->setConexion(false);
//			int i;
//			for(i=0;i<this->cantClientes; i++){
//				if((this->clientes[i]->getID() == client->getID()) && (strcmp(this->clientes[i]->getNombre(),client->getNombre())==0)){
//					this->clientes[i]->setConexion(false); //ver si es necesario
//				}
//			}
//			ver si this->cantClientes--;
//			todo desconexion cliente
//			break;
		}
		else if(cantidad ==-1){
			client->desactivar();
			printf("Error al recibir información del cliente\n");
			break;
		}
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
	while(!this->finalizar){
		while((!this->finalizar) && (this->cantClientes < this->cantidadMaxConexiones)){
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

int Servidor::recibirNombre(Cliente *client){
	char buffer[MAX_NAME_USER];
	int bytes_recibidos = client->getSocket()->recibir(buffer, MAX_NAME_USER);
	client->setNombre(buffer);
	return bytes_recibidos;
}

// Retorna un número positivo o cero [n], en caso de aceptar al nuevo cliente,
// siendo n la posicion a ocupar en el array clientes:
//		- Es nuevo y hay espacio para crearlo.
//		- Está creado e inactivo.
// Retorna -1 si:
//		- Está creado y activo.
// 		- No hay más espacio para crearlo.
int Servidor::checkNuevoCliente(Cliente *client){
//	while (!client->getNombre());
	int indice = 0;
	Cliente *cliente_recorrido = this->clientes[indice];
	while(cliente_recorrido != NULL ){
		if(!(strcmp(cliente_recorrido->getNombre(), client->getNombre()))){
			if(cliente_recorrido->getActivo()) {
				printf("Cliente existente: %s", cliente_recorrido->getNombre() );
				return -1; // El nombre está en uso y Activo.
			} else return indice; //Devuelve la posicion del Cliente Inactivo
		}
		indice++;
		if (indice == MAX_CANT_JUGADORES) break;
		cliente_recorrido = this->clientes[indice];
	}
	if(this->cantClientes >= MAX_CANT_JUGADORES) return -1; //No hay más espacio para nuevos Clientes
	return indice; // Devuelve la posicion donde será guardado el nuevo Cliente
}

bool Servidor::getFinalizar(){
	return this->finalizar;
}

void Servidor::setThreadEscuchar(SDL_Thread *listen){
	this->escuchar = listen;
}
void Servidor::setThreadAceptar(SDL_Thread *accept){
	this->aceptar = accept;
}

void Servidor::setAceptado(bool aceptar){
	((structInicial* )this->paqueteInicial)->cliente_aceptado = aceptar; //Todo check
}
Cliente** Servidor::getClientes(){
	return this->clientes;
}


int Servidor::getCantidadClientesActivos(){
	int activos = 0;
	for(int i = 0; i < this->cantClientes; i++){
		if(this->clientes[i] == NULL)
			if(this->clientes[i]->getActivo()) activos++;
	}
	this->clientesActivos = activos;
	return activos;
}

void Servidor::setFinalizar(bool condicion){
	this->finalizar = condicion;
}
