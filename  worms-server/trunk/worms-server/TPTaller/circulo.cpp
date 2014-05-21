#include "circulo.h"


Circulo::Circulo(Mundo *mundo,  b2Vec2 posicion, float32 escala,  bool estatico, SDL_Color color, float32 masa, float32 rot) : Figura(color,rot){

	tipo = 0;
	posicion_inicial = posicion;
	b2World* world = mundo->devolver_world();

	b2BodyDef bodyDef = b2BodyDef(); // creo el body def
	bodyDef.position = posicion ; // le asigno una posicion
	bodyDef.userData = this;
	b2MassData massData;
	radio = escala;
	massData.mass =  masa;
	massData.center = b2Vec2(CIRC_CENTRO_MASA_X, CIRC_CENTRO_MASA_Y);
	massData.I = CIRC_INERCIA_ROT;
	if(estatico) bodyDef.type = b2_staticBody;
	else bodyDef.type = b2_dynamicBody;
	body = world->CreateBody(&bodyDef);
	body->SetMassData(&massData);
	b2CircleShape* shape =new b2CircleShape(); // creo una shape
	shape->m_radius = escala;
	b2FixtureDef fd; // creo un fixture
	b2Filter filtro = b2Filter();
	filtro.groupIndex = INDICE_GRUPO;
	fd.filter = filtro;
	fd.restitution = CIRC_RESTITUCION;
	fd.friction = CIRC_FRICCION;
	fd.density = CIRC_DENSIDAD;
	fd.shape = shape; // le pongo el shape creado
	body->CreateFixture(&fd); // al body le pongo la fixture creada
	rotacion_inicial = body->GetAngle();
	delete shape;
}

Circulo::~Circulo(){
	b2Body* mybody = body;
	b2World* myworld = mybody->GetWorld();
	myworld->DestroyBody(mybody);
}

float Circulo::getRadio(){
	return this->radio;
}

int Circulo::dibujar(Dibujador* el_dibujador){
	return el_dibujador->dibujarCirculo(this);
}
