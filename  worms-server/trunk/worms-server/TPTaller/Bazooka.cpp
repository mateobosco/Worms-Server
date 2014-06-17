#include "Bazooka.h"

#include "Arma.h"

Bazooka::Bazooka(Personaje* personaje){
	this->fuerza = 0;
	this->tipo = 1;
	this->radio_explosion=3;
	personaje_duenio=personaje;
}

Bazooka::~Bazooka(){

}

void Bazooka::disparar(Mundo* mundo){
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
//	bodyDef.position.x = posicion_proyectil.x /*+ 4*direccion_proyectil.x*/; // le asigno una posicion
//	bodyDef.position.y = posicion_proyectil.y /*+ 4*direccion_proyectil.y*/;
	if(this->personaje_duenio->getOrientacion() == 1)
		bodyDef.position.x = this->personaje_duenio->getPosition().x + 1; // le asigno una posicion
	else
		bodyDef.position.x = this->personaje_duenio->getPosition().x - 1;
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
//	proyectil->SetAwake(false);
	b2Vec2 velocidad;
	velocidad.x = direccion_proyectil.x*this->fuerza; // Multiplicar por Fuerza
	velocidad.y = direccion_proyectil.y*this->fuerza; // Multiplicar por Fuerza
	proyectil->SetLinearVelocity(velocidad);
//	b2Vec2 antigravedad = b2Vec2(0,-9.8f);
//	proyectil->ApplyForceToCenter(antigravedad, true );
}

b2Vec2* Bazooka::definirImpulso(b2Vec2 destino){
	b2Vec2 *impulso = new b2Vec2();
	impulso->x = (destino.x - this->proyectil->GetPosition().x) * this->fuerza;
	impulso->y = (destino.y - this->proyectil->GetPosition().y) * this->fuerza;
	return impulso;
}

//Retorna True si chocó con una figura.
//Retorna True si chocó con un personaje.
//Retorna False no hay choque.
/*
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
*/

class QueryCheckImpacto : public b2QueryCallback {
 public:
     std::vector<b2Shape*> foundShapes;

     bool ReportFixture(b2Fixture* fixture) {
   	  b2Shape* shape= fixture->GetShape();

   	  if (shape->GetType() == 3){ // b2ChainShape == 3
		  vector<b2Shape*>::iterator it = std::find(foundShapes.begin(), foundShapes.end(), shape);
		  if(it==foundShapes.end()){
			  foundShapes.push_back( shape );
		  }
   	  }
      return true;//keep going to find all fixtures in the query area
     }
 };

bool Bazooka::checkImpacto(Mundo* mundo){
	printf(" Entra acaaaaaaaaaaa \n");
	float32 radio = this->shape_proy->m_radius;
	b2Vec2 pos = this->proyectil->GetPosition();
	QueryCheckImpacto query;
	b2AABB aabb;
	aabb.upperBound = pos + b2Vec2(radio,radio);
	aabb.lowerBound = pos - b2Vec2(radio,radio);

	b2World* world = mundo->devolver_world();
	world->QueryAABB(&query, aabb);

	std::vector<b2Shape*> res = query.foundShapes;

	if (res.size() > 0) {
		printf(" DEVUELVE TRUEEEE\n");
		return true;
	}


	return false;
}

bool Bazooka::setFuerza(){
	if(this->fuerza >= MAX_FUERZA){
		//printf("entra al setFuerza: %f \n", this->fuerza);
		return false;
	} else{
		this->fuerza += (MAX_FUERZA/20);//todo poner algo inferior
		printf("entra al setFuerza: %f \n", this->fuerza);
		return true;
	}
}

void Bazooka::setAngulo(int un_angulo, int direc){
	this->angulo = un_angulo;
	b2Vec2 direccion;
	printf(" ENTRA EN ESTE ANGULO3 Y RECIBE DE DIRECCION ESTO : %d \n", direc);
	//direccion.x = 1 * cos( angulo * PI / 180 ); //Orientacion Derecha
	//direccion.y = 1 * sin( angulo * PI / 180 );
	if(direc == 1){
		direccion.x = 1 * cos( angulo * PI / 180 ); //Orientacion Derecha
		direccion.y = 1 * sin( angulo * PI / 180 );
		this->setDireccion(direccion);
	}
	if(direc == -1){
		direccion.x = -1 * cos( angulo * PI / 180 ); //Orientacion IZQ
		direccion.y = -1 * sin( angulo * PI / 180 );
		this->setDireccion(direccion);
	}
	if(direc == 0){
		direccion.x = 1 * cos( angulo * PI / 180 ); //Orientacion IZQ
		direccion.y = 1 * sin( angulo * PI / 180 );
		this->setDireccion(direccion);
	}


}

void Bazooka::setTipo(int tipo_arma){
	this->tipo = 1;
}

void Bazooka::setDireccion(b2Vec2 una_direccion){
	this->direccion_proyectil.x = una_direccion.x;
	this->direccion_proyectil.y = una_direccion.y;
}

void Bazooka::setPosicion(b2Vec2 una_posicion){
	printf("UBICO LA BOMBA EN (%f,%f) \n",una_posicion.x,una_posicion.y);
	this->posicion_proyectil.x = una_posicion.x;
	this->posicion_proyectil.y = una_posicion.y;
}

b2Body* Bazooka::getProyectil(){
	return proyectil;
}

int Bazooka::getAngulo(){
	b2Vec2 velocidad = proyectil->GetLinearVelocity();
	float32 angulo_aux = atan2( velocidad.y, velocidad.x);
	this->angulo = (int) angulo_aux;
	return angulo;
}

int Bazooka::getTipo(){
	return this->tipo;
}

b2Vec2 Bazooka::getDireccion(){
	b2Vec2 velocidad = proyectil->GetLinearVelocity();
	float32 modulo = sqrt((velocidad.x * velocidad.x) + (velocidad.y * velocidad.y) );
	direccion_proyectil.x = velocidad.x / modulo;
	direccion_proyectil.y = velocidad.y / modulo;
	return this->direccion_proyectil;
}

b2Vec2 Bazooka::getPosicion(){
	return this->posicion_proyectil = proyectil->GetPosition();
}

b2Vec2 Bazooka::getTamanio(){
	return this->tamanio_proyectil;
}

void Bazooka::resetFuerza(){
	this->fuerza = 0.0f;
}

class RayCastMasCercano : public b2RayCastCallback
{
public:
    b2Body* body;
    b2Vec2 pos;

    RayCastMasCercano() { body = NULL; }

    float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
    {
        body = fixture->GetBody();
        pos = point;
        return fraction;
    }
};

void Bazooka::aplicarExplosion(){
	b2World* world = this->proyectil->GetWorld();
	b2Vec2 pos = this->proyectil->GetPosition();
	//printf("UBICO LA BOMBA EN (%f,%f) \n",pos.x,pos.y);
	float32 blastRadius = 30;
	int numRays = 20;
	for (int i = 0; i < numRays; i++) {
		float angle = (i / (float)numRays) * PI;
		b2Vec2 rayDir(cosf(angle), sinf(angle));
		b2Vec2 rayEnd = pos + blastRadius * rayDir;
		//printf("ANGULOS %f \n",angle);

		RayCastMasCercano callback;
		//printf("HAGO UN RAYCAST ENTRE LAS POSICIONES (%f,%f) y (%f,%f) \n",pos.x,pos.y,rayEnd.x,rayEnd.y);
		world->RayCast(&callback, pos, rayEnd);
		if ( callback.body ){
			b2Body* body = callback.body;
			b2Vec2 posImpacto = callback.pos;
			if ( body == this->proyectil || body->GetType() != b2_dynamicBody ) continue;
			b2Vec2 dir = posImpacto - pos;
			float32 distancia = dir.Normalize();
			if (distancia == 0 ) continue;
	//		float32 invDistancia = 1/distancia;
	//		float32 impulso = this->danio * invDistancia*10;
	//		impulso = b2Min(impulso, 500.0f); // estaba en el tutorial, no estoy seguro
		  	if((abs(body->GetLinearVelocity().x) < 10) && (abs(body->GetLinearVelocity().y) < 10)){
		  		body->ApplyLinearImpulse(dir, posImpacto, true);
		  	}
			//printf("EL RAYCAST ENCONTRO UN CUERPO Y LE APLICA UN LINEAR IMPULSE DE %f \n",impulso);
			//FALTA SACARLE VIDA A LOS GUSANOS
		}
	}
}

int Bazooka::getRadioExplosion(){
	return this->radio_explosion;
}
