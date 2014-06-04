#ifndef POLIGONO_H
#define POLIGONO_H

#include <cmath>
#include <Box2D/Common/b2Math.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>

#include "mundo.h"

class Poligono : public Figura {
	public:
		Poligono(Mundo* mundo, b2Vec2 posicion, float32 escala, int lados, bool estatico, SDL_Color color , int masa, float32 rotacion);
		~Poligono();
		int getCantVertices();
		float32* getVecX();
		float32* getVecY();
		int dibujar(Dibujador* un_dibujador);
		b2Vec2* getVertices();

	private:
		float32 radio; // longitud desde el centro al vertice (radio)
		b2PolygonShape* shapePoligono;
		void initVertices(b2Vec2* vertices, int cantidad);
};

#endif
