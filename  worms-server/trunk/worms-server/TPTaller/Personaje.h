/*
 * Personaje.h
 *
 *  Created on: 24/04/2014
 *      Author: juanmahidalgo
 */

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
	Personaje(Mundo* mundo, Uint8 numero_jugador);
	void mover(b2Vec2 direccion);
	void saltar(b2Vec2 direccion);
	void dejar_quieto();
	SDL_Texture* dibujar(Dibujador* el_dibujador);
	void leermovimiento(int direccion, int id_jugador);
	void setSeleccionado(bool seleccion, int id_jugador);
	b2Vec2 getPosition();
	char* getDirImagen();
	void setMuerto();
	bool getMuerto();
	void setConectado(int estado);
	int getConectado();

	float32 getAlto();
	float32 getAncho();
	float32* getVecX();
	float32* getVecY();
	b2Vec2* getVertices();
	b2Body* getBody();
	b2PolygonShape* getShape();
	bool getSeleccionado();
	int getNrojugador();
	virtual ~Personaje();


private:
	char* dir_imagen;
	b2Body* body;
	b2PolygonShape* shape;
	Uint8 nro_jugador;
	bool seleccionado[4];
	float32 ancho;
	float32 alto;
	bool muerto;
	int conectado;

};

#endif /* PERSONAJE_H_ */
