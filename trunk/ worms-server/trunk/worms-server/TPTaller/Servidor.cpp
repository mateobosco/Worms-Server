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
	memset(this->mensaje_mostrar, 0, MAX_MENSAJE);
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
	return servidor->runRecibirInfo(cliente);
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

		Cliente* cliente = new Cliente(sockCliente);
		bool recibio_nombre = false;
		//SDL_LockMutex(this->mutex);
		while (!recibio_nombre){
			int bytes = this->recibirNombre(cliente);
			if(bytes > 0 ) recibio_nombre = true;
			if(bytes <= 0){
				printf("Error al recibir nombre del cliente\n");
				return EXIT_FAILURE;// TODO Verificar qué pasa si la # de Bytes es -1 o 0;
			}
		}
		if(recibio_nombre){
			printf( "Socket CL %s: %i \n", cliente->getNombre(), sockCliente);
			int posicion = this->checkNuevoCliente(cliente);
			if(this->getCantidadClientesActivos() < this->getCantidadMaxConexiones()){
				if(posicion != -1){
					Cliente *cliente_viejo = this->clientes[posicion];
					if (cliente_viejo != NULL){
						char mensaje[MAX_MENSAJE];
						strcpy(mensaje, "Se ha reconectado ");
						strcat(mensaje, cliente_viejo->getNombre());
						this->setMensajeMostrar(mensaje);
						cliente->setJugador(cliente_viejo->getJugador());
						cliente->getJugador()->conectar();
						cliente->setID(cliente_viejo->getID());
						delete cliente_viejo;
					} else{
						this->cantClientes++;
					}
					this->setAceptado(true);
					cliente->activar();
					if (this->runEnviarInfoInicial(cliente) <= 0 ) /*log Error todo */;
					conexion_t par;
					par.cliente = cliente;
					par.servidor = this;

					comThreads hilosCliente;
					hilosCliente.enviar = SDL_CreateThread(runSendInfo,"enviar",(void*)&par);
					if(hilosCliente.enviar ==NULL){
						//log error todo
					}
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
					//this->runEnviarInfoInicial(cliente);
					if (this->runEnviarInfoInicial(cliente) <= 0 ) /*log Error todo */;
					delete cliente;
					return EXIT_FAILURE;
				}
			} else{
				this->setAceptado(false);
				loguear();
				logFile << "Cliente rechazado \n" << endl;
				if(this->runEnviarInfoInicial(cliente)<=0)	return EXIT_FAILURE;
			}
		}else {
			delete cliente;
			return EXIT_FAILURE;
		}
	}else{
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
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

	struct timeval timeout;
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;

	while(cliente->getActivo()){
		if (this->enviar == false){
			continue;
		}
		SDL_Delay(25);
		char envio[MAX_PACK];
		memset(envio,0,MAX_PACK);
		char envio2[MAX_PACK];
		memset(envio2,0,MAX_PACK);
		//SDL_LockMutex(this->mutex);
		memcpy(envio, this->paqueteEnviar, MAX_PACK);
		//SDL_UnlockMutex(this->mutex);
		structPaquete* paqueteCiclo = (structPaquete*) envio;
		paqueteCiclo->id=cliente->getID();
		memcpy(envio2, paqueteCiclo, MAX_PACK);
		if (setsockopt (cliente->getSocket()->getFD(), SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,
			sizeof(timeout)) < 0) return ERROR;
		int enviados = cliente->getSocket()->enviar(envio2, MAX_PACK);

		if (enviados > 0){
			this->enviar = false;
		}
		else if(enviados == -1){ // no se pudo enviar
			printf("SV-Enviar: Error -1 Cliente %s\n", cliente->getNombre());
			showTime();
			cliente->desactivar();
		}
		if(enviados == 0){
			printf("SV-Enviar: 0b = Cliente %s desconectado \n", cliente->getNombre());
			showTime();
			cliente->desactivar();
		}
	}
	return EXIT_SUCCESS;
}

int Servidor::runEnviarInfoInicial(Cliente* cliente){
	SDL_Delay(25);
	char envio[MAX_PACK];
	memset(envio,0,MAX_PACK);
	//SDL_LockMutex(this->mutex);
	memcpy(envio, this->paqueteInicial, MAX_PACK);
	//SDL_UnlockMutex(this->mutex);

	struct timeval timeout;
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;

    if (setsockopt (cliente->getSocket()->getFD(), SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,
                sizeof(timeout)) < 0) return ERROR;

	int enviados = cliente->getSocket()->enviar(envio, MAX_PACK);

	if (enviados > 0){
		this->enviar = false;
	}
	if(enviados == 0){
		printf("SV-EnviarInicial: 0b = Cliente %s desconectado \n", cliente->getNombre());
		showTime();
		cliente->desactivar();
	}
	if(enviados == -1){ // no se pudo enviar
		printf("SV-EnviarInicial: Error -1 Cliente %s\n", cliente->getNombre());
		showTime();
		cliente->desactivar();
	}
	return EXIT_SUCCESS;
}

int Servidor::runRecibirInfo(void* cliente){
	struct timeval timeout;
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;
	Cliente* client = (Cliente*) cliente;

	while(!client->getNombre()) printf("Falta Nombre");
	while(client->getActivo()){
		SDL_Delay(25);
		char paquete[MAX_PACK];
		memset(paquete, 0, MAX_PACK);
		if (setsockopt (client->getSocket()->getFD(), SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
			sizeof(timeout)) < 0) return ERROR;
		int cantidad = client->getSocket()->recibir(paquete, MAX_PACK);
		if(cantidad >0){
			structEvento* evento = (structEvento*) paquete;
			void* novedad = malloc (MAX_PACK);
			//SDL_LockMutex(this->mutex);
			memcpy(novedad, paquete, MAX_PACK); //todo ver como determinar el tamaño del paquete
			SDL_LockMutex(this->mutex);
			if (this->paquetesRecibir.empty()) this->paquetesRecibir.push(novedad);
			structEvento* anterior = (structEvento*) this->paquetesRecibir.front();
			SDL_UnlockMutex(this->mutex);

			if (evento == NULL) continue;
			if (anterior == NULL) continue;
			if (anterior->aleatorio != evento->aleatorio){
				if (evento->click_mouse.x == -1 && evento->direccion==-9 && evento->click_mouse.y == -1 ){
				}
				else{
					this->paquetesRecibir.push(novedad);
				}
			}
		}
		else if(cantidad == 0){
			printf("SV-Recibir: Se ha desconectado el cliente %s.\n", client->getNombre());
			showTime();
			loguear();
			logFile << "Cliente: " << client->getNombre() << "desconectado "<< endl;
			client->desactivar();
			char mensaje[MAX_MENSAJE];
			strcpy(mensaje, "Se ha desconectado ");
			strcat(mensaje, client->getNombre());
			this->setMensajeMostrar(mensaje);

		}
		else if(cantidad ==-1){
			printf("SV-Recibir: Error -1 Cliente: %s\n", client->getNombre());
			showTime();
			client->desactivar();
			loguear();
			logFile << "Error al recibir información del cliente: " << client->getNombre() << endl;
//			break;
		}
	}
	return EXIT_SUCCESS;
}

int Servidor::runEscucharConexiones(){
	int conexiones;
	try{
		conexiones = this->getSocket()->EnlazarYEscuchar(this->cantidadMaxConexiones);
	}catch(exception &e){
		//loguear error
		loguear();
		logFile << "No se pudo enlazar y escuchar" << endl;
		close(this->listener->getFD());
		return EXIT_FAILURE;

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
			loguear();
			logFile << "Se alcanzó la máxima cantidad de clientes " << endl;
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
	memset(buffer,0,MAX_NAME_USER);
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
		if(this->clientes[i] != NULL)
			if(this->clientes[i]->getActivo()) activos++;
	}
	this->clientesActivos = activos;
	return activos;
}

void Servidor::setFinalizar(bool condicion){
	this->finalizar = condicion;
}

char* Servidor::getMensajeMostrar(){
	return this->mensaje_mostrar;
}

void Servidor::setMensajeMostrar(char* mensaje){
	strncpy(this->mensaje_mostrar, mensaje, 50);
}
