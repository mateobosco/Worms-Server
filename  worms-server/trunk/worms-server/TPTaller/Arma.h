#ifndef ARMA_H_
#define ARMA_H_

#include <cmath>
#include "Constantes.h"
#include "figura.h"
#include "Personaje.h"

typedef enum tipo_de_arma{
	ninguno, bazooka, etc
}type_arma;

class Arma{
	private:
		char* dir_imagen;
		b2Vec2 posicion_proyectil;
		b2Vec2 direccion_proyectil;
		b2Vec2 tamanio_proyectil;
		b2Body* proyectil;
		b2CircleShape* shape_proy;
		int danio;
		float fuerza;
		int angulo;
		type_arma tipo;
		b2Vec2* definirImpulso(b2Vec2 destino);

	public:
		Arma();
		virtual ~Arma();
		void disparar(Mundo* mundo);
		bool checkImpacto(Mundo *mundo);
		bool setFuerza();
		void setAngulo(int un_angulo);
		void setTipo(int tipo_arma);
		void setDireccion(b2Vec2 una_direccion);
		void setPosicion(b2Vec2 una_posicion);

		int getAngulo();
		int getTipo();
		b2Vec2 getDireccion();
		b2Vec2 getPosicion();
		b2Vec2 getTamanio();
		void resetFuerza();

		b2Body* getProyectil();

};

#endif /* ARMA_H_ */
