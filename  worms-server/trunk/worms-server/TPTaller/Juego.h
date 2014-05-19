#ifndef JUEGO_H_
#define JUEGO_H_

#include "Cargador.h"
#include "Jugador.h"

class Juego{
	private:
		Mundo *mundo;

		uint8 cantidad_jugadores;
		Jugador *jugadores[MAX_CANT_JUGADORES];
		ManejadorPersonajes *manejador;
		Escalador *escalador;
		LectorMascara *lector;

		Figura **figuras;
		uint8 cantidad_figuras;
		string cielo;
		Agua *agua;

		void abrirLog();
		Node* cargaInicial(Cargador* cargador);
		void cargarEscalador(Cargador *cargador, Node nodo_escenario);
		void cargarAgua(Cargador *cargador, Node nodo_escenario);
		void cargarMundo();
		string cargarTierra(Cargador *cargador, Node nodo_escenario);
		void cargarCielo(Cargador *cargador, Node nodo_escenario);
		void cargarLector(string tierra);
		void generarTierra();
		void cargarFiguras(Cargador *cargador, Node nodo_escenario);
		void cargaPrincipal(Cargador *cargador, Node nodo_escenario);


		void cargar();
		structInicial* inicial;


	public:
		Juego();
		~Juego();
		uint8 crearJugador();
		Jugador** getJugadores();
		uint8 getCantidadJugadores();
		Mundo* getMundo();
		Figura** getFiguras();
		LectorMascara* getLector();
		Agua* getAgua();
		Escalador* getEscalador();
		string getCielo();
		uint8 getCantidadFiguras();
		int run();
		structInicial* getPaqueteInicial();
		void aplicarPaquete(structEvento* evento, ManejadorPersonajes* manejador_personajes);


};

#endif /* JUEGO_H_ */
