#include "Paquete.h"

structFigura* crearPaqueteFigura(Figura* figura){
	structFigura* paquete = new structFigura;
	int tipo = figura->tipo;
	paquete->color= figura->getColor();

	if (tipo == 0){ //ES UN CIRCULO
		Circulo* circulo = (Circulo*) figura;
		paquete->vector_vertices[0] = circulo->obtenerPosicion();
		paquete->vector_vertices[1].x = circulo->getRadio();
		paquete->vector_vertices[1].y = circulo->getRadio();
		paquete->cantidad = 2;
	}
	if (tipo == 1){ //ES UN RECTANGULO
		Rectangulo* rectangulo = (Rectangulo*) figura;
		b2Vec2* vertices = rectangulo->getVertices();
		for (int i = 0 ; i<4; i++){
			paquete->vector_vertices[i].x = vertices[i].x;
			paquete->vector_vertices[i].y = vertices[i].y;
		}
		delete[] vertices;
		paquete->cantidad = 4;

	}
	if (tipo == 2){ //ES UN POLIGONO
		Poligono* poligono = (Poligono*) figura;
		int lados = poligono->getCantVertices();
		paquete->cantidad = lados;
		b2Vec2* vertices = poligono->getVertices();
		for (int i = 0 ; i<lados; i++){
			paquete->vector_vertices[i].x = vertices[i].x;
			paquete->vector_vertices[i].y = vertices[i].y;
		}
		delete[] vertices;
	}
	return paquete;
}

structPersonaje* crearPaquetePersonaje(Personaje* personaje){
	if (personaje == NULL) return NULL;
	structPersonaje* paquete = new structPersonaje;
	b2Vec2 pos = personaje->getPosition();
	int dir;
	b2Body* body = personaje->getBody();
	if (body->GetLinearVelocity().x > 0.5 ){
		dir = 1;
	}
	else{
		dir = -1;
	}
	paquete->direccion = personaje->getOrientacion();
	paquete->posicion = pos;
	paquete->tamano.x = personaje->getAncho();
	paquete->tamano.y = personaje->getAlto();
	paquete->conectado = personaje->getConectado();
	paquete->id_jugador = personaje->getNrojugador();
	strcpy(paquete->nombre_cliente, personaje->getNombreCliente());
	paquete->energia = personaje->getEnergia();
	//paquete->nombre_cliente = personaje->getNombreCliente();
	paquete->arma_seleccionada = personaje->getArmaSeleccionada();
	paquete->angulo_arma = personaje->getAnguloArma();


	bool* seleccion = personaje->getSeleccion();
	for (int i = 0 ; i<4; i++){
		if (seleccion[i]){
			paquete->seleccionado[i]=1;
		}
		if (!seleccion[i]){
			paquete->seleccionado[i]=0;
		}
	}
	return paquete;
}


void destruirPaqueteFigura(structFigura* paquete){
	delete paquete;
}

void destruirPaquetePersonaje(structPersonaje* paquete){
	delete paquete;
}

structEvento* crearPaqueteClick(int* click, Escalador* escalador, int cliente){
	structEvento* paquete = new structEvento; //todo
	b2Vec2 posicion(click[0],click[1]);
	b2Vec2 posicionEscalada = escalador->escalarPosicion(posicion);
	paquete->click_mouse = posicionEscalada;
	paquete->direccion = -9; //DIRECION NO VALIDA
	paquete->nro_jugador = cliente;
	paquete->aleatorio = random();
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
	paquete->aleatorio = random();
	return paquete;
}

structEvento* crearPaqueteVacio(){
	structEvento* paquete = new structEvento;
	paquete->click_mouse = b2Vec2 (-1,-1);
	paquete->direccion = -9;
	paquete->nro_jugador = MAX_CANT_JUGADORES;
	paquete->aleatorio = random();
	return paquete;
}

structEvento* crearPaqueteEvento(int* click, bool* KEYS, Escalador* escalador, int cliente){
	structEvento* paquete;
	if ( KEYS[100] || KEYS[101] || KEYS[102] || KEYS[SDLK_SPACE]){ // no es un click, es un movimiento
		paquete = crearPaqueteMovimiento(KEYS, cliente);

	}
	else if (click[0] != -1){
		paquete = crearPaqueteClick(click, escalador, cliente);
	}
	else {
		paquete = crearPaqueteVacio();
	}
	return paquete;
}

structPaquete* crearPaqueteCiclo(Mundo* mundo, char* mensaje, int jugador_actual, int empezar_juego, int tiempo_ronda, char* nombre, char* winner, int cant_winners, bool boolReset){
	structPaquete* paquete = (structPaquete*) malloc(MAX_PACK);
	paquete->tipo = 1;
	paquete->ganador[0]='\0';
	Personaje** vector_personajes = mundo->getPersonajes();
	int cantidad_personajes = mundo->getCantidadPersonajes();
	for (int i=0 ; i<cantidad_personajes; i++){
		structPersonaje* paquetito = crearPaquetePersonaje(vector_personajes[i]);
		memcpy(&paquete->vector_personajes[i], paquetito, sizeof(structPersonaje));
		destruirPaquetePersonaje(paquetito);
	}
	Figura** vector_figuras = mundo->getFiguras();
	int cantidad_figuras = mundo->getCantidadFiguras();
	for (int i=0 ; i<cantidad_figuras; i++){
		structFigura* paquetito = crearPaqueteFigura(vector_figuras[i]);
		memcpy(&paquete->vector_figuras[i],paquetito,sizeof(structFigura));
		destruirPaqueteFigura(paquetito);
	}
	paquete->cantidad_figuras = cantidad_figuras;
	paquete->cantidad_personajes = cantidad_personajes;
	paquete->turno_jugador = jugador_actual;
	paquete->comenzar = empezar_juego; // 1 si el juego debe comenzar, 0  si se debe esperar a mas jugadores a que se conecten
	paquete->reloj = tiempo_ronda;
	paquete->resetear = boolReset;


	strcpy(paquete->nombre_jugador_actual, nombre);
	if (mensaje != NULL){
		strcpy(paquete->mensaje_mostrar, mensaje);
	}
	else {
		memset(paquete->mensaje_mostrar, 0, MAX_MENSAJE);
	}
	paquete->radio_explosion=-1;
	if(winner[0] != '\0')
		strcpy(paquete->ganador, winner);
	paquete->cant_ganadores = cant_winners;
	if (cant_winners == 1) paquete->resultado = 1;
	if (cant_winners > 1) paquete->resultado = 0;
	if (cant_winners <= 0) paquete->resultado = -1;
	return paquete;
}

void destruirPaqueteCiclo(structPaquete* paquete){
	free(paquete);
}

bool estaVacio(structEvento* paquete){
	return (paquete->nro_jugador == MAX_CANT_JUGADORES);
}
