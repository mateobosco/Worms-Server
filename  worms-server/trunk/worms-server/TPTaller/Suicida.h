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
};

#endif /* SUICIDA_H_ */
