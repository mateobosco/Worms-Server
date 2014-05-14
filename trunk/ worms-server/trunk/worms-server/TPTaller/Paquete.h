#ifndef PAQUETEDIBUJAR_H_
#define PAQUETEDIBUJAR_H_

#include "Personaje.h"
#include "figura.h"
#include "circulo.h"
#include "poligono.h"
#include "rectangulo.h"

typedef struct structFigura {
	b2Vec2* vector_vertices ;
	int cantidad;
	SDL_Color color;
}structFigura;


typedef struct structPersonaje {
	b2Vec2 posicion;
	b2Vec2 tamano;
	int direccion; // 0 derecha, 1 izquierda
}structPersonaje;

typedef struct structPaquete{
	structFigura** vector_figuras;
	int cantidad_figuras;
	structPersonaje** vector_personajes;
	int cantidad_personajes;
}structPaquete;

typedef struct structInicial {
	string tierra;
	string cielo;
	string agua;
	int ancho_ventana;
	int alto_ventana;
	float32 nivel_agua;
	int ancho_unidades;
	int alto_unidades;
	float32 ancho_escenario;
	float32 alto_escenario;
}structInicial;

typedef struct structEvento{
	b2Vec2 click_mouse; // posicion escalada del mouse
	int direccion; // -1 izquierda, 0 arriba, 1 derecha
	int nro_jugador; //Identificador del cliente
}structEventos;


structFigura* crearPaqueteFigura(Figura* figura);
structPersonaje* crearPaquetePersonaje(Personaje* personaje);
structPaquete* crearPaqueteCiclo(Mundo* mundo);

void destruirPaqueteFigura(structFigura* paquete);
void destruirPaquetePersonaje(structPersonaje* paquete);
void destruirPaqueteCiclo(structPaquete* paquete);


#endif /* PAQUETEDIBUJAR_H_ */
