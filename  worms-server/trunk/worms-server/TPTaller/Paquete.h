#ifndef PAQUETEDIBUJAR_H_
#define PAQUETEDIBUJAR_H_

#include "Personaje.h"
#include "figura.h"
#include "circulo.h"
#include "poligono.h"
#include "rectangulo.h"

typedef struct structFigura {
	b2Vec2 vector_vertices[6] ;
	int cantidad = 0;
	SDL_Color color;
}structFigura;


typedef struct structPersonaje {
	char nombre_cliente[20];
	int id_jugador;
	int seleccionado[4];
	int conectado;
	b2Vec2 posicion;
	b2Vec2 tamano;
	int direccion; // 0 derecha, 1 izquierda
}structPersonaje;

typedef struct structPaquete{
	int tipo = 0;
	structFigura vector_figuras[10];
	int cantidad_figuras = 0;
	structPersonaje vector_personajes[20];
	int cantidad_personajes = 0;
	int id;
	char mensaje_mostrar[MAX_MENSAJE];
	int turno_jugador;
}structPaquete;

typedef struct structInicial {
	bool cliente_aceptado = false;
	int tipo = 0;
	char tierra[50];
	char cielo[50];
	char agua[50];
	int ancho_ventana = 0;
	int alto_ventana = 0;
	float32 nivel_agua = 0.0;
	int ancho_unidades = 0;
	int alto_unidades = 0;
	float32 ancho_escenario = 0.0;
	float32 alto_escenario = 0.0;
}structInicial;

typedef struct structEvento{
	b2Vec2 click_mouse; // posicion escalada del mouse
	int direccion; // -1 izquierda, 0 arriba, 1 derecha //todo
	int nro_jugador = 0; //Identificador del cliente
	int aleatorio = 0;
}structEvento;


structFigura* crearPaqueteFigura(Figura* figura);
structPersonaje* crearPaquetePersonaje(Personaje* personaje);
structPaquete* crearPaqueteCiclo(Mundo* mundo, char* mensaje, int turno_jugador);
structEvento* crearPaqueteEvento(int* click, bool* KEYS, Escalador* escalador, int cliente);

void destruirPaqueteFigura(structFigura* paquete);
void destruirPaquetePersonaje(structPersonaje* paquete);
void destruirPaqueteCiclo(structPaquete* paquete);
bool estaVacio(structEvento* paquete);

#endif /* PAQUETEDIBUJAR_H_ */
