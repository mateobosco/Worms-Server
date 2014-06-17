#ifndef ARMA_H_
#define ARMA_H_

#include <cmath>
#include "Constantes.h"
#include "figura.h"
#include "ManejadorPersonajes.h"

typedef enum tipo_de_arma{
	ninguno, bazooka, etc
}type_arma;

class Arma{
	protected:
		char* dir_imagen;
		b2Vec2 posicion_proyectil;
		b2Vec2 direccion_proyectil;
		b2Vec2 tamanio_proyectil;
		b2Body* proyectil;
		b2CircleShape* shape_proy;
		int danio;
		float fuerza;
		int angulo;
		int radio_explosion;
		type_arma tipo;
		b2Vec2* definirImpulso(b2Vec2 destino);
		Personaje* personaje_duenio;
		void checkPersonajeLastimado(b2Body *body, ManejadorPersonajes *manejador);

	public:
		Arma();
		virtual ~Arma();
		virtual void disparar(Mundo* mundo);
		virtual bool checkImpacto(Mundo *mundo);
		virtual bool setFuerza();
		virtual void setAngulo(int un_angulo, int direc);
		virtual void setTipo(int tipo_arma);
		virtual void setDireccion(b2Vec2 una_direccion);
		virtual void setPosicion(b2Vec2 una_posicion);
		virtual void aplicarExplosion(ManejadorPersonajes *manejador);
		virtual int getRadioExplosion();

		virtual int getAngulo();
		virtual int getTipo();
		virtual b2Vec2 getDireccion();
		virtual b2Vec2 getPosicion();
		virtual b2Vec2 getTamanio();
		void resetFuerza();

		virtual b2Body* getProyectil();

};

#endif /* ARMA_H_ */
