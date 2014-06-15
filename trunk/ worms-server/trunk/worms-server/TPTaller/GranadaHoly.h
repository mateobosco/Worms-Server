/*
 * GranadaHoly.h
 *
 *  Created on: 14/06/2014
 *      Author: juanmahidalgo
 */

#ifndef GRANADAHOLY_H_
#define GRANADAHOLY_H_

#include "Arma.h"


class GranadaHoly: public Arma {
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
	int tipo;
	b2Vec2* definirImpulso(b2Vec2 destino);
	Personaje* personaje_duenio;
	int radio_explosion;
public:
	GranadaHoly(Personaje* personaje);
	virtual ~GranadaHoly();
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

};
#endif /* GRANADAHOLY_H_ */
