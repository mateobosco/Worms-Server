/*
 * Personaje.cpp
 *
 *  Created on: 24/04/2014
 *      Author: juanmahidalgo
 */

#include "Personaje.h"

int random( int n ) // TODO
{
    static std::mt19937 generator( std::time(0) ) ;
    std::uniform_int_distribution<> distribution( 0, n-1 ) ;
    return distribution(generator) ;
}

Personaje::Personaje(Mundo* mundo, Uint8 numero_jugador) {
	conectado = 1;
	muerto = false;
	dir_imagen = "TPTaller/imagenes/gusanitoderecha.png";
	nro_jugador = numero_jugador;
	b2Vec2 escalas = mundo->GetEscalas();
	alto = escalas.y / 50;
	ancho = escalas.x / 100;
	b2Vec2* vertices_tierra = mundo->GetVerticesTierra();
	bool posicion_correcta = false;
	b2Vec2 posicion;
	while (!posicion_correcta){
		int indice = random( (mundo->GetCantVerticesTierra()));
	//int indice = 250;
		posicion = vertices_tierra[indice] - b2Vec2(0,alto/2);
		if (posicion.y < mundo->getAgua()->GetNivel()){
			posicion_correcta=true;
		}
	}
	b2World* world = mundo->devolver_world();
	b2BodyDef bodyDef = b2BodyDef(); // creo el body def
	bodyDef.position = posicion; // le asigno una posicion
	bodyDef.userData = this; // no se si funciona bien esto,
	bodyDef.type = b2_dynamicBody;
	body = world->CreateBody(&bodyDef);

	b2MassData massData = b2MassData();

	massData.mass = 0.1;
	massData.center = b2Vec2(0, 0);
	massData.I = RECT_INERCIA_ROT;
	body->SetMassData(&massData);

	b2PolygonShape* polygonShape = new b2PolygonShape(); // creo una shape
	polygonShape->SetAsBox(ancho/2 , alto/2) ;

	b2Filter filtro = b2Filter();
	filtro.groupIndex = INDICE_GRUPO;

	b2FixtureDef fd; // creo un fixture
	fd.filter = filtro;
	fd.restitution = 0;
	fd.friction = 1;
	//fd.density = 100;
	fd.shape = polygonShape; // le pongo el shape creado
	body->CreateFixture(&fd); // al body le pongo la fixture creada
	shape = polygonShape;
	body->SetAwake(false);
	//seleccionado = false;
	for (int i = 0 ; i<4; i++){
		seleccionado[i] = false;
	}

}

Personaje::~Personaje() {
	b2World* mundo = body->GetWorld();
	mundo->DestroyBody(body);
	//delete shape; //SUPONGO QUE ESTO YA LO HACE EL DESTROY BODY
}

void Personaje::mover(b2Vec2 direccion){
	float32 modulo = 1;
	b2Vec2 fuerza = b2Vec2(modulo * direccion.x, modulo* direccion.y ); // TODO ver cuanto aplicarle
	body->ApplyForceToCenter(fuerza, true );
}

void Personaje::dejar_quieto(){

}


void Personaje::saltar(b2Vec2 direccion){
	//TODO

}

SDL_Texture* Personaje::dibujar(Dibujador* el_dibujador){
	return el_dibujador->dibujarPersonaje2(this);
}


b2Vec2* Personaje::getVertices(){
	int cantidad = 4;
	b2Vec2* verticesGlobales = new b2Vec2[cantidad];
	int i;
	for (i = cantidad-1 ; i>=0 ; i -- ){
		b2Vec2 puntoGlobal;
		b2Vec2 puntoLocal = shape->GetVertex(i);
		puntoGlobal = body->GetWorldPoint(puntoLocal);
		verticesGlobales[i] = puntoGlobal;
	}
	return verticesGlobales;
}



float32* Personaje::getVecX(){
	int cantidad = 4;
	float32* vector_x = new float32[cantidad];
	b2Vec2* vertices = getVertices();

	for (int i = cantidad-1 ; i>=0 ; i-- ){
		vector_x[i] = vertices[i].x;
	}
	delete[] vertices;
	//free(vertices);
	return vector_x;
}


float32* Personaje::getVecY(){
	int cantidad = 4;
	float32* vector_y = new float32[cantidad];
	b2Vec2* vertices = getVertices();

	for (int i = cantidad-1 ; i>=0 ; i-- ){
		vector_y[i] = vertices[i].y;
	}
	delete[] vertices;
	//free(vertices);
	return vector_y;
}

//void Personaje::leermovimiento(SDL_Event evento, bool* KEYS, int id_jugador){
//	if (this->nro_jugador == id_jugador && seleccionado[id_jugador-1]){
//		if (KEYS[100] && body->GetLinearVelocity().x == 0){ // para la derecha
//			dir_imagen = "TPTaller/imagenes/gusanitoderecha.png";
//			this->mover(b2Vec2(10,0));
//		}
//		if (KEYS[101] && body->GetLinearVelocity().x == 0 ){ // para la izquierda
//			dir_imagen = "TPTaller/imagenes/gusanitoizquierda.png";
//			this->mover(b2Vec2(-10,0));
//		}
//		if ((KEYS[102] || KEYS[SDLK_SPACE])  && body->GetLinearVelocity().y == 0){ // para arriba
//			this->mover(b2Vec2(0,-15)); //15
//		}
//	}
//}

void Personaje::leermovimiento(int direccion, int id_jugador){
	if (this->nro_jugador == id_jugador && seleccionado[id_jugador]){
		if (direccion == 1 && body->GetLinearVelocity().x == 0){ // para la derecha
			dir_imagen = "TPTaller/imagenes/gusanitoderecha.png";
			this->mover(b2Vec2(10,0));
		}
		if (direccion == -1 && body->GetLinearVelocity().x == 0 ){ // para la izquierda
			dir_imagen = "TPTaller/imagenes/gusanitoizquierda.png";
			this->mover(b2Vec2(-10,0));
		}
		if (direccion == 0  && body->GetLinearVelocity().y == 0){ // para arriba
			this->mover(b2Vec2(0,-15)); //15
		}
	}
}

b2Body* Personaje::getBody(){
	return this->body;
}

void Personaje::setSeleccionado(bool seleccion, int id_jugador){
	this->seleccionado[id_jugador] = seleccion;
}

b2PolygonShape* Personaje::getShape(){
	return this->shape;
}

b2Vec2 Personaje::getPosition(){
	return body->GetPosition();
}

float32 Personaje::getAlto(){
	return alto;
}

float32 Personaje::getAncho(){
	return ancho;
}

char* Personaje::getDirImagen(){
	return dir_imagen;
}

bool Personaje::getSeleccionado(){
	return seleccionado;
}

int Personaje::getNrojugador(){
	return nro_jugador;
}

void Personaje::setMuerto(){
	muerto=true;
}

bool Personaje::getMuerto(){
	return muerto;
}

int Personaje::getConectado(){
	return conectado;
}

void Personaje::setConectado(int estado){
	conectado = estado;
}


