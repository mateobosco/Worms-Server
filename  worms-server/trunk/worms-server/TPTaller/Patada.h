/*
 * Patada.h
 *
 *  Created on: 16/06/2014
 *      Author: mateo
 */

#ifndef PATADA_H_
#define PATADA_H_

#include "Arma.h"


class Patada: public Arma {
public:
	Patada(Personaje* personaje);
	virtual ~Patada();
	void aplicarExplosion(ManejadorPersonajes* manejador);
	bool checkImpacto(Mundo* mundo);
	int getTipo();

private:
	int tipo;

};


#endif /* PATADA_H_ */
