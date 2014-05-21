#include "mundo.h"
#include "Personaje.h"

Mundo::Mundo(b2Vec2 gravedad, Agua* el_agua, b2Vec2 las_escalas){
	this->escalas = las_escalas;
	this->grav = gravedad;
	this->agua = el_agua;
	this->cantidad_figuras = 0;
	this->my_world = new b2World(gravedad);
	this->cantidad_personajes = 0;
	this->cantidad_jugadores = 0;
	this->cantidad_vertices_tierra = 0;
	this->figuras = NULL;
	this->vector_personajes = NULL;
	this->vertices_tierra = NULL;
}

Mundo::~Mundo(){
	delete this->my_world;
}

b2Body* Mundo::devolverBodies(){
	b2Body* cuerpos = my_world->GetBodyList();
	return cuerpos;
}

void Mundo::crear_tierra(vector<int> vector_tierra){
	b2ChainShape shape;
	size_t i;
	int altura;
	size_t ancho = vector_tierra.size();
	b2Vec2* vector = new b2Vec2(); //todo es necesario mantenerlo o se puede delete?
	for (i = 0; i < ancho; i++){
		altura = vector_tierra[i] ;
		b2Vec2 posicion;
		posicion.x = (float32) i;
		posicion.y = (float32) altura;
		vector[i] = posicion;
	}
	shape.CreateChain(vector, ancho);
	b2BodyDef ChainDef;
	b2Body* body = my_world->CreateBody(&ChainDef);
	b2FixtureDef fd; // creo un fixture
	fd.shape = &shape; // le pongo el shape creado
	body->CreateFixture(&fd); // al body le pongo la fixture creada
	//delete vector; todo

}

b2World* Mundo::devolver_world(){
	return this->my_world;
}

void Mundo::step(float32 timeStep, int32 velocityIterations, int32 positionIterations){
	this->my_world->Step(timeStep, velocityIterations, positionIterations);
}

void Mundo::Crear_Chains(b2Vec2* tierra, float32 pixelesX){
	size_t cantidad = pixelesX;
	b2Vec2* vs = new b2Vec2[cantidad+2];
	size_t i;
	vs[0].Set(-10, 200);
	float32 aux = 0;
	for(i = 0; i < cantidad; i++){
		vs[i+1].Set(tierra[i].x ,tierra[i].y);
		aux = tierra[i].x;
	}
	vs[cantidad+1].Set(aux + 100, 200);
	b2Body* body;
	b2BodyDef bodyDef = b2BodyDef();
	body = my_world->CreateBody(&bodyDef);

	b2ChainShape shape;
	shape.CreateChain(vs, cantidad+2);

	b2FixtureDef fd;
	fd.shape = &shape;

	body->CreateFixture(&fd);
	b2Vec2 vertices;
	for (i = 0; i < cantidad; i++){
		vertices = (shape.m_vertices)[i];
	}
	this->vertices_tierra = vs;
	this->cantidad_vertices_tierra = cantidad+2;
	delete[] tierra;
	//delete[] vs;
}

void Mundo::comprobar_nivel_agua(){
	for (int i = 0; i < (int) cantidad_figuras; i++){
		Figura* figura_actual = this->figuras[i];
		b2Body* body_actual = figura_actual->getBody();
		b2Vec2 posicion = body_actual->GetPosition();
		if (posicion.y >= agua->GetNivel()){
			b2Vec2 velocidad(0, 0);
			b2Vec2 fuerza;
			fuerza.x = 0;
			fuerza.y = -grav.y;
			body_actual->SetLinearVelocity(velocidad);
			body_actual->ApplyForceToCenter(fuerza, true);
		}
	}

	for (int i = 0; i < cantidad_personajes; i++){
		Personaje* personaje_actual = vector_personajes[i];
		b2Body* body_actual = personaje_actual->getBody();
		b2Vec2 posicion = body_actual->GetPosition();
		if (posicion.y >= agua->GetNivel()){
			b2Vec2 velocidad(0, 0);
			b2Vec2 fuerza;
			fuerza.x = 0;
			fuerza.y = -grav.y /100 ;
			body_actual->SetLinearVelocity(velocidad);
			body_actual->ApplyForceToCenter(fuerza, true);
			personaje_actual->setMuerto();
		}
	}
}

void Mundo::dormir_figuras(Figura** figuras){
	for (size_t i = 0; i < cantidad_figuras; i++){
		b2Body* body_actual = figuras[i]->getBody();
		body_actual->SetAwake(false);
	}
}

void Mundo::despertar_figuras(Figura** figuras){
		for (size_t i = 0; i < cantidad_figuras; i++){
			b2Body* body_actual = figuras[i]->getBody();
			body_actual->SetAwake(true);
	}
}

size_t Mundo::GetCantidadFiguras(){
	return cantidad_figuras;
}

bool Mundo::checkOverlap(Figura* figura_ins, Escalador *escalador,size_t i){
	b2Shape *forma_ins, *forma_world;
	int index_ins, index_world;
	b2Transform transf_ins, transf_world;

	forma_ins = figura_ins->getBody()->GetFixtureList()->GetShape();
	index_ins = 0;
	transf_ins = figura_ins->getBody()->GetTransform();

	for (b2Body* body_actual = my_world->GetBodyList()->GetNext(); body_actual; body_actual = body_actual->GetNext()){
		forma_world = body_actual->GetFixtureList()->GetShape();
		index_world = 0;
		transf_world = body_actual->GetTransform();
		bool overlap = b2TestOverlap(forma_ins, index_ins, forma_world, index_world, transf_ins, transf_world);
		if(overlap){
			loguear();
			logFile <<"    Warning " <<"\t Figura número: "<<i<<" se superpone con otra en el mundo. Se destruye" << endl;
			return false;
		}

		if(forma_world->GetType() == forma_world->e_chain){
			for(int32 index_chain = 0; index_chain < forma_world->GetChildCount(); index_chain++){
				overlap = b2TestOverlap(forma_ins, index_ins, forma_world, index_chain, transf_ins, transf_world);
				if(overlap){
					loguear();
					logFile << "    Warning " <<"\t Figura número: "<<i<<" se superpone con la tierra. Se destruye" << endl;
					return false;
				}
			}
			b2ChainShape *chain_world = ((b2ChainShape*) forma_world);
			b2Vec2* vector_tierra = chain_world->m_vertices;
			int32 max_vertx = chain_world->GetChildCount();

			b2Vec2 pos_ins = figura_ins->getBody()->GetPosition();
			if( pos_ins.x < 0 ){
				if(pos_ins.y > vector_tierra[0].y) {
					loguear();
					logFile << "    Warning " <<"\t Figura número: "<<i<<" se inicializa parcialmente en tierra. Se destruye. " << endl;
					return false;
				}
			} else{
				if( pos_ins.x > ((escalador->getEscalaX()) - 1) ){
					loguear();
					logFile << "    Warning " <<"\t Figura número: "<<i<<" se inicializa parcialmente en tierra. Se destruye. " << endl;
					if(pos_ins.y > vector_tierra[max_vertx].y) {

						return false;
					}
				} else{
					b2Vec2 *indice_tierra = escalador->pixelarPosicion(pos_ins);
					if(pos_ins.y > vector_tierra[(int)(indice_tierra->x)].y) {
						loguear();
						logFile <<"    Warning " <<"\t Figura número: "<<i<<" se inicializa en tierra. Se destruye."<< endl;
						delete indice_tierra;
						return false;
					}
					delete indice_tierra;
				}
			}
		}
	}
	return true;
}

bool Mundo::destruir_cuerpo(b2Body* cuerpo){
	if(cuerpo != NULL){
		my_world->DestroyBody(cuerpo);
		return true;
	}
	return false;
}


int Mundo::GetCantVerticesTierra(){
	return cantidad_vertices_tierra;
}


b2Vec2* Mundo::GetVerticesTierra(){
	return vertices_tierra;
}
b2Vec2 Mundo::GetEscalas(){
	return escalas;
}


void Mundo::setFiguras(Figura** vect_figuras, int cant_figuras){
	figuras = vect_figuras;
	cantidad_figuras = cant_figuras;
}

void Mundo::setVectorPersonajes(Personaje** vector, int cant_personajes, int cant_jugadores){
	cantidad_personajes = cant_personajes;
	cantidad_jugadores = cant_jugadores;
	vector_personajes = vector;
}

Agua* Mundo::getAgua(){
	return agua;
}

Personaje** Mundo::getPersonajes(){
	return this->vector_personajes;
}

int Mundo::getCantidadPersonajes(){
	return this->cantidad_personajes;
}
Figura** Mundo::getFiguras(){
	return this->figuras;
}

int Mundo::getCantidadFiguras(){
	return (int) this->cantidad_figuras;
}
