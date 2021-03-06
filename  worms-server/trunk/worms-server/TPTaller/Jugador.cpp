#include "Jugador.h"

void Jugador::seleccionarColor(){
	switch(this->numero){
		case 1: this->color.r = 255;
				this->color.g = 0;
				this->color.b = 0;
				this->color.a = OPACIDAD;
				break;
		case 2: this->color.r = 0;
				this->color.g = 255;
				this->color.b = 0;
				this->color.a = OPACIDAD;
				break;
		case 3: this->color.r = 0;
				this->color.g = 0;
				this->color.b = 255;
				this->color.a = OPACIDAD;
				break;
		case 4: this->color.r = 255;
				this->color.g = 0;
				this->color.b = 255;
				this->color.a = OPACIDAD;
				break;
	}
}

Jugador::Jugador(Mundo *mundo, uint8 cantidad, ManejadorPersonajes *manejador, char* nombre_cliente){
	this->numero = cantidad;
	this->conectado = true;
	this->nombre = nombre_cliente;
	this->personaje_seleccionado=0;
	seleccionarColor();
	for(int i = 0; i < MAX_CANT_PERSONAJES; i++){
		this->personajes[i] = new Personaje(mundo, this->numero, nombre_cliente);
		manejador->agregarPersonaje(this->personajes[i], this->numero);
	}
	this->personajes[personaje_seleccionado]->setSeleccionado(true, numero);
	this->cantidad_holy=1;
	this->cantidad_granadas=3;
	this->cantidad_bazookas=15;
	this->cantidad_dinamita=10;
}

Jugador::~Jugador(){
	for(int i = 0; i < MAX_CANT_PERSONAJES; i++){
		if(this->personajes[i] != NULL){
			if(personajes[i]) delete this->personajes[i]; this->personajes[i] = NULL;
		}
	}
}

Personaje** Jugador::getPersonajes(){
	return this->personajes;
}

Personaje* Jugador::getPersonaje(Uint8 numero_pj){
	return this->personajes[numero_pj];
}

Uint8 Jugador::getNumero(){
	return this->numero;
}

void Jugador::conectar(){
	for( int i =0; i<4; i++){
		this->personajes[i]->conectar();
	}
}

SDL_Color Jugador::getColor(){
	return this->color;
}

void Jugador::desconectar(){
	for( int i =0; i<4; i++){
		if((this))
			this->personajes[i]->desconectar();
	}
}

bool Jugador::getPerdio(){
	bool perdio = true;
	for (int i = 0; i < 4; i++){
		if(!this->personajes[i]->getMuerto()){
			perdio=false;
		}
	}
	return perdio;
}


bool Jugador::getConectado(){
	return this->conectado;
}

char* Jugador::getNombre(){
	return nombre;
}

void Jugador::seleccionarSiguientePersonaje(){
	// agregar qe si el jugador perdio no cambie nada aca
	if(this->getPerdio()){
		return;
	}
	this->personaje_seleccionado++;
	if(this->personaje_seleccionado == 4){
		this->personaje_seleccionado=0;
	}
	for(int i = 0 ; i < 4; i++){
		this->personajes[i]->setSeleccionado(false, this->numero);
	}
	while(this->personajes[personaje_seleccionado]->getMuerto()){
		this->personaje_seleccionado++;
		if(this->personaje_seleccionado == 4){
			this->personaje_seleccionado=0;
		}
	}
	this->personajes[personaje_seleccionado]->setSeleccionado(true, this->numero);
}


int Jugador::getPersonajeSeleccionado(){
	return this->personaje_seleccionado;
}


void Jugador::reiniciarJugador(ManejadorPersonajes* manejador, Mundo* mundo){
	for (int i = 0; i<4;i++){
		if (this->personajes[i]) delete this->personajes[i];
		this->personajes[i] = NULL; //todo

	}
	for(int i = 0; i < MAX_CANT_PERSONAJES; i++){
		this->personajes[i] = new Personaje(mundo, this->numero, this->nombre);
		manejador->agregarPersonaje(this->personajes[i], this->numero);
	}
	//this->personajes[personaje_seleccionado]->setSeleccionado(false, numero);
	this->personaje_seleccionado = 0;
	this->personajes[personaje_seleccionado]->setSeleccionado(true, numero);
}

bool Jugador::tienePersonajesVivos(uint8 cantidad_jugadores){
	for(int i = 0; i < ((MAX_CANT_PERSONAJES * MAX_CANT_JUGADORES) - (MAX_CANT_PERSONAJES * (MAX_CANT_JUGADORES - cantidad_jugadores))); i++){
		if(!this->personajes[i]->getMuerto()){
			return true;
		}
	}
	return false;
}

int Jugador::getCantBazooka(){
	return this->cantidad_bazookas;
}
int Jugador::getCantGranadas(){
	return this->cantidad_granadas;
}
int Jugador::getCantDinamita(){
	return this->cantidad_dinamita;
}
int Jugador::getCantHoly(){
	return this->cantidad_holy;
}

void Jugador::disminuirCantBazooka(){
	this->cantidad_bazookas--;
}
void Jugador::disminuirantGranadas(){
	this->cantidad_granadas--;
}
void Jugador::disminuirCantDinamita(){
	this->cantidad_dinamita--;
}
void Jugador::disminuirCantHoly(){
	this->cantidad_holy--;
}


