#ifndef DIBUJABLE_H_
#define DIBUJABLE_H_

#include "Dibujador.h"

class Dibujable{
public:
	virtual int dibujar(Dibujador*) = 0;
	virtual ~Dibujable(){};
};

#endif /* DIBUJABLE_H_ */
