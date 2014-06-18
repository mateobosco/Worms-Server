#ifndef GRANADAHOLY_H_
#define GRANADAHOLY_H_

#include "Arma.h"

class GranadaHoly: public Arma {
private:
	b2Vec2* definirImpulso(b2Vec2 destino);

public:
	GranadaHoly(Personaje* personaje);
	virtual ~GranadaHoly();
	void disparar(Mundo* mundo);
	bool checkImpacto(Mundo *mundo);
	bool setFuerza();
	void setAngulo(int un_angulo, int direc);
	void setDireccion(b2Vec2 una_direccion);
	void setPosicion(b2Vec2 una_posicion);
	void aplicarExplosion(ManejadorPersonajes *manejador);
	int getRadioExplosion();

	double getAngulo();
	int getTipo();
	b2Vec2 getDireccion();
	b2Vec2 getPosicion();
	b2Vec2 getTamanio();
	void resetFuerza();

	b2Body* getProyectil();
	int getContadorSegundos();
};
#endif /* GRANADAHOLY_H_ */
