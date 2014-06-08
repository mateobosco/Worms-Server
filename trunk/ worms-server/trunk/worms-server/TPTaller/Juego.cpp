#include "Juego.h"

Juego::Juego(){
	mundo = NULL;

	cantidad_jugadores = 0;

	escalador = NULL;
	lector = NULL;

	figuras = NULL;
	cantidad_figuras = 0;

	cielo.clear();
	agua = NULL;
	inicial = NULL;

	this->cargar();
	manejador = new ManejadorPersonajes();

	for (int i =0; i< 4; i++){
		jugadores[i]=NULL;
	}
	jugador_actual = 0;
	reloj_ronda=0;
	indice_jugador_turno=0;
	arma_actual = new Arma();
	proj_in_air = false;
}

Juego::~Juego(){
	for(size_t i= 0; i < cantidad_figuras ; i++){
		if (figuras != NULL){
			if(figuras[i] != NULL){
				delete figuras[i];
			}
		}
	}
	delete[] figuras;
	delete agua;
	delete lector;
	delete mundo;
	delete manejador;
	delete escalador;
	for(Uint8 i = 0; i < cantidad_jugadores; i++){
		if(jugadores[i] != NULL){
			delete jugadores[i];
		}
	}
	if(arma_actual){
		delete arma_actual;
	}
}

Jugador** Juego::getJugadores(){
	return jugadores;
}

uint8 Juego::getCantidadJugadores(){
	return cantidad_jugadores;
}

Mundo* Juego::getMundo(){
	return mundo;
}

Figura** Juego::getFiguras(){
	return figuras;
}

LectorMascara* Juego::getLector(){
	return lector;
}

Agua* Juego::getAgua(){
	return agua;
}

Escalador* Juego::getEscalador(){
	return escalador;
}

string Juego::getCielo() {
	return cielo;
}

uint8 Juego::getCantidadFiguras(){
	return cantidad_figuras;
}

void Juego::cargar() {
	this->abrirLog();
	Cargador *cargador = new Cargador(pathEscTest.c_str());
	Node *nodo_escenario = this->cargaInicial(cargador);
	this->cargaPrincipal(cargador, *nodo_escenario);
	inicial = cargador->getPaqueteInicial();
	delete nodo_escenario;
	delete cargador;
}

// Funciones Privadas:

void Juego::abrirLog(){
	logFile.open("log.txt",ios::out);
	logFile << "Log File. Ejecución Worms" << endl;
	logFile << "   Fecha  | Hora |  Gravedad  | \t Mensaje " << endl;
}

Node* Juego::cargaInicial(Cargador* cargador){
	Node *nodo_escenario =  new Node();
	if(!((cargador->getNodo(cargador->getNodo(),*nodo_escenario,"esc"))||(cargador->getNodo(cargador->getNodo(),*nodo_escenario,"escenario"))||(cargador->getNodo(cargador->getNodo(),*nodo_escenario,"esce"))||(cargador->getNodo(cargador->getNodo(),*nodo_escenario,"escen"))||
			(cargador->getNodo(cargador->getNodo(),*nodo_escenario,"escena"))||(cargador->getNodo(cargador->getNodo(),*nodo_escenario,"escenar"))||(cargador->getNodo(cargador->getNodo(),*nodo_escenario,"escenari"))||
			(cargador->getNodo(cargador->getNodo(),*nodo_escenario,"Esc"))||(cargador->getNodo(cargador->getNodo(),*nodo_escenario,"ESC"))||(cargador->getNodo(cargador->getNodo(),*nodo_escenario,"ESCENARIO"))||
			(cargador->getNodo(cargador->getNodo(),*nodo_escenario,"Escenario"))||(cargador->getNodo(cargador->getNodo(),*nodo_escenario,"Esc")))){
		loguear();
		logFile << "    Error   " << "\t No se encuentra el escenario. Se carga escenario por defecto."<<endl;
		delete cargador;
		cargador = new Cargador(pathDefEs.c_str());
		(*nodo_escenario) = cargador->getNodo()["escenario"];
	}
	return nodo_escenario;
}

void Juego::cargarEscalador(Cargador *cargador, Node nodo_escenario){
	this->escalador = cargador->loadEscalador(nodo_escenario);
	if(!this->escalador){
		loguear();
		logFile << "    Error   " << "\t  No se pudo escalar la información "<< endl;
	}
}

void Juego::cargarAgua(Cargador *cargador, Node nodo_escenario){
	string imagen_agua = pathAgua;
	this->agua = cargador->loadAgua(nodo_escenario, imagen_agua);
	if(!this->agua){
		loguear();
		logFile << "    Error   " << "\t  No se pudo crear el agua. "<< endl;
	}
}

void Juego::cargarMundo(){
	b2Vec2 escalas = b2Vec2(this->escalador->getEscalaX(), this->escalador->getEscalaY());
	this->mundo = new Mundo(b2Vec2(GRAVEDAD_X,GRAVEDAD_Y), this->agua, escalas);
	if(!this->mundo){
		loguear();
		logFile << "    Error   " << "\t  No se pudo crear el mundo."<< endl;
	}
}

string Juego::cargarTierra(Cargador *cargador, Node nodo_escenario){
	string tierra;
	if(!((cargador->loadPath(nodo_escenario, "imagen_tierra",tierra))||(cargador->loadPath(nodo_escenario, "tierra",tierra))||(cargador->loadPath(nodo_escenario, "imagen-tierra",tierra)))){
		loguear();
		logFile << "    Error  " <<"\t No se pudo cargar el path correspondiente a la clave buscada. " << endl;
		tierra = pathDefMas;
	}
	return tierra;
}

void Juego::cargarCielo(Cargador *cargador, Node nodo_escenario){
	string cielo;
	if(!((cargador->loadPath(nodo_escenario, "imagen_cielo",cielo))||(cargador->loadPath(nodo_escenario, "cielo",cielo))||(cargador->loadPath(nodo_escenario, "imagen-cielo",cielo)))){
		loguear();
		logFile << "    Error  " <<"\t No se pudo cargar el path correspondiente a la clave buscada. " << endl;
		cielo = pathDefCielo;
	}
}

void Juego::cargarLector(string tierra){
	this->lector = new LectorMascara(tierra);
	if(!this->lector){
		loguear();
		logFile << "    Error   " << "\t  No se pudo crear el L ector de Máscara. " <<  SDL_GetError()<< endl;
	}
}

void Juego::generarTierra(){
	b2Vec2* vectorTierra = this->lector->LeerMascara(this->escalador);
	this->mundo->Crear_Chains(vectorTierra, this->lector->GetPixelAncho());
}

void Juego::cargarFiguras(Cargador *cargador, Node nodo_escenario){
	Node objetos;
	if((cargador->getNodo(nodo_escenario,objetos,"objetos"))||(cargador->getNodo(nodo_escenario,objetos,"obj"))||
			(cargador->getNodo(nodo_escenario,objetos,"o"))||(cargador->getNodo(nodo_escenario,objetos,"Objetos"))||(cargador->getNodo(nodo_escenario,objetos,"OBJETOS"))){
		this->cantidad_figuras = objetos.size();
		this->figuras = new Figura*[this->cantidad_figuras];
		for(size_t i = 0; i < this->cantidad_figuras; i++){
			this->figuras[i] = cargador->cargarFigura(objetos[i], this->mundo, this->escalador, i + 1);
		}
	}else{
		loguear();
		logFile << "    Warning " << "\t  No hay objetos para cargar "<< endl;
	}
}

//Retorna el mundo cargado y si hubo algún error terminante retorna NULL.
void Juego::cargaPrincipal(Cargador *cargador, Node nodo_escenario){
	cargarEscalador(cargador, nodo_escenario);
	cargarAgua(cargador, nodo_escenario);
	cargarMundo();
	string tierra = cargarTierra(cargador, nodo_escenario);
	cargarCielo(cargador, nodo_escenario);
	cargarLector(tierra);
	if(this->lector == NULL){
		delete this->mundo;
		this->mundo = NULL;
	}
	generarTierra();
	cargarFiguras(cargador, nodo_escenario);
}

structInicial* Juego::getPaqueteInicial(){
	return this->inicial;
}

void Juego::aplicarPaquete(structEvento* evento, int comenzar){
	if (evento == NULL) return;
//	if (!proj_in_air){ // TODO MODIFICAR PARA ARMAR QUE PERMITEN MOVERSE 3 SEG DESPUES DE DISPARADAS
		if (evento->click_mouse.x != -1){ // recibio un click
			manejador->seleccionarPersonaje(evento->click_mouse, evento->nro_jugador);
		}
		if ((evento->direccion > 0) && (evento->nro_jugador == this->getJugadorActual()) && comenzar ==1){ // PROCESO EL MOVIMIENTO SOLO SI ES SU TURNO
			manejador->moverPersonaje(evento->direccion , evento->nro_jugador);
			//printf(" APLICO UN PAQUETE MOVIMIENTO, PASO DE TURNO");
			//this->pasarTurno();
		}
		if (evento->arma_seleccionada != 0 ){
			for (int j = 0; j < manejador->getCantidadPersonajes(); j++){ // TODO ver si es necesario cant actuales activos.
				Personaje* personaje_actual = manejador->getPersonajes()[j];
				if (! personaje_actual->getMuerto()){
					if( personaje_actual->getSeleccion()[evento->nro_jugador] ){
						printf( " SE LE ASIGNO EL PERSONAJE SELECCIONADO EL ARMA %d \n", evento->arma_seleccionada);
						personaje_actual->setArmaSeleccionada(evento->arma_seleccionada);
						personaje_actual->setAnguloArma(evento->angulo_arma);
//						printf("evento arma \n ");
//						printf("posicion: x- %f y- %f \n", personaje_actual->getPosition().x, personaje_actual->getPosition().y);
						this->setArma(evento->arma_seleccionada, personaje_actual->getPosition(), personaje_actual->getAnguloArma() );
					}
				}
			}
		}
		if(evento->angulo_arma != 0){
			printf( " /////////////////////////////////////////////// \n");
			for (int j = 0; j < manejador->getCantidadPersonajes(); j++){ // TODO ver si es necesario cant actuales activos.
				Personaje* personaje_actual = manejador->getPersonajes()[j];
				if (! personaje_actual->getMuerto()){
					if( personaje_actual->getSeleccion()[evento->nro_jugador] ){
						printf( " SE LE ASIGNA EL ARMA UN ANGULO %d \n", evento->angulo_arma);
						//personaje_actual->setArmaSeleccionada(evento->arma_seleccionada);
						personaje_actual->setAnguloArma(evento->angulo_arma);
						this->arma_actual->setAngulo(personaje_actual->getAnguloArma());
						printf(" EL ANGULO NEUVO ES %d \n SELECCIONADA ES %i \n", personaje_actual->getAnguloArma(), evento->arma_seleccionada);
					}
				}
			}
		}
		if(evento->fuerza == 1){
			arma_actual->setFuerza();
		} else
			if(evento->fuerza == 2) {
				this->disparar();
				arma_actual->resetFuerza();
			}

//	if(evento->fuerza == 2){
//		printf("Dejó de disparar\n");
//	}
	//else return;
//	}
}

Jugador* Juego::agregarJugador(int id, char* nombre_cliente){
	this->jugadores[id] = new Jugador(mundo,id,manejador, nombre_cliente);
	Personaje** pers = this->jugadores[id]->getPersonajes();
	this->manejador->AgregarJugador(mundo,id, pers);
	this->jugadores_jugando.push_back(this->jugadores[id]);
	return this->jugadores[id];
}

ManejadorPersonajes* Juego::getManejadorPersonajes(){
	return this->manejador;
}




int Juego::getJugadorActual(){
	return indice_jugador_turno;
}

void Juego::pasarTurno(){
	//reloj_ronda = SDL_GetTicks();
	this->resetearRelojRonda();
	//jugador_actual++;
	indice_jugador_turno++;
	if(indice_jugador_turno == 1){
		indice_jugador_turno = 0;
	}
	printf(" LLEGA HASTA ACA 1  indice jugador turno es %d \n", indice_jugador_turno);
	Jugador* jugador_actual = jugadores_jugando.at(indice_jugador_turno);
	while(jugador_actual->getConectado() == false){
		//printf(" ENTRA ACAAAAA \n");
		indice_jugador_turno++;

		jugador_actual = jugadores_jugando.at(indice_jugador_turno);
		if (indice_jugador_turno > jugadores_jugando.size()){
			indice_jugador_turno=0;
		}
	}
	//printf(" LLEGA HASTA ACA 2 \n");
	//if(jugador_actual == 2){
	//	jugador_actual =0;
	//}


	if (indice_jugador_turno > jugadores_jugando.size()){
		indice_jugador_turno = 0;
	}
	for(int i = 0; i < jugadores_jugando.size(); i++){
		Jugador* jugador_actual = jugadores_jugando.at(i);
		//Cliente* clienteActual = servidor->getClientes()[i];
		Personaje** vector_personajes = jugador_actual->getPersonajes();
		for (int i = 0; i < 4; i++){
			Personaje* personaje_actual = vector_personajes[i];
			if (personaje_actual->getEnergia() == 0){
				personaje_actual->setMuerto();
			}
		}
		if(jugador_actual->getPerdio()){
			//jugadores_jugando.erase(jugadores_jugando.begin() + i);
		}
	}




}

int Juego::getRelojRonda(){
	return (SDL_GetTicks()-reloj_ronda);
}

void Juego::resetearRelojRonda(){
	reloj_ronda=SDL_GetTicks();
}

void Juego::setArma(int tipo_arma, b2Vec2 posicion, int angulo){
	this->arma_actual->setTipo(tipo_arma);
	this->arma_actual->setPosicion(posicion);
	this->arma_actual->setAngulo(angulo);
}

void Juego::disparar(){
	if(arma_actual->getTipo() != 0){
		arma_actual->disparar(this->mundo);
		proj_in_air = true;
	} else{
//		printf("No hay arma seleccionada \n");
	}
}

void Juego::checkColisionProyectil(){
	if(proj_in_air){
		if(arma_actual->checkImpacto(this->mundo)){
			proj_in_air = false;
			this->arma_actual->setTipo(0);
			this->mundo->destruir_cuerpo(arma_actual->getProyectil());
			this->arma_actual->aplicarExplosion();
		} else{
			//b2Vec2 antigravedad = b2Vec2(0,-0.98f);
			//arma_actual->getProyectil()->ApplyForceToCenter(antigravedad, true );
		}
	}
}

void Juego::setPaqueteProyectil(structPaquete *pack){
	pack->show_proyectil = this->proj_in_air;
	if (proj_in_air){
		pack->tipo_proyectil = this->arma_actual->getTipo();
		pack->posicion_proyectil = this->arma_actual->getPosicion();
		pack->direccion_proyectil = this->arma_actual->getDireccion();
		pack->tamanio_proyectil = this->arma_actual->getTamanio();
	} else{
		pack->tipo_proyectil = 0;
		pack->posicion_proyectil = b2Vec2(0,0);
		pack->direccion_proyectil = b2Vec2(0,0);
		pack->tamanio_proyectil = b2Vec2(0,0);
	}
}
