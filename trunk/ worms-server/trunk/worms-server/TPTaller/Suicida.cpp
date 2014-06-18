#include "Suicida.h"

Suicida::Suicida(Personaje *un_personaje) {
	this->danio = DANIO_SUICIDA;
	this->fuerza = 0;
	this->tipo = 5;
	this->personaje_duenio = un_personaje;
	this->radio_explosion = 4;
}

Suicida::~Suicida() {
	if (this->shape_proy) delete this->shape_proy;
}

void Suicida::disparar(Mundo* mundo){
	b2Vec2 escalas = mundo->GetEscalas();
	tamanio_proyectil.x = escalas.x / 60;
	tamanio_proyectil.y = escalas.y / 50;

	b2World* world = mundo->devolver_world();
	b2BodyDef bodyDef = b2BodyDef(); // creo el body def
	bodyDef.position.x = this->personaje_duenio->getPosition().x; // le asigno una posicion
	bodyDef.position.y = this->personaje_duenio->getPosition().y;
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

//	this->personaje_duenio->quitarEnergia(this->personaje_duenio->getEnergia());
//	this->aplicarExplosion();
}

b2Vec2* Suicida::definirImpulso(b2Vec2 destino){
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

bool Suicida::checkImpacto(Mundo* mundo){
	return true;
}

bool Suicida::setFuerza(){
	if(this->fuerza >= MAX_FUERZA){
		//printf("entra al setFuerza: %f \n", this->fuerza);
		return false;
	} else{
		this->fuerza += (MAX_FUERZA/20);//todo poner algo inferior
		printf("entra al setFuerza: %f \n", this->fuerza);
		return true;
	}
}

void Suicida::setAngulo(int un_angulo, int direc){
	this->angulo = un_angulo;
	b2Vec2 direccion;
	direccion.x = 1 * cos( angulo * PI / 180 ); //Orientacion Derecha
	direccion.y = 1 * sin( angulo * PI / 180 );
	this->setDireccion(direccion);
}

void Suicida::setTipo(int tipo_arma){
	this->tipo = (type_arma) tipo_arma;
	switch (tipo){
		case ninguno: break;
		case bazooka: {
//			danio = 100;
		}
		default: break;
	}
}

void Suicida::setDireccion(b2Vec2 una_direccion){
	this->direccion_proyectil.x = una_direccion.x;
	this->direccion_proyectil.y = una_direccion.y;
}

void Suicida::setPosicion(b2Vec2 una_posicion){
	printf("UBICO LA BOMBA EN (%f,%f) \n",una_posicion.x,una_posicion.y);
	this->posicion_proyectil.x = una_posicion.x;
	this->posicion_proyectil.y = una_posicion.y;
}

b2Body* Suicida::getProyectil(){
	return proyectil;
}

double Suicida::getAngulo(){
	return angulo=0;
}

int Suicida::getTipo(){
	printf(" ENTRA EN EL GET TIPO DE BAZOOOOKAAA ****************\n");
	return this->tipo;
}

b2Vec2 Suicida::getDireccion(){
	b2Vec2 velocidad = proyectil->GetLinearVelocity();
	float32 modulo = sqrt((velocidad.x * velocidad.x) + (velocidad.y * velocidad.y) );
	direccion_proyectil.x = velocidad.x / modulo;
	direccion_proyectil.y = velocidad.y / modulo;
	return this->direccion_proyectil;
}

b2Vec2 Suicida::getPosicion(){
	return this->posicion_proyectil = proyectil->GetPosition();
}

b2Vec2 Suicida::getTamanio(){
	return this->tamanio_proyectil;
}

void Suicida::resetFuerza(){
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

//void Suicida::aplicarExplosion(){
//	b2World* world = this->proyectil->GetWorld();
//	b2Vec2 pos = this->proyectil->GetPosition();
//	//printf("UBICO LA BOMBA EN (%f,%f) \n",pos.x,pos.y);
//	float32 blastRadius = 30;
//	int numRays = 20;
//	b2Body *auxiliar = NULL;
//	for (int i = 0; i < numRays; i++) {
//	  float angle = (i / (float)numRays) * PI;
//	  b2Vec2 rayDir( sinf(angle), cosf(angle) );
//	  b2Vec2 rayEnd = pos + blastRadius * rayDir;
//	  //printf("ANGULOS %f \n",angle);
//
//	  RayCastMasCercano callback;
//	  //printf("HAGO UN RAYCAST ENTRE LAS POSICIONES (%f,%f) y (%f,%f) \n",pos.x,pos.y,rayEnd.x,rayEnd.y);
//	  world->RayCast(&callback, pos, rayEnd);
//	  if ( callback.body ){
//
//		b2Body* body = callback.body;
//		 if(auxiliar != NULL && auxiliar == body) continue;
//		 auxiliar = body;
//		b2Vec2 posImpacto = callback.pos;
//		//if ( body == this->proyectil || body->GetType() != b2_dynamicBody ) continue;
//		b2Vec2 dir = posImpacto - pos;
//		float32 distancia = dir.Normalize();
//		//if (distancia == 0 ) continue;
//		float32 invDistancia = 1/distancia;
//		float32 impulso = this->danio * invDistancia;//*10;
//		printf("Distancia invertida: %f\n",invDistancia);
////		impulso = b2Min(impulso, 500.0f); // estaba en el tutorial, no estoy seguro
//		b2Vec2 aux = {dir.x/distancia, dir.y/distancia};
//		body->ApplyLinearImpulse(/*impulso * */aux, posImpacto, true);
//		//printf("EL RAYCAST ENCONTRO UN CUERPO Y LE APLICA UN LINEAR IMPULSE DE %f \n",impulso);
//		//FALTA SACARLE VIDA A LOS GUSANOS
//	  }
//	}
//}

void Suicida::aplicarExplosion(ManejadorPersonajes *manejador){
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

int Suicida::getRadioExplosion(){
	return this->radio_explosion;
}
