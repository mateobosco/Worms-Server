#ifndef ESCALADOR_H_
#define ESCALADOR_H_

#include <Box2D/Box2D.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <fstream>

using namespace std;

class Escalador {
	public:
		Escalador(int venanaX, int ventanaY, float32 escalaX, float32 escalaY, int ancho_esc, int alto_esc);
		virtual ~Escalador();
		b2Vec2 escalarPosicion(b2Vec2 posicionPixel);
		b2Vec2* pixelarPosicion(b2Vec2 posicionEscalar);
		Sint16* pixelarVectorX(float32* vec, int cantidad);
		Sint16* pixelarVectorY(float32* vec, int cantidad);
		Sint16* ventanarVectorX(float32* vec, int cantidad);
		Sint16* ventanarVectorY(float32* vec, int cantidad);
		b2Vec2* ventanarPosicion(b2Vec2 posicionEscalar);
		b2Vec2* aplicarZoomPosicion(b2Vec2 posicionEscalar);
		b2Vec2* aplicarZoomPixel(b2Vec2 posicionEscalar);
		Sint16* aplicarZoomXVector(float32* vec, int cantidad);
		Sint16* aplicarZoomYVector(float32* vec, int cantidad);
		void hacerZoom(int* posicion_mouse_scroll);

		void moverVentana(int* posicion_mouse);
		int getZoom();

		int ventanarEnX(float32 x);
		int ventanarEnY(float32 y);
		int getPixelX();
		int getPixelY();
		int getOffsetX();
		int getOffsetY();
		int getVentanaX();
		int getVentanaY();
		float32 getEscalaX();
		float32 getEscalaY();
		int aplicarZoomX(float32 valor);
		int aplicarZoomY(float32 valor);
		int aplicarZoomXaEscala(float32 valor);
		int aplicarZoomYaEscala(float32 valor);
		int aplicarZoomXaPix(int valor);
		int aplicarZoomYaPix(int valor);
		void moverDerecha(int n);
		void moverIzquierda(int n);
		void moverArriba(int n);
		void moverAbajo(int n);
		int zoomAlejar();
		int zoomAcercar();
		int centroX;
		int centroY;

	private:
		int pixelesX;
		int pixelesY;
		int ventanaX;
		int ventanaY;
		int offsetX;
		int offsetY;

		int zoom;
		float32 escalaX;
		float32 escalaY;
};

#endif /* ESCALADOR_H_ */
