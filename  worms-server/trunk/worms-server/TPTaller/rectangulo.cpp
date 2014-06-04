#include <iostream>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <Box2D/Box2D.h>
#include <Box2D/Common/b2Math.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include "figura.h"
#include "rectangulo.h"
#include "Constantes.h"

Rectangulo::Rectangulo(Mundo* mundo, b2Vec2 posicion, float32 anchoParam, float32 altoParam, bool estatico, SDL_Color color, float32 masa, float32 rotacion):Figura(color,rotacion){
	tipo = 1;
	posicion_inicial = posicion;
	b2World* world = mundo->devolver_world();
	b2BodyDef bodyDef = b2BodyDef(); // creo el body def

	bodyDef.position = posicion; // le asigno una posicion
	bodyDef.userData = this;
	bodyDef.angle = rotacion;

	ancho = anchoParam;
	alto = altoParam;
	radio = sqrt((alto/2)*(alto/2) + (ancho/2)*(ancho/2));
	anguloRec = asin((alto/2)/radio) *180.0 / PI;

	if(estatico) bodyDef.type = b2_staticBody;
	else bodyDef.type = b2_dynamicBody;
	body = world->CreateBody(&bodyDef);

	b2MassData massData;
	massData.mass =  masa;
	massData.center = b2Vec2(RECT_CENTRO_MASA_X, RECT_CENTRO_MASA_Y);
	massData.I = RECT_INERCIA_ROT;
	body->SetMassData(&massData);

	b2PolygonShape* polygonShape = new b2PolygonShape(); // creo una shape
	polygonShape->SetAsBox(ancho/2 , alto/2);

	b2Filter filtro = b2Filter();
	filtro.groupIndex = INDICE_GRUPO;

	b2FixtureDef fd; // creo un fixture
	fd.filter = filtro;
	fd.restitution = RECT_RESTITUCION;
	fd.friction = RECT_FRICCION;
	fd.density = RECT_DENSIDAD;
	fd.shape = polygonShape; // le pongo el shape creado
	body->CreateFixture(&fd); // al body le pongo la fixture creada
	rotacion_inicial = body->GetAngle();
	shape = polygonShape;
}

Rectangulo::~Rectangulo() {
	delete shape;
	b2Body* mybody = body;
	b2World* myworld = mybody->GetWorld();
	myworld->DestroyBody(mybody);
}

b2Vec2* Rectangulo::getVertices(){
	b2Vec2* vertices = new b2Vec2[sizeof(b2Vec2)*4];
	float32 complemento = 90 - anguloRec;
	b2Transform transformada = body->GetTransform();
	b2Rot rotacion = transformada.q;
	float32 rot = rotacion.GetAngle();
	b2Vec2 punto;
	float32 ang = rot *(180/PI) + anguloRec;

	punto.Set(radio*cos(ang * PI/180),radio*sin(ang * PI/180));
	vertices[0]= punto + body->GetPosition();
	ang += (2*complemento);

	punto.Set(radio*cos(ang * PI/180),radio*sin(ang * PI/180));
	vertices[1]= punto + body->GetPosition();
	ang += (2*anguloRec);

	punto.Set(radio*cos(ang * PI/180),radio*sin(ang * PI/180));
	vertices[2]= punto + body->GetPosition();
	ang += (2*complemento);

	punto.Set(radio*cos(ang * PI/180),radio*sin(ang * PI/180));
	vertices[3]= punto + body->GetPosition();

	return vertices;
}

float32* Rectangulo::getVecX(){
	int cantidad = 4;
	float32* vector_x = new float32[cantidad];
	b2Vec2* vertices = getVertices();

	for (int i = 0 ; i<cantidad ; i++ ){
		vector_x[i] = vertices[i].x;
	}
	delete[] vertices;
	return vector_x;
}

float32* Rectangulo::getVecY(){
	int cantidad = 4;
	float32* vector_y = new float32[cantidad];
	b2Vec2* vertices = getVertices();

	for (int i = 0 ; i<cantidad ; i++ ){
		vector_y[i] = vertices[i].y;
	}
	delete[] vertices;
	return vector_y;
}

int Rectangulo::dibujar(Dibujador* el_dibujador){
	return el_dibujador->dibujarRectangulo(this);
}
