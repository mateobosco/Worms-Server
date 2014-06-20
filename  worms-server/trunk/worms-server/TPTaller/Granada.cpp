#include "Granada.h"

Granada::Granada(Personaje* personaje){
	this->fuerza = 0;
	this->tipo = granada;
	this->radio_explosion = 3;
	this->personaje_duenio = personaje;
	this->reloj_comienzo = 0;
	this->danio = DANIO_GRANADA;
}

Granada::~Granada(){
	if (this->shape_proy) delete this->shape_proy;
}

void Granada::disparar(Mundo* mundo){

	b2Vec2 escalas = mundo->GetEscalas();
	tamanio_proyectil.x = escalas.x / 60;
	tamanio_proyectil.y = escalas.y / 50;

	b2World* world = mundo->devolver_world();
	b2BodyDef bodyDef = b2BodyDef(); // creo el body def
//	bodyDef.position.x = posicion_proyectil.x /*+ 4*direccion_proyectil.x*/; // le asigno una posicion
//	bodyDef.position.y = posicion_proyectil.y /*+ 4*direccion_proyectil.y*/;
	bodyDef.position.x = this->personaje_duenio->getPosition().x; // le asigno una posicion
	bodyDef.position.y = this->personaje_duenio->getPosition().y-1;
	bodyDef.userData = this; // no se si funciona bien esto,
	bodyDef.type = b2_dynamicBody;
	proyectil = world->CreateBody(&bodyDef);

	b2MassData massData = b2MassData();
	massData.mass = 0.5;
	massData.center = b2Vec2(0, 0);
	massData.I = 5.0f; //RECT_INERCIA_ROT;
	proyectil->SetMassData(&massData);

	shape_proy = new b2CircleShape();
	shape_proy->m_radius = tamanio_proyectil.x/2;
	b2Filter filtro = b2Filter();
	filtro.groupIndex = INDICE_GRUPO;

	b2FixtureDef fd; // creo un fixture
	fd.filter = filtro;
	fd.restitution = 0.5;
	fd.friction = 1;
	fd.density = 0.5;
	fd.shape = shape_proy;
	proyectil->CreateFixture(&fd); // al body le pongo la fixture creada
	b2Vec2 velocidad;
	velocidad.x = direccion_proyectil.x*this->fuerza; // Multiplicar por Fuerza
	velocidad.y = direccion_proyectil.y*this->fuerza; // Multiplicar por Fuerza
	proyectil->SetLinearVelocity(velocidad);
	this->reloj_comienzo = SDL_GetTicks();
}

b2Vec2* Granada::definirImpulso(b2Vec2 destino){
	b2Vec2 *impulso = new b2Vec2();
	impulso->x = (destino.x - this->proyectil->GetPosition().x) * this->fuerza;
	impulso->y = (destino.y - this->proyectil->GetPosition().y) * this->fuerza;
	return impulso;
}

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

bool Granada::checkImpacto(Mundo* mundo){
	int resultado = SDL_GetTicks() - reloj_comienzo;
	if (resultado >= 5000) return true;
	else return false;
}

int Granada::getContadorSegundos(){
	if (SDL_GetTicks() - reloj_comienzo > 5000)
		return 0;
	else{
		return  ((5000 - (SDL_GetTicks() - reloj_comienzo)) / 1000 ) + 1;
	}
}

bool Granada::setFuerza(){
	if(this->fuerza >= MAX_FUERZA){
		return false;
	} else{
		this->fuerza += (MAX_FUERZA/20);//todo poner algo inferior
		return true;
	}
}

void Granada::setAngulo(int un_angulo, int direc){
	this->angulo = un_angulo;
	b2Vec2 direccion;
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

void Granada::setTipo(int tipo_arma){
	this->tipo = (type_arma) 2;
}

void Granada::setDireccion(b2Vec2 una_direccion){
	this->direccion_proyectil.x = una_direccion.x;
	this->direccion_proyectil.y = una_direccion.y;
}

void Granada::setPosicion(b2Vec2 una_posicion){
	this->posicion_proyectil.x = una_posicion.x;
	this->posicion_proyectil.y = una_posicion.y;
}

b2Body* Granada::getProyectil(){
	return proyectil;
}

double Granada::getAngulo(){
	angulo = this->proyectil->GetAngle()* 180/PI;
	return angulo;
}

int Granada::getTipo(){
	return this->tipo;
}

b2Vec2 Granada::getDireccion(){
	b2Vec2 velocidad = proyectil->GetLinearVelocity();
	float32 modulo = sqrt((velocidad.x * velocidad.x) + (velocidad.y * velocidad.y) );
	direccion_proyectil.x = velocidad.x / modulo;
	direccion_proyectil.y = velocidad.y / modulo;
	return this->direccion_proyectil;
}

b2Vec2 Granada::getPosicion(){
	return this->posicion_proyectil = proyectil->GetPosition();
}

b2Vec2 Granada::getTamanio(){
	return this->tamanio_proyectil;
}

void Granada::resetFuerza(){
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
void Granada::aplicarExplosion(ManejadorPersonajes *manejador){
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
            this->checkPersonajeLastimado(body, manejador, this->danio );
            //      float32 invDistancia = 1/distancia;
            //      float32 impulso = this->danio * invDistancia;//*10;
            //      impulso = b2Min(impulso, 500.0f); // estaba en el tutorial, no estoy seguro
            if((abs(body->GetLinearVelocity().x) < 10) && (abs(body->GetLinearVelocity().y) < 10)){
                body->ApplyLinearImpulse(b2Vec2(dir.x ,dir.y), posImpacto, true);
            }
            //FALTA SACARLE VIDA A LOS GUSANOS
        }
    }
}
int Granada::getRadioExplosion(){
	return this->radio_explosion;
}
