

#include "Paquete.h"

structFigura* crearPaqueteFigura(Figura* figura){
	structFigura* paquete = new structFigura;
	int tipo = figura->tipo;
	paquete->color= figura->getColor();
	b2Vec2* vector1;

	if (tipo == 0){ //ES UN CIRCULO
		vector1 = new b2Vec2[2];
		Circulo* circulo = (Circulo*) figura;
		vector1[0] = circulo->obtenerPosicion();
		vector1[1].x = circulo->getRadio();
		vector1[1].y = circulo->getRadio();
		paquete->cantidad = 2;
	}
	if (tipo == 1){ //ES UN RECTANGULO
		Rectangulo* rectangulo = (Rectangulo*) figura;
		vector1 = rectangulo->getVertices();
		paquete->cantidad = 4;

	}
	if (tipo == 2){ //ES UN POLIGONO
		Poligono* poligono = (Poligono*) figura;
		int lados = poligono->getCantVertices();
		paquete->cantidad = lados;
		vector1 = poligono->getVertices();
	}
	paquete->vector_vertices = vector1;
	return paquete;
}

structPersonaje* crearPaquetePersonaje(Personaje* personaje){
	structPersonaje* paquete = new structPersonaje;
	b2Vec2 pos = personaje->getPosition();
	int dir;
	b2Body* body = personaje->getBody();
	if (body->GetLinearVelocity().x > 0 ){
		dir = 0;
	}
	else{
		dir = 1;
	}
	paquete->direccion = dir;
	paquete->posicion = pos;
	paquete->tamano.x = personaje->getAncho();
	paquete->tamano.y = personaje->getAlto();
	return paquete;
}


void destruirPaqueteFigura(structFigura* paquete){
	b2Vec2* vector1 = paquete->vector_vertices;
	delete[] vector1;
	delete paquete;
}

void destruirPaquetePersonaje(structPersonaje* paquete){
	delete paquete;
}

structEvento* crearPaqueteClick(int* click, Escalador* escalador, int cliente){
	structEvento* paquete = new structEvento;
	b2Vec2 posicion(click[0],click[1]);
	b2Vec2 posicionEscalada = escalador->escalarPosicion(posicion);
	paquete->click_mouse = posicionEscalada;
	paquete->direccion = -9; //DIRECION NO VALIDA
	paquete->nro_jugador = cliente;

	return paquete;
}

structEvento* crearPaqueteMovimiento(bool* KEYS, int id_jugador){
	structEvento* paquete = new structEvento;
	if (KEYS[100]){ // para la derecha
		paquete->direccion = 1;
	}
	if (KEYS[101]){ // para la izquierda
		paquete->direccion = -1;
	}
	if ((KEYS[102] || KEYS[SDLK_SPACE])){ // para arriba
		paquete->direccion = 0;
	}
	paquete->nro_jugador = id_jugador;
	paquete->click_mouse = b2Vec2( -1, -1 );
	return paquete;
}

structPaquete* crearPaqueteCiclo(Mundo* mundo){
	structPaquete* paquete = (structPaquete*) malloc(sizeof(structPaquete));

	Personaje** vector_personajes = mundo->getPersonajes();
	int cantidad_personajes = mundo->getCantidadPersonajes();
	structPersonaje** personajesEmpaquetados = (structPersonaje**) malloc(sizeof(structPersonaje*)*cantidad_personajes);
	for (int i=0 ; i<cantidad_personajes; i++){
		personajesEmpaquetados[i] = crearPaquetePersonaje(vector_personajes[i]);
	}
	Figura** vector_figuras = mundo->getFiguras();
	int cantidad_figuras = mundo->getCantidadFiguras();
	structFigura** figurasEmpaquetadas = (structFigura**) malloc(sizeof(structFigura*)*cantidad_figuras);
	for (int i=0 ; i<cantidad_figuras; i++){
		figurasEmpaquetadas[i] = crearPaqueteFigura(vector_figuras[i]);
	}
	paquete->cantidad_figuras = cantidad_figuras;
	paquete->cantidad_personajes = cantidad_personajes;
	paquete->vector_figuras = figurasEmpaquetadas;
	paquete->vector_personajes = personajesEmpaquetados;


	return paquete;
}

void destruirPaqueteCiclo(structPaquete* paquete){
	for (int i=0 ; i < paquete->cantidad_figuras; i++){
		destruirPaqueteFigura(paquete->vector_figuras[i]);
	}
	for (int i=0 ; i < paquete->cantidad_personajes; i++){
		destruirPaquetePersonaje(paquete->vector_personajes[i]);
	}
	free(paquete);
}
