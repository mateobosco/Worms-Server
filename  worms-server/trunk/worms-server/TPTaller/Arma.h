#ifndef ARMA_H_
#define ARMA_H_

#include "Constantes.h"
#include "figura.h"
#include "Personaje.h"

class Arma{
	private:
		char* dir_imagen;
		b2Body* body;
		b2PolygonShape* shape;
		int danio;
		float fuerza;
		int angulo;

		b2Vec2* definirImpulso(b2Vec2 destino);

	public:
		Arma();
		virtual ~Arma();
		void disparar();
		int checkImpacto(Mundo *mundo);
		bool setFuerza();
		void setAngulo(int un_angulo);
};

#endif /* ARMA_H_ */
