#ifndef JUEGO_H_
#define JUEGO_H_

#include "Cargador.h"
#include "Jugador.h"
#include "Arma.h"

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
		int jugador_actual;
		int indice_jugador_turno;
		int reloj_ronda;
		std::vector<Jugador*> jugadores_jugando;

		void cargar();
		structInicial* inicial;

		bool proj_in_air;
		Arma *arma_actual;

	public:
		Juego();
		~Juego();

		void crearJugador();
		Jugador** getJugadores();
		uint8 getCantidadJugadores();
		Mundo* getMundo();
		Figura** getFiguras();
		LectorMascara* getLector();
		Agua* getAgua();
		Escalador* getEscalador();
		string getCielo();
		uint8 getCantidadFiguras();
		structInicial* getPaqueteInicial();
		void aplicarPaquete(structEvento* evento, int comenzar);
		Jugador* agregarJugador(int id, char* nombre_cliente);
		ManejadorPersonajes* getManejadorPersonajes();
		int getJugadorActual();
		void pasarTurno();
		void resetearRelojRonda();
		int getRelojRonda();

		void setArma(int tipo_arma, b2Vec2 posicion, int angulo);
		void disparar();
		void checkColisionProyectil();
		void setPaqueteProyectil(structPaquete *pack);
};

#endif /* JUEGO_H_ */
