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
		bool conectado;
		char* nombre;
		int personaje_seleccionado;

	public:
		Jugador(Mundo *mundo, uint8 cantidad, ManejadorPersonajes *manejador, char* nombre_cliente);
		~Jugador();
		Personaje** getPersonajes();
		Personaje* getPersonaje(Uint8 numero_pj);
		Uint8 getNumero();
		void conectar();
		SDL_Color getColor();
		void desconectar();
		bool getPerdio();
		bool getConectado();
		char* getNombre();
		void seleccionarSiguientePersonaje();
		int getPersonajeSeleccionado();
		void reiniciarJugador(ManejadorPersonajes* manejador, Mundo* mundo);
		bool tienePersonajesVivos(uint8 cantidad_jugadores);
		int cantidad_holy;
		int cantidad_granadas;
		int cantidad_bazookas;
		int cantidad_dinamita;

		int getCantBazooka();
		int getCantGranadas();
		int getCantDinamita();
		int getCantHoly();

		void disminuirCantBazooka();
		void disminuirantGranadas();
		void disminuirCantDinamita();
		void disminuirCantHoly();
};

#endif /* JUGADOR_H_ */
