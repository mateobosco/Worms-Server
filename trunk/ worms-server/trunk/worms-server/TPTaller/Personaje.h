#ifndef PERSONAJE_H_
#define PERSONAJE_H_

#include <cmath>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <Box2D/Box2D.h>
#include <SDL2/SDL_pixels.h>
#include <Box2D/Common/b2Math.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <time.h>
#include <random>
#include <ctime>

#include "mundo.h"

class Personaje {
public:
	Personaje(Mundo* mundo, Uint8 numero_jugador, char* nombre_cliente);
	void mover(b2Vec2 direccion);
	SDL_Texture* dibujar(Dibujador* el_dibujador);
	void leermovimiento(int direccion, int id_jugador);
	void setSeleccionado(bool seleccion, int id_jugador);
	b2Vec2 getPosition();
	const char* getDirImagen();
	void setMuerto();
	bool getMuerto();
	void setConectado(int estado);
	int getConectado();
	int getOrientacion();
	void desconectar();
	void conectar();
	char* getNombreCliente();
	float32 getAlto();
	float32 getAncho();
	float32* getVecX();
	float32* getVecY();
	b2Vec2* getVertices();
	b2Body* getBody();
	b2CircleShape* getShape();
	bool getSeleccionado();
	int getNrojugador();
	virtual ~Personaje();
	bool* getSeleccion();
	int getEnergia();
	void setArmaSeleccionada(int nro_arma);
	int getArmaSeleccionada();
	int getAnguloArma();
	void setAnguloArma(int angulo);
	void quitarEnergia(int descontar);

private:
	const char* dir_imagen;
	char* nombre_cliente;
	b2Body* body;
	b2PolygonShape* shape;

	Uint8 nro_jugador;
	bool seleccionado[4];
	float32 ancho;
	float32 alto;
	bool muerto;
	int conectado;
	int orientacion;
	int arma_seleccionada;
	int energia;
	int angulo_arma;

	b2CircleShape* shape2;
	float32 radio;
};

#endif /* PERSONAJE_H_ */
