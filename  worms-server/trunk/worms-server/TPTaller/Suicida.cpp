#include "Suicida.h"

Suicida::Suicida(Personaje *un_personaje) {
	this->dir_imagen = NULL;
	this->danio = DANIO_SUICIDA;
	this->fuerza = 0;
	this->tipo = 5;
	this->personaje_duenio = un_personaje;
	this->radio_explosion = 3;
}

Suicida::~Suicida() {

}

void Suicida::disparar(Mundo* mundo){
	b2Vec2 escalas = mundo->GetEscalas();
	tamanio_proyectil.x = escalas.x / 60;
	tamanio_proyectil.y = escalas.y / 50;

	b2World* world = mundo->devolver_world();
	b2BodyDef bodyDef = b2BodyDef(); // creo el body def

	bodyDef.position.x = this->personaje_duenio->getPosition().x; // le asigno una posicion
	bodyDef.position.y = this->personaje_duenio->getPosition().y-1;
	bodyDef.userData = this; // no se si funciona bien esto,
	bodyDef.type = b2_dynamicBody;
	proyectil = world->CreateBody(&bodyDef);

	b2MassData massData = b2MassData();
	massData.mass = 0.1;
	massData.center = b2Vec2(0, 0);
	massData.I = RECT_INERCIA_ROT;
	proyectil->SetMassData(&massData);

	shape_proy = new b2CircleShape();
	shape_proy->m_radius = tamanio_proyectil.x/2;
	b2Filter filtro = b2Filter();
	filtro.groupIndex = INDICE_GRUPO;

	b2FixtureDef fd; // creo un fixture
	fd.filter = filtro;
	fd.restitution = 0;
	fd.friction = 0.5;
	fd.shape = shape_proy;
	proyectil->CreateFixture(&fd); // al body le pongo la fixture creada

	this->personaje_duenio->quitarEnergia(this->personaje_duenio->getEnergia());
	this->aplicarExplosion();
}
