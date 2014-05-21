#ifndef PAQUETEDIBUJAR_H_
#define PAQUETEDIBUJAR_H_

#include "Personaje.h"
#include "figura.h"
#include "circulo.h"
#include "poligono.h"
#include "rectangulo.h"

typedef struct structFigura {
	b2Vec2 vector_vertices[6] ;
	int cantidad;
	SDL_Color color;
}structFigura;


typedef struct structPersonaje {
	int id_jugador;
	int seleccionado;
	int conectado;
	b2Vec2 posicion;
	b2Vec2 tamano;
	int direccion; // 0 derecha, 1 izquierda
}structPersonaje;

typedef struct structPaquete{
	int tipo;
	structFigura vector_figuras[10];
	int cantidad_figuras;
	structPersonaje vector_personajes[20];
	int cantidad_personajes;
}structPaquete;

typedef struct structInicial {
	bool cliente_aceptado;
	int tipo;
	char tierra[50];
	char cielo[50];
	char agua[50];
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
	int aleatorio;
}structEvento;


structFigura* crearPaqueteFigura(Figura* figura);
structPersonaje* crearPaquetePersonaje(Personaje* personaje);
structPaquete* crearPaqueteCiclo(Mundo* mundo);
structEvento* crearPaqueteEvento(int* click, bool* KEYS, Escalador* escalador, int cliente);

void destruirPaqueteFigura(structFigura* paquete);
void destruirPaquetePersonaje(structPersonaje* paquete);
void destruirPaqueteCiclo(structPaquete* paquete);


#endif /* PAQUETEDIBUJAR_H_ */
