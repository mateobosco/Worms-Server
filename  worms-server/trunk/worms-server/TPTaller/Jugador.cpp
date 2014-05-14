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

Jugador::Jugador(Mundo *mundo, uint8 cantidad, ManejadorPersonajes *manejador){
	numero = cantidad;
	seleccionarColor();
	for(Uint8 i = 0; i < MAX_CANT_PERSONAJES; i++){
		personajes[i] = new Personaje(mundo, numero);
		manejador->agregarPersonaje(personajes[i], numero);
	}
//	Jugador::cantidad_jugadores++;
}

Jugador::~Jugador(){
	for(Uint8 i = 0; i < MAX_CANT_PERSONAJES; i++){
		if(personajes[i] != NULL){
			delete personajes[i];
		}
	}
//	Jugador::cantidad_jugadores--;
}

Personaje** Jugador::getPersonajes(){
	return personajes;
}

Personaje* Jugador::getPersonaje(Uint8 numero_pj){
	return personajes[numero_pj];
}

Uint8 Jugador::getNumero(){
	return numero;
}

SDL_Color Jugador::getColor(){
	return color;
}
