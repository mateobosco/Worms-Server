#ifndef SUICIDA_H_
#define SUICIDA_H_

#include "Arma.h"

class Suicida : public Arma{
private:
	int tipo;

public:
	Suicida(Personaje *un_personaje);
	~Suicida();
	void disparar(Mundo* mundo);
	b2Vec2* definirImpulso(b2Vec2 destino);
	bool checkImpacto(Mundo *mundo);
	bool setFuerza();
	void setAngulo(int un_angulo, int direc);
	void setTipo(int tipo_arma);
	void setDireccion(b2Vec2 una_direccion);
	void setPosicion(b2Vec2 una_posicion);
	void aplicarExplosion();
	int getRadioExplosion();

	int getAngulo();
	int getTipo();
	b2Vec2 getDireccion();
	b2Vec2 getPosicion();
	b2Vec2 getTamanio();
	void resetFuerza();

	b2Body* getProyectil();
};

#endif /* SUICIDA_H_ */
