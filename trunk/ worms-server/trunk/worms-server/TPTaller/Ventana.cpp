/*
 * Ventana.cpp
 *
 *  Created on: 28/04/2014
 *      Author: mateo
 */

#include "Ventana.h"


Ventana::Ventana(int x , int y , int escX , int escY) {
	this->ventanaX = x;
	this->ventanaY = y;
	this->escenarioX = escX;
	this->escenarioY = escY;
	this->offsetX = 0;
	this->offsetY = 0;
	this->zoom = 100; //100 es el zoom maximo

}

Ventana::~Ventana() {
	// TODO Auto-generated destructor stub
}

int Ventana::getOffsetX(){
	return offsetX;
}

int Ventana::getOffsetY(){
	return offsetY;
}

int Ventana::getZoom(){
	return zoom;
}

void Ventana::moverDerecha(){
	if (offsetX == 0) return;
	offsetX -=1;
}

void Ventana::moverIzquierda(){
	if (offsetX + ventanaX == escenarioX) return;
	offsetX -=1;
}

void Ventana::moverArriba(){
	if (offsetY == 0) return;
	offsetY -=1;
}

void Ventana::moverAbajo(){
	if (offsetX + ventanaX == escenarioX) return;
	offsetY +=1;
}

void Ventana::zoomAlejar(){
	if (zoom == 100) return;
	else zoom +=1;
}

void Ventana::zoomAcercar(){
	if (zoom == 0) return;
	else zoom -=1;
}
