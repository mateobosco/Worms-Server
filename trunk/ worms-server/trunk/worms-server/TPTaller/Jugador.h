#ifndef JUGADOR_H_
#define JUGADOR_H_

#include <SDL2/SDL.h>

#include "Personaje.h"
#include "Constantes.h"
#include "ManejadorPersonajes.h"

class Jugador{
	private:
		uint8 numero;
		SDL_Color color;
		Personaje *personajes[MAX_CANT_PERSONAJES];
		void seleccionarColor();
//		static Uint8 cantidad_jugadores;

	public:
		Jugador(Mundo *mundo, uint8 cantidad, ManejadorPersonajes *manejador);
		~Jugador();
		Personaje** getPersonajes();
		Personaje* getPersonaje(Uint8 numero_pj);
		Uint8 getNumero();
		SDL_Color getColor();
};

//Uint8 Jugador::cantidad_jugadores = 1;

#endif /* JUGADOR_H_ */
