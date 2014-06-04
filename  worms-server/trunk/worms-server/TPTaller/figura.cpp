#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Box2D.h>

#include "figura.h"

Figura::Figura(SDL_Color color,	float32 rot){
	this->body = NULL;
	this->hundido = false;
	this->color = color;
	this->rotacion_inicial = rot;
	this->tipo = 0;
}

Figura::~Figura(){
	b2World* mundo = body->GetWorld();
	mundo->DestroyBody(body);
}

b2Vec2 Figura::obtenerPosicion(){
	return body->GetPosition();
}

b2Body* Figura::getBody(){
	return this->body;
}

SDL_Color Figura::getColor(){
	return this->color;
}

float32 Figura::getRot(){
	return this->rotacion_inicial;
}

void Figura::Hundir(){
	this->hundido = true;
}

void Figura::setPosicionInicial(){
	this->body->SetTransform(posicion_inicial,rotacion_inicial);
}
