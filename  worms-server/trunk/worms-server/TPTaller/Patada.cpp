/*
 * Patada.cpp
 *
 *  Created on: 16/06/2014
 *      Author: mateo
 */

#include "Patada.h"


Patada::Patada(Personaje* personaje){
	this->fuerza = 0;
	this->tipo = 6;
	this->radio_explosion=3;
	personaje_duenio=personaje;
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

void Patada::disparar(Mundo* mundo){
	b2World* world = this->proyectil->GetWorld();
	b2Vec2 pos = this->proyectil->GetPosition();
	//printf("UBICO LA BOMBA EN (%f,%f) \n",pos.x,pos.y);
	float32 blastRadius = (float) radio_explosion;
	float angle = (float) angulo;
	b2Vec2 rayDir( sinf(angle), cosf(angle) );
	b2Vec2 rayEnd = pos + blastRadius * rayDir;
	//printf("ANGULOS %f \n",angle);

	RaycastPatear callback;
	//printf("HAGO UN RAYCAST ENTRE LAS POSICIONES (%f,%f) y (%f,%f) \n",pos.x,pos.y,rayEnd.x,rayEnd.y);
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
	//		impulso = b2Min(impulso, 500.0f); // estaba en el tutorial, no estoy seguro
	body->ApplyLinearImpulse(impulso * dir , posImpacto, true);
	//printf("EL RAYCAST ENCONTRO UN CUERPO Y LE APLICA UN LINEAR IMPULSE DE %f \n",impulso);
	//FALTA SACARLE VIDA A LOS GUSANOS

	}
}


