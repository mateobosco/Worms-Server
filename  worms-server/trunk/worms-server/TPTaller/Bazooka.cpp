#include "Bazooka.h"

Bazooka::Bazooka(Personaje* personaje){
	this->fuerza = 0;
	this->tipo = 1;
	this->radio_explosion = 5;
	this->danio = DANIO_BAZOOKA;
	this->personaje_duenio = personaje;
	this->proyectil = NULL;
	this->shape_proy = NULL;
	this->angulo = 0;
}

Bazooka::~Bazooka(){
	if (shape_proy) delete shape_proy; shape_proy = NULL;
}

void Bazooka::disparar(Mundo* mundo){

	b2Vec2 escalas = mundo->GetEscalas();
	tamanio_proyectil.x = escalas.x / 60;
	tamanio_proyectil.y = escalas.y / 50;

	b2World* world = mundo->devolver_world();
	b2BodyDef bodyDef = b2BodyDef(); // creo el body def
	b2Vec2 direccion;
	if(this->personaje_duenio->getOrientacion() == -1){
		int angulo_aux = (180 - angulo);
		direccion.x = cos(angulo_aux * PI / 180 ); //Orientacion IZQ
		direccion.y = sin(angulo_aux * PI / 180 );
	} else{
		direccion.x = cos( angulo * PI / 180 ); //Orientacion DER
		direccion.y = sin( angulo * PI / 180 );
	}
	float32 modulo = sqrt((direccion.x * direccion.x) + (direccion.y * direccion.y) );
	bodyDef.position.x = 2 * (direccion.x / modulo) + this->personaje_duenio->getPosition().x;
	bodyDef.position.y = 2 * (direccion.y / modulo) + this->personaje_duenio->getPosition().y;

	posicion_proyectil.x = bodyDef.position.x;
	posicion_proyectil.y = bodyDef.position.y;

	printf( "POS_CENTRAL_X_Y: x:%f y:%f\t RADIO_X: %f\t RADIO_Y: %f\n",this->personaje_duenio->getPosition().x,
			this->personaje_duenio->getPosition().y, posicion_proyectil.x, posicion_proyectil.y );

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
     std::vector<b2Body*> foundBodies;
     b2Body* llamador;

     bool ReportFixture(b2Fixture* fixture) {
   	  b2Shape* shape= fixture->GetShape();
    	 b2Body* body = fixture->GetBody();

   	  if ((shape->GetType() == 3) || (shape->GetType() == 0)){ // b2ChainShape == 3
   		  if (body != llamador){
   			  vector<b2Body*>::iterator it = std::find(foundBodies.begin(), foundBodies.end(), body);
   			  if(it==foundBodies.end()){
   				  foundBodies.push_back( body );
//		}
   			  }
   		  }
   	  }
      return true;//keep going to find all fixtures in the query area
     }
 };


bool Bazooka::checkImpacto(Mundo* mundo){
	float32 radio = this->shape_proy->m_radius;
	float32 radioPersonaje = this->personaje_duenio->getBody()->GetFixtureList()->GetShape()->m_radius;
	b2Vec2 pos = this->proyectil->GetPosition();
	QueryCheckImpacto query;
	query.llamador = this->proyectil;
	b2AABB aabb;
//	printf("Radio:%f\n", radioPersonaje);
	aabb.upperBound = pos + b2Vec2(radio,radio);
	aabb.lowerBound = pos - b2Vec2(radio,radio);

	b2World* world = mundo->devolver_world();
	world->QueryAABB(&query, aabb);

	std::vector<b2Body*> res = query.foundBodies;
	b2Body* duenio = this->personaje_duenio->getBody();
//	b2Shape* shapeDuenio = duenio->GetFixtureList()->GetShape();


	if (res.size() > 0) {
//		for (size_t i = 0 ; i<res.size(); i++){
//			b2Body* actual = res[i];
//			if (actual->GetPosition() == duenio->GetPosition()){
//				return false;
//			}
//		}
		return true;
	}


	return false;
}

bool Bazooka::setFuerza(){
	if(this->fuerza >= MAX_FUERZA){
		return false;
	} else{
		this->fuerza += (MAX_FUERZA/20);//todo poner algo inferior
		return true;
	}
}

void Bazooka::setAngulo(int un_angulo, int direc){
	this->angulo = un_angulo;
	b2Vec2 direccion;
	//direccion.x = 1 * cos( angulo * PI / 180 ); //Orientacion Derecha
	//direccion.y = 1 * sin( angulo * PI / 180 );
	if(direc == -1){
		int angulo_aux = (180 - angulo);
		direccion.x = cos(angulo_aux * PI / 180 ); //Orientacion IZQ
		direccion.y = sin(angulo_aux * PI / 180 );
		this->setDireccion(direccion);
	} else{
		direccion.x = cos( angulo * PI / 180 ); //Orientacion DER
		direccion.y = sin( angulo * PI / 180 );
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
	this->posicion_proyectil.x = una_posicion.x;
	this->posicion_proyectil.y = una_posicion.y;
}

b2Body* Bazooka::getProyectil(){
	return proyectil;
}

double Bazooka::getAngulo(){
	b2Vec2 direccion = this->getDireccion();
	float32 angulo_aux = atan2( direccion.y, direccion.x);
	this->angulo = angulo_aux;
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

void Bazooka::aplicarExplosion(ManejadorPersonajes *manejador){
    b2World* world = this->proyectil->GetWorld();
    b2Vec2 pos = this->proyectil->GetPosition();
    float32 blastRadius = 25;
    int numRays = 20;
    for (int i = 0; i < numRays; i++) {
        float angle = (i / (float)numRays) * 2 * PI;
        b2Vec2 rayDir( sinf(angle), cosf(angle) );
        rayDir *= blastRadius;
        b2Vec2 rayEnd = pos + /*blastRadius * */rayDir;

        RayCastMasCercano callback;
        world->RayCast(&callback, pos, rayEnd);
        if ( callback.body ){
            b2Body* body = callback.body;
            b2Vec2 posImpacto = callback.pos;
            if ( body == this->proyectil || body->GetType() != b2_dynamicBody ) continue;
            b2Vec2 dir = posImpacto - pos;
            float32 distancia = dir.Normalize();
            if (distancia == 0 ) continue;
            this->checkPersonajeLastimado(body, manejador, this->danio);
            //      float32 invDistancia = 1/distancia;
            //      float32 impulso = this->danio * invDistancia;//*10;
            //      impulso = b2Min(impulso, 500.0f); // estaba en el tutorial, no estoy seguro
            if((abs(body->GetLinearVelocity().x) < 10) && (abs(body->GetLinearVelocity().y) < 10)){
                body->ApplyLinearImpulse(b2Vec2(dir.x,dir.y), posImpacto, true);
            }
            //FALTA SACARLE VIDA A LOS GUSANOS
        }
    }
}

int Bazooka::getRadioExplosion(){
	return this->radio_explosion;
}
