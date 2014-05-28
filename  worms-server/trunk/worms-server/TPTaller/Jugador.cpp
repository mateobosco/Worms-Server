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
	seleccionarColor();
	for(int i = 0; i < MAX_CANT_PERSONAJES; i++){
		this->personajes[i] = new Personaje(mundo, this->numero, nombre_cliente);
		manejador->agregarPersonaje(this->personajes[i], this->numero);
	}
//	Jugador::cantidad_jugadores++;
}

Jugador::~Jugador(){
	for(int i = 0; i < MAX_CANT_PERSONAJES; i++){
		if(this->personajes[i] != NULL){
			delete this->personajes[i];
		}
	}
//	Jugador::cantidad_jugadores--;
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
		if((this) && (this->personajes[i]))
			this->personajes[i]->desconectar();
	}
}
