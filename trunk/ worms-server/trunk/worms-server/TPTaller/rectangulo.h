#ifndef RECTANGULO_H_
#define RECTANGULO_H_

#include <Box2D/Common/b2Math.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>

#include "mundo.h"

class Rectangulo: public Figura{
	public:
		Rectangulo(Mundo* mundo, b2Vec2 posicion, float32 ancho, float32 alto, bool estatico,SDL_Color color , float32 masa, float32 rotacion); //Uint8 color[3]
		~Rectangulo();
		int dibujar(Dibujador*);
		float32* getVecX();
		float32* getVecY();
//		float32 getRotInicial();
//		float32 getAngulo();
		b2Vec2* getVertices();

	private:
		float32 ancho;
		float32 alto;
		float32 anguloRec;
		float32 radio;
		b2PolygonShape* shape;
};

#endif /* RECTANGULO_H_ */
