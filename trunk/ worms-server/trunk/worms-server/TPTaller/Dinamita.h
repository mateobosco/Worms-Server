#ifndef DINAMITA_H_
#define DINAMITA_H_

#include "Arma.h"
#include <ctime>

class Dinamita: public Arma {
private:
	int reloj_comienzo;

public:
	Dinamita(Personaje *personaje);
	virtual ~Dinamita();
	void disparar(Mundo* mundo);
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
	int getContadorSegundos();
};

#endif /* DINAMITA_H_ */
