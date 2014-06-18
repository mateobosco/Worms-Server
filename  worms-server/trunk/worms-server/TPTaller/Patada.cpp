/*
 * Patada.cpp
 *
 *  Created on: 16/06/2014
 *      Author: mateo
 */

#include "Patada.h"


Patada::Patada(Personaje* personaje){
	this->fuerza = 0;
	this->danio = DANIO_PATADA;
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

void Patada::aplicarExplosion(ManejadorPersonajes* manejador ){
	b2World* world = this->proyectil->GetWorld();
	b2Vec2 pos = this->proyectil->GetPosition();
	Personaje* pers = this->personaje_duenio;
	int orientacion = pers->getOrientacion();

	b2Vec2 rayo = b2Vec2(0,-1);
	if (orientacion == 1 )  rayo = b2Vec2(5,0);
	if (orientacion == -1)  rayo = b2Vec2(-5,0);
	b2Vec2 rayEnd = pos + rayo;
	RaycastPatear callback;
	world->RayCast(&callback, pos, rayEnd);
	if ( callback.body ){
	b2Body* body = callback.body;
	b2Vec2 posImpacto = callback.pos;
	if ( body == this->personaje_duenio->getBody() || body->GetType() != b2_dynamicBody ) return;
	b2Vec2 dir = posImpacto - pos;
	float32 distancia = dir.Normalize();
	if (distancia == 0 ) return;
    this->checkPersonajeLastimado(body, manejador, this->danio );
    if((abs(body->GetLinearVelocity().x) < 10) && (abs(body->GetLinearVelocity().y) < 10)){

    	if (orientacion == 1 ) body->ApplyLinearImpulse(b2Vec2(1.5 ,-0.5), posImpacto, true);
    	if (orientacion == -1) body->ApplyLinearImpulse(b2Vec2(-1.5 ,-0.5), posImpacto, true);
    }
	}
}

bool Patada::checkImpacto(Mundo* mundo){
	return true;
}

int Patada::getTipo(){
	return this->tipo;
}
