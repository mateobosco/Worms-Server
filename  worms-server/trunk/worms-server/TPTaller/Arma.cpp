#include "Arma.h"

Arma::Arma(){
	this->fuerza = 0;
	this->tipo = ninguno;
}

Arma::~Arma(){

}

void Arma::disparar(Mundo* mundo){
	dir_imagen = "TPTaller/imagenes/misil_bazooka.png";
//	printf("DISPARAR \n ");
//	printf("posicion: x- %f y- %f \n", posicion_proyectil.x, posicion_proyectil.y);
//	printf("Direccion: x- %f y- %f \n", direccion_proyectil.x, direccion_proyectil.y);
//	printf("Tamanio: x- %f y- %f \n", tamanio_proyectil.x, tamanio_proyectil.y);

	b2Vec2 escalas = mundo->GetEscalas();
	tamanio_proyectil.x = escalas.x / 60;
	tamanio_proyectil.y = escalas.y / 50;

	b2World* world = mundo->devolver_world();
	b2BodyDef bodyDef = b2BodyDef(); // creo el body def
	bodyDef.position.x = posicion_proyectil.x + 4*direccion_proyectil.x; // le asigno una posicion
	bodyDef.position.y = posicion_proyectil.y + 4*direccion_proyectil.y;
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
//	proyectil->SetAwake(false);
	b2Vec2 velocidad;
	velocidad.x = direccion_proyectil.x*10; // Multiplicar por Fuerza 
	velocidad.y = direccion_proyectil.y*10; // Multiplicar por Fuerza 
	proyectil->SetLinearVelocity(velocidad);
//	b2Vec2 antigravedad = b2Vec2(0,-9.8f);
//	proyectil->ApplyForceToCenter(antigravedad, true );
}

b2Vec2* Arma::definirImpulso(b2Vec2 destino){
	b2Vec2 *impulso = new b2Vec2();
	impulso->x = (destino.x - this->proyectil->GetPosition().x) * this->fuerza;
	impulso->y = (destino.y - this->proyectil->GetPosition().y) * this->fuerza;
	return impulso;
}

//Retorna True si chocó con una figura.
//Retorna True si chocó con un personaje.
//Retorna False no hay choque.
bool Arma::checkImpacto(Mundo *mundo){

//	if (this->proyectil->GetContactList() && this->proyectil->GetContactList()->contact->IsTouching()){
//		printf("BIINGOOOO: HAY IMPACTO \n");
//		return true;
//	} else{
//		return false;
//	}

	Figura **figuras = mundo->getFiguras();
	Figura *figura_aux;
	Personaje **personajes = mundo->getPersonajes();
	Personaje *personaje_aux;

	b2Shape *forma_ins, *forma_world;
	int index_ins, index_world;
	b2Transform transf_ins, transf_world;

	forma_ins = this->proyectil->GetFixtureList()->GetShape();
	index_ins = 0;
	transf_ins = this->proyectil->GetTransform();
	bool overlap;
	for(unsigned int i = 0; i < mundo->GetCantidadFiguras(); i++){
		figura_aux = figuras[i];
		forma_world = figura_aux->getBody()->GetFixtureList()->GetShape();
		index_world = 0;
		transf_world = figura_aux->getBody()->GetTransform();
		overlap = b2TestOverlap(forma_ins, index_ins, forma_world, index_world, transf_ins, transf_world);
		if(overlap){
			return true;
		}
	}
	for(int i = 0; i < mundo->getCantidadPersonajes(); i++){
		personaje_aux = personajes[i];
		forma_world = personaje_aux->getBody()->GetFixtureList()->GetShape();
		index_world = 0;
		transf_world = personaje_aux->getBody()->GetTransform();
		overlap = b2TestOverlap(forma_ins, index_ins, forma_world, index_world, transf_ins, transf_world);
		if(overlap){
			personaje_aux->quitarEnergia(this->danio);
			return true;
		}
	}
	return false;

}

bool Arma::setFuerza(){
	if(this->fuerza >= MAX_FUERZA){
		//printf("entra al setFuerza: %f \n", this->fuerza);
		return false;
	} else{
		this->fuerza += (MAX_FUERZA / 10);//todo poner algo inferior
		//printf("entra al setFuerza: %f \n", this->fuerza);
		return true;
	}
}

void Arma::setAngulo(int un_angulo){
	this->angulo = un_angulo;
	b2Vec2 direccion;
	direccion.x = 1 * cos( angulo * PI / 180 ); //Orientacion Derecha
	direccion.y = 1 * sin( angulo * PI / 180 );
	this->setDireccion(direccion);
}

void Arma::setTipo(int tipo_arma){
	this->tipo = (type_arma) tipo_arma;
	switch (tipo){
		case ninguno: break;
		case bazooka: {
//			danio = 100;
		}
		default: break;
	}
}

void Arma::setDireccion(b2Vec2 una_direccion){
	this->direccion_proyectil.x = una_direccion.x;
	this->direccion_proyectil.y = una_direccion.y;
}

void Arma::setPosicion(b2Vec2 una_posicion){
	this->posicion_proyectil.x = una_posicion.x;
	this->posicion_proyectil.y = una_posicion.y;
}

b2Body* Arma::getProyectil(){
	return proyectil;
}

int Arma::getAngulo(){
	b2Vec2 velocidad = proyectil->GetLinearVelocity();
	float32 angulo_aux = atan2( velocidad.y, velocidad.x);
	this->angulo = (int) angulo_aux;
	return angulo;
}

int Arma::getTipo(){
	return this->tipo;
}

b2Vec2 Arma::getDireccion(){
	b2Vec2 velocidad = proyectil->GetLinearVelocity();
	float32 modulo = sqrt((velocidad.x * velocidad.x) + (velocidad.y * velocidad.y) );
	direccion_proyectil.x = velocidad.x / modulo;
	direccion_proyectil.y = velocidad.y / modulo;
	return this->direccion_proyectil;
}

b2Vec2 Arma::getPosicion(){
	return this->posicion_proyectil = proyectil->GetPosition();
}

b2Vec2 Arma::getTamanio(){
	return this->tamanio_proyectil;
}

void Arma::resetFuerza(){
	this->fuerza = 0.0f;
}
