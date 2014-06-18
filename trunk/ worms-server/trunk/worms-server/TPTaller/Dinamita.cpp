#include "Dinamita.h"

Dinamita::Dinamita(Personaje *personaje) {
	this->fuerza = 0;
	this->tipo = (type_arma) 3;
	this->radio_explosion = 3;
	this->personaje_duenio = personaje;
	this->reloj_comienzo = 0;
	this->danio = DANIO_DINAMITA;
}

Dinamita::~Dinamita() {
	// TODO Auto-generated destructor stub
}

void Dinamita::disparar(Mundo* mundo){

	b2Vec2 escalas = mundo->GetEscalas();
	tamanio_proyectil.x = escalas.x / 60;
	tamanio_proyectil.y = escalas.y / 50;

	b2World* world = mundo->devolver_world();
	b2BodyDef bodyDef = b2BodyDef(); // creo el body def
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
	this->reloj_comienzo = SDL_GetTicks();
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

bool Dinamita::checkImpacto(Mundo* mundo){
	int resultado = SDL_GetTicks() - reloj_comienzo;
	if (resultado >= 4000) return true;
	else return false;
}

int Dinamita::getContadorSegundos(){
	if (SDL_GetTicks() - reloj_comienzo > 4000)
		return 0;
	else{
		return  ((4000 - (SDL_GetTicks() - reloj_comienzo)) / 1000 ) + 1;
	}
}

bool Dinamita::setFuerza(){
	if(this->fuerza >= MAX_FUERZA){
		//printf("entra al setFuerza: %f \n", this->fuerza);
		return false;
	} else{
		this->fuerza += (MAX_FUERZA/20);//todo poner algo inferior
		printf("entra al setFuerza: %f \n", this->fuerza);
		return true;
	}
}

void Dinamita::setAngulo(int un_angulo, int direc){
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

void Dinamita::setTipo(int tipo_arma){
	this->tipo = (type_arma) 3;
}

void Dinamita::setDireccion(b2Vec2 una_direccion){
	this->direccion_proyectil.x = una_direccion.x;
	this->direccion_proyectil.y = una_direccion.y;
}

void Dinamita::setPosicion(b2Vec2 una_posicion){
	printf("UBICO LA BOMBA EN (%f,%f) \n",una_posicion.x,una_posicion.y);
	this->posicion_proyectil.x = una_posicion.x;
	this->posicion_proyectil.y = una_posicion.y;
}

b2Body* Dinamita::getProyectil(){
	return proyectil;
}

double Dinamita::getAngulo(){
	return angulo = 0;
}

int Dinamita::getTipo(){
	return this->tipo;
}

b2Vec2 Dinamita::getDireccion(){
	b2Vec2 velocidad = proyectil->GetLinearVelocity();
	float32 modulo = sqrt((velocidad.x * velocidad.x) + (velocidad.y * velocidad.y) );
	direccion_proyectil.x = velocidad.x / modulo;
	direccion_proyectil.y = velocidad.y / modulo;
	return this->direccion_proyectil;
}

b2Vec2 Dinamita::getPosicion(){
	return this->posicion_proyectil = proyectil->GetPosition();
}

b2Vec2 Dinamita::getTamanio(){
	return this->tamanio_proyectil;
}

void Dinamita::resetFuerza(){
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

void Dinamita::aplicarExplosion(ManejadorPersonajes *manejador){
    b2World* world = this->proyectil->GetWorld();
    b2Vec2 pos = this->proyectil->GetPosition();
    //printf("UBICO LA BOMBA EN (%f,%f) \n",pos.x,pos.y);
    float32 blastRadius = 25;
    int numRays = 20;
    for (int i = 0; i < numRays; i++) {
        float angle = (i / (float)numRays) * 2 * PI;
        b2Vec2 rayDir( sinf(angle), cosf(angle) );
        rayDir *= blastRadius;
        b2Vec2 rayEnd = pos + /*blastRadius * */rayDir;
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
            this->checkPersonajeLastimado(body, manejador, this->danio * (blastRadius/distancia) );
            //      float32 invDistancia = 1/distancia;
            //      float32 impulso = this->danio * invDistancia;//*10;
            //      impulso = b2Min(impulso, 500.0f); // estaba en el tutorial, no estoy seguro
            if((abs(body->GetLinearVelocity().x) < 10) && (abs(body->GetLinearVelocity().y) < 10)){
                body->ApplyLinearImpulse(b2Vec2(dir.x* (blastRadius/distancia) ,dir.y* (blastRadius/distancia)), posImpacto, true);
            }
            //printf("EL RAYCAST ENCONTRO UN CUERPO Y LE APLICA UN LINEAR IMPULSE DE %f \n",impulso);
            //FALTA SACARLE VIDA A LOS GUSANOS
        }
    }
}
int Dinamita::getRadioExplosion(){
	return this->radio_explosion;
}
