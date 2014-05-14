#ifndef CIRCULO_H
#define CIRCULO_H

#include <Box2D/Collision/Shapes/b2CircleShape.h>

#include "mundo.h"

class Circulo: public Figura {
	public:

		Circulo(Mundo* mundo, b2Vec2 posicion, float32 escala, bool estatico, SDL_Color color, float32 masa, float32 rot);

		~Circulo();

		float getRadio();

		int dibujar(Dibujador* el_dibujador);

	private:
		float32 radio;
		b2CircleShape shape;
};

#endif

