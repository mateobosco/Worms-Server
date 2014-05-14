#include "Juego.h"

Juego::Juego(){
	mundo = NULL;

	cantidad_jugadores = 0;
	manejador = NULL;
	escalador = NULL;
	lector = NULL;

	figuras = NULL;
	cantidad_figuras = 0;

	cielo.clear();
	agua = NULL;
	inicial = NULL;

	this->cargar();
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
	for(Uint8 i = 0; i < cantidad_jugadores; i++){
		if(jugadores[i] != NULL){
			delete jugadores[i];
		}
	}
}

uint8 Juego::crearJugador(){
	jugadores[cantidad_jugadores] = new Jugador(mundo, cantidad_jugadores + 1, manejador);
	cantidad_jugadores++;
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
	delete cargador;
}

//TODO ver el tema del retorno
int Juego::run(){


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
	escalador = cargador->loadEscalador(nodo_escenario);
	if(!escalador){
		loguear();
		logFile << "    Error   " << "\t  No se pudo escalar la información "<< endl;
	}
}

void Juego::cargarAgua(Cargador *cargador, Node nodo_escenario){
	string imagen_agua = pathAgua;
	agua = cargador->loadAgua(nodo_escenario, imagen_agua);
	if(!agua){
		loguear();
		logFile << "    Error   " << "\t  No se pudo crear el agua. "<< endl;
	}
}

void Juego::cargarMundo(){
	b2Vec2 escalas = b2Vec2(escalador->getEscalaX(), escalador->getEscalaY());
	mundo = new Mundo(b2Vec2(GRAVEDAD_X,GRAVEDAD_Y), agua, escalas);
	if(!mundo){
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
	lector = new LectorMascara(tierra);
	if(!lector){
		loguear();
		logFile << "    Error   " << "\t  No se pudo crear el Lector de Máscara. " <<  SDL_GetError()<< endl;
	}
}

void Juego::generarTierra(){
	b2Vec2* vectorTierra = lector->LeerMascara(escalador);
	mundo->Crear_Chains(vectorTierra, lector->GetPixelAncho());
}

void Juego::cargarFiguras(Cargador *cargador, Node nodo_escenario){
	Node objetos;
	if((cargador->getNodo(nodo_escenario,objetos,"objetos"))||(cargador->getNodo(nodo_escenario,objetos,"obj"))||
			(cargador->getNodo(nodo_escenario,objetos,"o"))||(cargador->getNodo(nodo_escenario,objetos,"Objetos"))||(cargador->getNodo(nodo_escenario,objetos,"OBJETOS"))){
		cantidad_figuras = objetos.size();
		figuras = new Figura*[cantidad_figuras];
		for(size_t i = 0; i < cantidad_figuras; i++){
			figuras[i] = cargador->cargarFigura(objetos[i], mundo, escalador, i + 1);
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
	if(lector == NULL){
		delete mundo;
		mundo = NULL;
	}
	generarTierra();
	cargarFiguras(cargador, nodo_escenario);
}

structInicial* Juego::getPaqueteInicial(){
	return inicial;
}


void Juego::aplicarPaquete(structEventos* evento, ManejadorPersonajes* manejador_personajes){
	if (evento->click_mouse.x != -1){ // recibio un click
		manejador_personajes->seleccionarPersonaje(evento->click_mouse, evento->nro_jugador);
	}
	if (evento->direccion != -9){ // recibio un click
		manejador_personajes->moverPersonaje(evento->direccion , evento->nro_jugador);
	}
}

