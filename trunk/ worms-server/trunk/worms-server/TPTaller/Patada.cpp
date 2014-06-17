/*
 * Patada.cpp
 *
 *  Created on: 16/06/2014
 *      Author: mateo
 */

#include "Patada.h"


Patada::Patada(Personaje* personaje){
	this->fuerza = 0;
	this->danio = 10;
	this->tipo = 6;
	this->radio_explosion=3;
	personaje_duenio=personaje;
	this->proyectil = personaje->getBody();
}

Patada::~Patada() {
	// TODO Auto-generated destructor stub
}


class RaycastPatear : public b2RayCastCallback
{
public:
    b2Body* body;
    b2Vec2 pos;

    RaycastPatear() { body = NULL; }

    float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
    {
        body = fixture->GetBody();
        pos = point;
        return fraction;
    }
};

void Patada::aplicarExplosion(){
	b2World* world = this->proyectil->GetWorld();
	b2Vec2 pos = this->proyectil->GetPosition();
	float32 blastRadius = (float) radio_explosion;
	float angle = (float) angulo;
	b2Vec2 rayDir( sinf(angle), cosf(angle) );
	b2Vec2 rayEnd = pos + blastRadius * rayDir;

	RaycastPatear callback;
	world->RayCast(&callback, pos, rayEnd);
	if ( callback.body ){
	b2Body* body = callback.body;
	b2Vec2 posImpacto = callback.pos;
	if ( body == this->personaje_duenio->getBody() || body->GetType() != b2_dynamicBody ) return;
	b2Vec2 dir = posImpacto - pos;
	float32 distancia = dir.Normalize();
	if (distancia == 0 ) return;
	float32 invDistancia = 1/distancia;
	float32 impulso = this->danio * invDistancia*10;
	body->ApplyLinearImpulse(impulso * dir , posImpacto, true);
	//FALTA SACARLE VIDA A LOS GUSANOS

	}
}

bool Patada::checkImpacto(Mundo* mundo){
	return true;
}

int Patada::getTipo(){
	return this->tipo;
}
