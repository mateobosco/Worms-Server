/*
 * ManejadorPersonajes.cpp
 *
 *  Created on: 26/04/2014
 *      Author: juanmahidalgo
 */

#include "ManejadorPersonajes.h"
#include <Box2D/Box2D.h>
#include <SDL2/SDL.h>


#define RADIOMOUSE 1

ManejadorPersonajes::ManejadorPersonajes() {
	personajesMax = MAX_CANT_PERSONAJES;
	cantidad_actual_personajes = 0;
	cantidad_jugadores = 0;
	vector_personajes = new Personaje*[MAX_CANT_JUGADORES * personajesMax];
}

ManejadorPersonajes::~ManejadorPersonajes() {
//	for(int i = 0; i<cantidad_actual_personajes; i++){
//		Personaje* personaje = vector_personajes[i];
//		delete personaje;
//	}
	//Es responsabilidad de cada Jugador borrar sus personajes
	delete[] vector_personajes;
}

void ManejadorPersonajes::seleccionarPersonaje(b2Vec2 posicion, int id_jugador){

	b2CircleShape* shapeMouse = new b2CircleShape();
	shapeMouse->m_radius = RADIOMOUSE;
	b2Transform transformMouse = b2Transform(posicion, b2Rot(0) );
	for (int i = 0; i < personajesMax * cantidad_jugadores ; i++){
		Personaje* personaje = vector_personajes[i];
		b2Body* body_actual = personaje->getBody();
		b2Transform transformada_actual = body_actual->GetTransform();
		b2PolygonShape* shape_actual = personaje->getShape();

		bool resultado = b2TestOverlap(shape_actual,0, shapeMouse , 0,  transformada_actual, transformMouse);
		if (resultado){
			for (int j = 0; j < personajesMax * cantidad_jugadores ; j++){
				Personaje* personaje_aux = vector_personajes[j];
				personaje_aux->setSeleccionado(false, id_jugador);
			}
			personaje->setSeleccionado(resultado, id_jugador);
		}

	}
}

//TODO hay que borrarlo
void ManejadorPersonajes::AgregarJugador(Mundo* mundo, int id_jugador){
	for(int i=0 ; i <personajesMax; i++){
		vector_personajes[i+cantidad_actual_personajes] = new Personaje(mundo, id_jugador);
	}
	cantidad_actual_personajes += personajesMax;
	cantidad_jugadores+=1;
}

Personaje** ManejadorPersonajes::getPersonajes(){
	return vector_personajes;
}

int ManejadorPersonajes::getCantidadPersonajes(){
	return cantidad_actual_personajes;
}

//void ManejadorPersonajes::moverPersonaje(SDL_Event event, bool* KEYS,int id_jugador){
//	for (int j = 0; j < personajesMax * cantidad_jugadores; j++){
//		Personaje* personaje_actual = vector_personajes[j];
//		if (! personaje_actual->getMuerto()){
//			personaje_actual->leermovimiento(event, KEYS, id_jugador);
//		}
//	}
//}

void ManejadorPersonajes::moverPersonaje(int direccion,int id_jugador){
	for (int j = 0; j < personajesMax * cantidad_jugadores; j++){
		Personaje* personaje_actual = vector_personajes[j];
		if (! personaje_actual->getMuerto()){
			personaje_actual->leermovimiento(direccion, id_jugador);
		}
	}
}


void ManejadorPersonajes::agregarPersonaje(Personaje *personaje, uint8 numero_jugador){
	vector_personajes[cantidad_actual_personajes] = personaje;
	cantidad_actual_personajes++;
	cantidad_jugadores = numero_jugador;
}


int ManejadorPersonajes::getCantidadJugadores(){
	return cantidad_jugadores;
}


