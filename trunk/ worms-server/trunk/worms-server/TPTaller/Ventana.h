/*
 * Ventana.h
 *
 *  Created on: 28/04/2014
 *      Author: mateo
 */

#ifndef VENTANA_H_
#define VENTANA_H_


class Ventana {
public:
	Ventana(int sizeX , int sizeY, int escenarioX, int escenarioY);
	virtual ~Ventana();
	int getOffsetX();
	int getOffsetY();
	int getZoom();
	void moverDerecha();
	void moverIzquierda();
	void moverArriba();
	void moverAbajo();
	void zoomAlejar();
	void zoomAcercar();


private:
	int ventanaX;
	int ventanaY;
	int escenarioX;
	int escenarioY;
	int zoom;
	int offsetX;
	int offsetY;
};
#endif /* VENTANA_H_ */
