#include "ManejadorPersonajes.h"
#include <Box2D/Box2D.h>
#include <SDL2/SDL.h>

#define RADIOMOUSE 0.25

ManejadorPersonajes::ManejadorPersonajes() {
	this->personajesMax = MAX_CANT_PERSONAJES;
	this->cantidad_actual_personajes = 0;
	this->cantidad_jugadores = 0;
	this->vector_personajes = new Personaje*[MAX_CANT_JUGADORES * this->personajesMax];
	for(int i = 0; i < MAX_CANT_JUGADORES * this->personajesMax ;i++ ){
		this->vector_personajes[i] = NULL;
	}
}

ManejadorPersonajes::~ManejadorPersonajes() {
	//Es responsabilidad de cada Jugador borrar sus personajes
	if(this->vector_personajes) delete[] this->vector_personajes; this->vector_personajes = NULL;
}

void ManejadorPersonajes::seleccionarPersonaje(b2Vec2 posicion, int id_jugador){
	b2CircleShape* shapeMouse = new b2CircleShape();
	shapeMouse->m_radius = RADIOMOUSE;
	b2Transform transformMouse = b2Transform(posicion, b2Rot(0) );

	for (int i = 0; i < this->cantidad_actual_personajes ; i++){
		Personaje* personaje = this->vector_personajes[i];
		b2Body* body_actual = personaje->getBody();
		b2Transform transformada_actual = body_actual->GetTransform();
		b2CircleShape* shape_actual = personaje->getShape();
		bool resultado = b2TestOverlap(shape_actual,0, shapeMouse , 0,  transformada_actual, transformMouse);
		if (resultado){
			for (int j = 0; j < this->personajesMax * this->cantidad_jugadores ; j++){
				Personaje* personaje_aux = this->vector_personajes[j];
				personaje_aux->setSeleccionado(false, id_jugador);
				personaje_aux->setArmaSeleccionada(0);
			}
			personaje->setSeleccionado(resultado, id_jugador);
			int dueno = personaje->getNrojugador();
		}
	}
	if(shapeMouse) delete shapeMouse; shapeMouse = NULL;
}

void ManejadorPersonajes::AgregarJugador(Mundo* mundo, int id_jugador, Personaje** vectorPersonajes){
	for(int i=0 ; i <this->personajesMax; i++){
		this->vector_personajes[i+this->cantidad_actual_personajes] = vectorPersonajes[i];
	}
	this->cantidad_jugadores+=1;
}

Personaje** ManejadorPersonajes::getPersonajes(){
	return this->vector_personajes;
}

int ManejadorPersonajes::getCantidadPersonajes(){
	return this->cantidad_actual_personajes;
}

void ManejadorPersonajes::moverPersonaje(int direccion,int id_jugador){
	for (int j = 0; j < this->cantidad_actual_personajes; j++){
		Personaje* personaje_actual = this->vector_personajes[j];
		if (! personaje_actual->getMuerto()){
			personaje_actual->leermovimiento(direccion, id_jugador);
		}
	}
}

void ManejadorPersonajes::agregarPersonaje(Personaje *personaje, uint8 numero_jugador){
	this->vector_personajes[this->cantidad_actual_personajes] = personaje;
	this->cantidad_actual_personajes++;
}

int ManejadorPersonajes::getCantidadJugadores(){
	return this->cantidad_jugadores;
}

void ManejadorPersonajes::resetManejador(){
	for(int i = 0; i < MAX_CANT_JUGADORES * this->personajesMax ;i++ ){
		this->vector_personajes[i] = NULL;
	}
	this->cantidad_jugadores = 0;
	this->cantidad_actual_personajes = 0;
}

void ManejadorPersonajes::quitarVidaPersonaje(Personaje *personaje, int danio){
	if(!personaje->getDaniadoTurnoActual()){
		personaje->setDaniadoTurnoActual(true);
		personaje->quitarEnergia(danio);
	}
}

void ManejadorPersonajes::resetDaniadoTurnoActual(){
	for(int i = 0; i < this->cantidad_actual_personajes; i++){
		this->vector_personajes[i]->setDaniadoTurnoActual(false);
	}
}
