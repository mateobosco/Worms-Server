#include "Escalador.h"

#define BORDE1 10
#define BORDE2 1


Escalador::Escalador(int ventanaX, int ventanaY, float32 escalaX, float32 escalaY, int ancho_esc, int alto_esc) {
	this->pixelesX = ancho_esc;
	this->pixelesY = alto_esc;
	this->centroX = pixelesX/2;
	this->centroY = pixelesY/2;
	this->ventanaX = ventanaX;
	this->ventanaY = ventanaY;
	this->escalaX = escalaX;
	this->escalaY = escalaY;
	this->offsetX = centroX - ventanaX/2;
	this->offsetY = centroY - ventanaY/2;
	this->zoom = 100;
}

Escalador::~Escalador() {

}

int Escalador::getPixelX(){
	return this->pixelesX;
}
int Escalador::getPixelY(){
	return this->pixelesY;
}
int Escalador::getVentanaX(){
	return this->ventanaX;
}
int Escalador::getVentanaY(){
	return this->ventanaY;
}
int Escalador::getOffsetX(){
	return this->offsetX;
}
int Escalador::getOffsetY(){
	return this->offsetY;
}

float32 Escalador::getEscalaX(){
	return this->escalaX;
}
float32 Escalador::getEscalaY(){
	return this->escalaY;
}

b2Vec2 Escalador::escalarPosicion(b2Vec2 posicionPixel){

	float32 escaladoX = ((posicionPixel.x+offsetX -centroX) *((float)100/zoom) + centroX) * ((float)escalaX/pixelesX);
	float32 escaladoY = ((posicionPixel.y+offsetY -centroY) *((float)100/zoom) + centroY) * ((float)escalaY/pixelesY);
	b2Vec2 escalado = b2Vec2(escaladoX,escaladoY);
	return escalado;
}

b2Vec2* Escalador::pixelarPosicion(b2Vec2 posicionEscalar){
	int pixelX = posicionEscalar.x / (this->escalaX/this->pixelesX);
	int pixelY = posicionEscalar.y / (this->escalaY/this->pixelesY);
	b2Vec2* pixelado = new b2Vec2(pixelX,pixelY); //ver delete todo
	return pixelado;
}

b2Vec2* Escalador::ventanarPosicion(b2Vec2 posicionEscalar){
	int ventanadoX = ventanarEnX(posicionEscalar.x);
	int ventanadoY = ventanarEnY(posicionEscalar.y);
	b2Vec2* pixelado = new b2Vec2(ventanadoX,ventanadoY); //ver delete todo
	return pixelado;
}

Sint16* Escalador::pixelarVectorX(float32* vec, int cantidad){
	int i;
	Sint16* pixelado = new Sint16[cantidad]; //ver delete todo
	for (i = 0 ; i < cantidad ; i++){
		float32 numero = vec[i];
		pixelado[i] = numero * (this->pixelesX / this->escalaX);
	}
	return pixelado;
}

Sint16* Escalador::pixelarVectorY(float32* vec, int cantidad){
	int i;
	Sint16* pixelado = new Sint16[cantidad]; //ver delete todo
	for (i = 0 ; i < cantidad ; i++){
		float32 numero = vec[i];
		pixelado[i] =  numero * (this->pixelesY / this->escalaY);
	}
	return pixelado;
}


int Escalador::ventanarEnX(float32 x){
	int valor = (int) (x *(pixelesX/escalaX));
	valor = valor - offsetX;
	//valor = valor+zoom;
	return valor;
}

int Escalador::ventanarEnY(float32 y){
	int valor = (int) (y *(pixelesY/escalaY));
	valor = valor - offsetY;
	//valor = valor+zoom;
	return valor;
}

Sint16* Escalador::ventanarVectorX(float32* vec, int cantidad){
	int i;
	Sint16* ventanado = new Sint16[cantidad]; //ver delete todo
	for (i = 0 ; i < cantidad ; i++){
		float32 numero = vec[i];
		ventanado[i] = ventanarEnX(numero);
	}
	return ventanado;
}

Sint16* Escalador::ventanarVectorY(float32* vec, int cantidad){
	int i;
	Sint16* ventanado = new Sint16[cantidad]; //ver delete todo
	for (i = 0 ; i < cantidad ; i++){
		float32 numero = vec[i];
		ventanado[i] = ventanarEnY(numero);
	}
	return ventanado;
}

Sint16* Escalador::aplicarZoomXVector(float32* vec, int cantidad){
	int i;
	Sint16* ventanado = new Sint16[cantidad]; //ver delete todo
	for (i = 0 ; i < cantidad ; i++){
		float32 numero = vec[i];
		ventanado[i] = aplicarZoomXaEscala(numero);
	}
	return ventanado;
}

Sint16* Escalador::aplicarZoomYVector(float32* vec, int cantidad){
	int i;
	Sint16* ventanado = new Sint16[cantidad]; //ver delete todo
	for (i = 0 ; i < cantidad ; i++){
		float32 numero = vec[i];
		ventanado[i] = aplicarZoomYaEscala(numero);
	}
	return ventanado;
}


int Escalador::aplicarZoomX(float32 valor){
	int n = (int) (valor *(pixelesX/escalaX))*((float32)zoom/100);
	return n;
}

int Escalador::aplicarZoomY(float32 valor){
	int n = (int) (valor *(pixelesY/escalaY))*((float32)zoom/100);
	return n;
}

int Escalador::aplicarZoomXaEscala(float32 valor){
	int n = (int) ((valor *(pixelesX/escalaX))-centroX)*((float32)zoom/100) +centroX;
	n = n - offsetX;
	return n;
}

int Escalador::aplicarZoomYaEscala(float32 valor){
	int n = (int) ((valor *(pixelesY/escalaY))-centroY)*((float32)zoom/100) +centroY;
	n = n - offsetY;
	return n;
}

int Escalador::aplicarZoomXaPix(int valor){
	int n = (int) (valor-centroX)*((float32)zoom/100) +centroX;
	n = n - offsetX;
	return n;
}

int Escalador::aplicarZoomYaPix(int valor){
	int n = (int) (valor-centroY)*((float32)zoom/100) +centroY;
	n = n - offsetY;
	return n;
}

b2Vec2* Escalador::aplicarZoomPixel(b2Vec2 posicionEscalar){
	int ventanadoX = aplicarZoomXaPix(posicionEscalar.x);
	int ventanadoY = aplicarZoomYaPix(posicionEscalar.y);
	b2Vec2* pixelado = new b2Vec2(ventanadoX,ventanadoY);
	return pixelado;
}

b2Vec2* Escalador::aplicarZoomPosicion(b2Vec2 posicionEscalar){
	int ventanadoX = aplicarZoomXaEscala(posicionEscalar.x);
	int ventanadoY = aplicarZoomYaEscala(posicionEscalar.y);
	b2Vec2* pixelado = new b2Vec2(ventanadoX,ventanadoY); //ver delete todo
	return pixelado;
}

void Escalador::moverDerecha(int n){
	if (((((offsetX + ventanaX-centroX)*(float)100/zoom))+centroX) >= pixelesX-5) return;
	offsetX +=2;
}

void Escalador::moverIzquierda(int n){
	if (((((offsetX+centroX)*(float)zoom/100))-centroX) <= 5) return;
	offsetX -=2;
}

void Escalador::moverArriba(int n){
	if (((((offsetY+centroY)*(float)zoom/100))-centroY) <= 5) return;
	offsetY -=2;
}

void Escalador::moverAbajo(int n){
	if (((((offsetY + ventanaY-centroY)*(float)100/zoom))+centroY) >= pixelesY-5) return;
	offsetY +=2;
}

int Escalador::zoomAlejar(){
	int zom = zoom - 3;
	if (zom <= 10) return -5;

	if (((((offsetX + centroX) * (float)zom/100)) - centroX) <= 5){
		return -1;
	}
	if (((((offsetY + centroY) * (float)zom/100)) - centroY) <= 5){
		return -4;
	}
	if (((((offsetX + ventanaX - centroX) * (float)100/zom)) + centroX) >= pixelesX-5){
		return -3;
	}
	if (((((offsetY + ventanaY - centroY) * (float)100/zom)) + centroY) >= pixelesY-5){
		return -2;
	}


	zoom -=3;
	return 0;
}

int Escalador::zoomAcercar(){
	if (zoom >= 1000) return false;
	int zom = zoom+3;

	if (((((offsetX + centroX) * (float)zom/100)) - centroX) <= 5){
		return -1;
	}
	if (((((offsetY + centroY) * (float)zom/100)) - centroY) <= 5){
		return -4;
	}
	if (((((offsetX + ventanaX - centroX) * (float)100/zom)) + centroX) >= pixelesX-5){
		return -3;
	}
	if (((((offsetY + ventanaY - centroY) * (float)100/zom)) + centroY) >= pixelesY-5){
		return -2;
	}


	this->zoom +=3;
	return true;
}


int Escalador::getZoom(){
	return zoom;
}

void Escalador::hacerZoom(int* posicion_mouse_scroll){


	if (posicion_mouse_scroll[2] == 1){
		centroX = posicion_mouse_scroll[0];
		centroY = posicion_mouse_scroll[1];
		this->zoomAcercar();
	}
	if (posicion_mouse_scroll[2] == -1){
		//centroX = ventanaX - posicion_mouse_scroll[0];
		//centroY = ventanaY - posicion_mouse_scroll[1];
		centroX = ((pixelesX/2 ) - ventanaX/2) * ((float) zoom/100) + ventanaX/2 - offsetX;
		centroY = ((pixelesY/2 ) - ventanaY/2) * ((float) zoom/100) + ventanaY/2 - offsetY;

		int resultado = this->zoomAlejar();
		if (resultado == 0) return;
	}
}
void Escalador::moverVentana(int* posicion_mouse){
	if (posicion_mouse[0] > (ventanaX - BORDE1) ){
		this->moverDerecha(1);
	}
	if (posicion_mouse[1] > (ventanaY - BORDE1) ){
		this->moverAbajo(1);
	}
	if (posicion_mouse[0] <  BORDE1 && posicion_mouse[0]>=0){
		this->moverIzquierda(1);
	}
	if (posicion_mouse[1] <  BORDE1 && posicion_mouse[1]>=0 ){
		this->moverArriba(1);
	}
	if (posicion_mouse[0] > (ventanaX - BORDE2) ){
		this->moverDerecha(2);
	}
	if (posicion_mouse[1] > (ventanaY - BORDE2) ){
		this->moverAbajo(2);
	}
	if (posicion_mouse[0] <  BORDE2 && posicion_mouse[0]>=0){
		this->moverIzquierda(2);
	}
	if (posicion_mouse[1] <  BORDE2 && posicion_mouse[1]>=0 ){
		this->moverArriba(2);
	}

/*	int ceroX = (int) ((((offsetX+centroX)*(float)zoom/100))-centroX);
	int ceroY = (int)((((offsetY+centroY)*(float)zoom/100))-centroY);
	int limiteX =(int) (((((offsetX + ventanaX-centroX)*(float)100/zoom))+centroX));
	int limiteY =(int) (((((offsetY + ventanaY-centroY)*(float)100/zoom))+centroY));

	printf(" (%d , %d) (%d , %d) \n", ceroX,ceroY,ceroX,limiteY);
	printf(" (%d , %d) (%d , %d) \n", ceroX,limiteY,limiteX,limiteY);
	printf( "Centros %d , %d \n", centroX, centroY);
	printf("------------------- \n");*/
}
