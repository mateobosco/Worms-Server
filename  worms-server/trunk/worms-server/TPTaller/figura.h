#ifndef FIGURA_H
#define FIGURA_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <Box2D/Box2D.h>
#include <SDL2/SDL_pixels.h>

#include "Dibujable.h"

class Figura : public Dibujable{
	public:
		// inicializa las varibles
		Figura(	SDL_Color color, float32 rot);
		// destruye la figura
		virtual ~Figura();

		virtual int dibujar(Dibujador* un_dibujador) = 0;

		// devuelve el body de box2d
		b2Body* getBody();

		// devuelve la posicion del body de box2d
		b2Vec2 obtenerPosicion();

		SDL_Color getColor();

		float32 getRot();

		void setPosicionInicial();

		void Hundir();

		int tipo;
	protected:
		b2Body* body;
		SDL_Color color;
		float32 rotacion_inicial;
		b2Vec2 posicion_inicial;
		bool hundido;
};

#endif
