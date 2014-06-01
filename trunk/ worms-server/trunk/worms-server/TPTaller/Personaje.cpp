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

Personaje::Personaje(Mundo* mundo, Uint8 numero_jugador, char* nombre_client) {
	conectado = 1;
	nombre_cliente = nombre_client;
	muerto = false;

	dir_imagen = "TPTaller/imagenes/gusanitoderecha.png";
	nro_jugador = numero_jugador;
	b2Vec2 escalas = mundo->GetEscalas();
	alto = escalas.y / 50;
	ancho = escalas.x / 60;
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

//	b2PolygonShape* polygonShape = new b2PolygonShape(); // creo una shape
//	polygonShape->SetAsBox(ancho/2 , alto/2) ;
	shape2 = new b2CircleShape();
	shape2->m_radius = ancho/2;


	b2Filter filtro = b2Filter();
	filtro.groupIndex = INDICE_GRUPO;

	b2FixtureDef fd; // creo un fixture
	fd.filter = filtro;
	fd.restitution = 0;
	fd.friction = 0.5;
	//fd.density = 100;
	fd.shape = shape2;
//	fd.shape = polygonShape; // le pongo el shape creado
	body->CreateFixture(&fd); // al body le pongo la fixture creada
//	shape = polygonShape;
	body->SetAwake(false);
	//seleccionado = false;
	for (int i = 0 ; i<4; i++){
		seleccionado[i] = false;
	}
	orientacion = 1;

}

Personaje::~Personaje() {
	b2World* mundo = body->GetWorld();
	mundo->DestroyBody(body);
}

void Personaje::mover(b2Vec2 direccion){
	float32 modulo = 0.20;
	b2Vec2 fuerza = b2Vec2(modulo * direccion.x, modulo* direccion.y ); // TODO ver cuanto aplicarle
	if (direccion.x > 0) orientacion = 1;
	if (direccion.x < 0) orientacion = -1;
	//body->ApplyForceToCenter(fuerza, true );
	body->ApplyLinearImpulse(fuerza, body->GetWorldCenter(), true );
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

void Personaje::leermovimiento(int direccion, int id_jugador){
	if (this->nro_jugador == id_jugador&& seleccionado[id_jugador]){

		if (direccion == 3 && body->GetLinearVelocity().x < 0.7){ // para la derecha
			dir_imagen = "TPTaller/imagenes/gusanitoderecha.png";
			orientacion=1;
			this->mover(b2Vec2(2,0));
			return;
		}
		if (direccion == 1 && body->GetLinearVelocity().x > -0.7 ){ // para la izquierda
			dir_imagen = "TPTaller/imagenes/gusanitoizquierda.png";
			orientacion=-1;
			this->mover(b2Vec2(-2,0));
			return;
		}
		b2Vec2 posicion = this->getPosition();
		b2CircleShape* over = new b2CircleShape();
		over->m_radius = 0.0001;
		b2Transform transformOver = b2Transform(posicion+b2Vec2(0,alto/2), b2Rot(0) );
		b2World* mundo = body->GetWorld();
		bool resultado;

		for (b2Body* body_actual = mundo->GetBodyList()->GetNext(); body_actual; body_actual = body_actual->GetNext()){

			if (body_actual->GetPosition() == body->GetPosition()) continue;

			b2Transform transformada_actual = body_actual->GetTransform();
			b2Fixture* fix = body_actual->GetFixtureList();
			b2Shape* shape_actual = fix->GetShape();

			resultado = b2TestOverlap(shape_actual,0, over , 0,  transformada_actual, transformOver);
			if (resultado) break;
		}
		delete over;

		if ((direccion == 2  && resultado) || (direccion == 2  && body->GetLinearVelocity().y == 0)){ // para arriba
			this->mover(b2Vec2(0,-3));
			return;
		}
		//printf( " LA VELOCIDAD EN Y ES : %f \n", body->GetLinearVelocity().y);
		if ( (direccion == 4  && body->GetLinearVelocity().y == 0 )  ){ // para arriba a la derecha
			printf(" APLICO UN PAQUETE movimiento en la direccion  4");
					this->mover(b2Vec2(2,-3));
					return;
		}
		if ( (direccion == 5  && body->GetLinearVelocity().y == 0  )){ // para arriba a la izq
			printf(" APLICO UN PAQUETE movimiento en la direccion 5 \n");
				this->mover(b2Vec2(-2,-3));
				return;
		}

//		if (direccion == 2  && body->GetLinearVelocity().y == 0){ // para arriba
//			this->mover(b2Vec2(0,-5));
//		}
	}
}

b2Body* Personaje::getBody(){
	return this->body;
}

void Personaje::setSeleccionado(bool seleccion, int id_jugador){
	this->seleccionado[id_jugador] = seleccion;
}

b2CircleShape* Personaje::getShape(){
	return this->shape2;
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

const char* Personaje::getDirImagen(){
	return dir_imagen;
}

bool Personaje::getSeleccionado(){
	return seleccionado[this->nro_jugador];
}
bool* Personaje::getSeleccion(){
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
int Personaje::getOrientacion(){
	return orientacion;
}

void Personaje::desconectar(){
	this->conectado=0;
}

void Personaje::conectar(){
	this->conectado = 1;
}

char* Personaje::getNombreCliente(){
	return this->nombre_cliente;
}

