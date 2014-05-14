#include "poligono.h"

Poligono::Poligono(Mundo* mundo, b2Vec2 posicion, float32 escal, int cant_vertices, bool estatico, SDL_Color color, int masa, float32 rotacion): Figura(color,rotacion){
	tipo = 2;
	posicion_inicial = posicion;
	b2World* world = mundo->devolver_world();
	b2BodyDef bodyDef = b2BodyDef(); // creo el body def
	bodyDef.position = posicion; // le asigno una posicion
	bodyDef.userData = this; // no se si funciona bien esto,
	radio = escal;

	if(estatico) bodyDef.type = b2_staticBody;
	else bodyDef.type = b2_dynamicBody;

	body = world->CreateBody(&bodyDef);

	b2MassData massData = b2MassData();
	massData.mass = (float32) masa;
	massData.center = b2Vec2(POLI_CENTRO_MASA_X, POLI_CENTRO_MASA_Y);
	massData.I = POLI_INERCIA_ROT;
	body->SetMassData(&massData);

	b2Vec2 *vertices = new b2Vec2[cant_vertices];
	this->initVertices(vertices, cant_vertices);

	b2PolygonShape* shape = new b2PolygonShape(); // creo una shape
	shape->m_count = cant_vertices;
	shape->Set(vertices , cant_vertices);
	shapePoligono = shape;

	b2FixtureDef fd; // creo un fixture
	b2Filter filtro = b2Filter();
	filtro.groupIndex = INDICE_GRUPO;
	fd.filter = filtro;
	fd.restitution = POLI_RESTITUCION;
	fd.friction = POLI_FRICCION;
	fd.density = POLI_DENSIDAD;
	fd.shape = shape; // le pongo el shape creado
	body->CreateFixture(&fd); // al body le pongo la fixture creada
	rotacion_inicial = body->GetAngle();
	delete[] vertices;
}

Poligono::~Poligono(){
	delete shapePoligono;
	b2Body* mybody = body;
	b2World* myworld = mybody->GetWorld();
	myworld->DestroyBody(mybody);
}

int Poligono::getCantVertices(){
	return ((b2PolygonShape*) body->GetFixtureList()->GetShape())->GetVertexCount();
}

void Poligono::initVertices(b2Vec2* vertices, int cantidad){
	int anguloInicial = 360/cantidad;
	int angulo = anguloInicial - 90 + rotacion_inicial;
	int i;
	for (i = 0 ; i<cantidad ; i ++ ){
		b2Vec2 punto;
		punto.x = radio * cos( angulo * PI / 180 );
		punto.y = radio * sin( angulo * PI / 180 );
		angulo = angulo + anguloInicial;
		vertices[i] = punto;
	}
}

b2Vec2* Poligono::getVertices(){
	int cantidad = this->getCantVertices();
	b2Vec2* verticesGlobales = new b2Vec2[cantidad];
	int i;
	for (i = 0 ; i<cantidad ; i ++ ){
		b2Vec2 puntoGlobal;
		b2Vec2 puntoLocal = shapePoligono->GetVertex(i);
		puntoGlobal = body->GetWorldPoint(puntoLocal);
		verticesGlobales[i] = puntoGlobal;
	}
	return verticesGlobales;
}

float32* Poligono::getVecX(){
	int cantidad = this->getCantVertices();
	float32* vector_x = new float32[cantidad];
	b2Vec2* vertices = getVertices();
	for (int i = 0 ; i<cantidad ; i++ ){
		vector_x[i] = vertices[i].x;
	}
	delete[] vertices;
	return vector_x;
}

float32* Poligono::getVecY(){
	int cantidad = this->getCantVertices();
	float32* vector_y = new float32[cantidad];
	b2Vec2* vertices = getVertices();
	for (int i = 0 ; i<cantidad ; i++ ){
		vector_y[i] = vertices[i].y;
	}
	delete[] vertices;
	return vector_y;
}

int Poligono::dibujar(Dibujador* el_dibujador){
	return el_dibujador->dibujarPoligono(this);
}
