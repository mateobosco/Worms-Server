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
	size_t cantidad = pixelesX/10;
	b2Vec2* vs = new b2Vec2[cantidad+3];
	size_t i;
	vs[0].Set(-10, 200);
	float32 aux = 0;
	for(i = 0; i < cantidad; i++){
		vs[i+1].Set(tierra[i*10].x ,tierra[i*10].y);
		aux = tierra[i].x;
	}
	vs[cantidad+1].Set(aux + 100, 200);
	vs[cantidad+2].Set(-10,200);
	b2Body* body;
	b2BodyDef bodyDef = b2BodyDef();
	body = my_world->CreateBody(&bodyDef);

	b2ChainShape* shape = new b2ChainShape;
	shape->CreateChain(vs, cantidad+3);

	b2FixtureDef fd;
	fd.shape = shape;

	body->CreateFixture(&fd);
	this->vertices_tierra = vs;
	this->cantidad_vertices_tierra = cantidad+3;
	delete[] tierra;
	this->shapeTierra = shape;
	//delete[] vs;
}


void Mundo::crearEdge(b2Vec2* tierra, float32 pixelesX){
	size_t cantidad = pixelesX;
	b2Vec2* vs = new b2Vec2[cantidad+2];
	size_t i;
	vs[0].Set(-10, 200);
	float32 aux = 0;

	b2Vec2 primero = b2Vec2(-10,200);

	b2Body* body;
	b2BodyDef bodyDef = b2BodyDef();
	body = my_world->CreateBody(&bodyDef);

	b2EdgeShape shape;
	shape.Set(primero,tierra[0]);

	b2FixtureDef fd;
	fd.shape = &shape;

	body->CreateFixture(&fd);
	for(i = 0; i < cantidad -1; i++){


		b2Body* body;
		b2BodyDef bodyDef = b2BodyDef();
		body = my_world->CreateBody(&bodyDef);

		b2EdgeShape shape;
		shape.Set(tierra[i],tierra[i+1]);

		b2FixtureDef fd;
		fd.shape = &shape;

		body->CreateFixture(&fd);
		aux = tierra[i].x;
	}
//	b2Body* body;
//	b2BodyDef bodyDef = b2BodyDef();
	body = my_world->CreateBody(&bodyDef);

//	b2EdgeShape shape;
	b2Vec2 ultimo = b2Vec2(aux+100,200);
	shape.Set(tierra[i-1],ultimo);

//	b2FixtureDef fd;
	fd.shape = &shape;

	body->CreateFixture(&fd);

	this->vertices_tierra = vs;
	this->cantidad_vertices_tierra = cantidad+2;
	delete[] tierra;
}

void Mundo::CrearTierraPoligono(b2Vec2* tierra, float32 pixelesX,Escalador* escalador){
	//El vector tierra tiene todos los puntos mas altos de la mascara.
	size_t cantidad = pixelesX;
	b2Vec2 vs[(int)escalador->getEscalaX()];
	size_t i;
	int saltoHori = (pixelesX/ escalador->getEscalaX())/* + (pixelesX / escalador->getPixelX())*/; //puede ser float


	for (int k = 0; k<escalador->getEscalaX();k++){
		vs[k] = tierra[k*(saltoHori)];
	}

	for(i = 0; i < escalador->getEscalaX() ; i++){
		float32 inicio = vs[i].y;
		int j;
		b2Vec2 sumaPix;
		//float32 iEscalado = i / (pixelesX / escalador->getEscalaX());
		for(j = escalador->getEscalaY() ; j> inicio+1; j--){
			b2Body* body;
			b2BodyDef bodyDef = b2BodyDef();
			sumaPix = b2Vec2(i,j);
			bodyDef.position = sumaPix;
			bodyDef.type = b2_staticBody;
			body = my_world->CreateBody(&bodyDef);

			b2PolygonShape shape;
			shape.m_count = 4;
			b2Vec2 vertex[4];
			vertex[0] = b2Vec2(0 , 0);
			vertex[1] = b2Vec2(0 , -1);
			vertex[2] = b2Vec2(-1 , 0);
			vertex[3] = b2Vec2(-1 , -1);
			shape.Set(vertex , 4);

			b2FixtureDef fd;
			fd.shape = &shape;
			body->CreateFixture(&fd);
		}

//		b2Body* body;
//		b2BodyDef bodyDef = b2BodyDef();
//		bodyDef.position = b2Vec2(i,j) ;
//
////		printf("IMPRIMO LA POSICIONES (%f,%f) \n", bodyDef.position.x,bodyDef.position.y);
//
//		bodyDef.type = b2_staticBody;
//		body = my_world->CreateBody(&bodyDef);
//
//		b2PolygonShape shape;
//		shape.m_count = 4;
//		b2Vec2 vertex[4];
//		vertex[0] = b2Vec2(0 , 0);
//		vertex[1] = b2Vec2(1 , 0);
//		vertex[2] = b2Vec2(1 ,body->GetLocalPoint(vs[i+1]).y);
//		vertex[3] = b2Vec2(0 ,body->GetLocalPoint(vs[i]).y);
//		shape.Set(vertex , 4);
//
//		b2FixtureDef fd;
//		fd.shape = &shape;
//		body->CreateFixture(&fd);
	}

	this->vertices_tierra = vs;
	this->cantidad_vertices_tierra = cantidad+2;
	delete[] tierra;

}

void Mundo::CrearTierraPoligono1(b2Vec2* tierra, float32 pixelesX,Escalador* escalador){ // ESTE ES EL QUE ANDAAAAAAAAAA
	//El vector tierra tiene todos los puntos mas altos de la mascara.
	size_t cantidad = pixelesX;
	b2Vec2* vs = new b2Vec2[cantidad+2];
	size_t i;
	int saltoHori = pixelesX / escalador->getEscalaX();

	for(i = 0; i < cantidad - 60*saltoHori ; i+=saltoHori){
		float32 inicio = tierra[i].y;
		int j;
		b2Vec2 sumaPix;
		float32 iEscalado = i / (pixelesX / escalador->getEscalaX());
		for(j = escalador->getEscalaY() ; j> inicio+1; j--){
			b2Body* body;
			b2BodyDef bodyDef = b2BodyDef();
			sumaPix = b2Vec2(iEscalado,j);
			bodyDef.position = sumaPix;
			bodyDef.type = b2_staticBody;
			body = my_world->CreateBody(&bodyDef);

			b2PolygonShape shape;
			shape.m_count = 4;
			b2Vec2 vertex[4];
			vertex[0] = b2Vec2(0 , 0);
			vertex[1] = b2Vec2(0 , -1);
			vertex[2] = b2Vec2(-1 , 0);
			vertex[3] = b2Vec2(-1 , -1);
			shape.Set(vertex , 4);

			b2FixtureDef fd;
			fd.shape = &shape;
			body->CreateFixture(&fd);
		}

		b2Body* body;
		b2BodyDef bodyDef = b2BodyDef();
		bodyDef.position = b2Vec2(iEscalado,j) ;

		printf("IMPRIMO LA POSICIONES (%f,%f) \n", bodyDef.position.x,bodyDef.position.y);

		bodyDef.type = b2_staticBody;
		body = my_world->CreateBody(&bodyDef);

		b2PolygonShape shape;
		shape.m_count = 4;
		b2Vec2 vertex[4];
		vertex[0] = b2Vec2(0 , 0);
		vertex[1] = b2Vec2(1 , 0);
		vertex[2] = body->GetLocalPoint(tierra[i+saltoHori]);
		vertex[3] = body->GetLocalPoint(tierra[i]);
		shape.Set(vertex , 4);

		b2FixtureDef fd;
		fd.shape = &shape;
		body->CreateFixture(&fd);
	}

	this->vertices_tierra = vs;
	this->cantidad_vertices_tierra = cantidad+2;
	delete[] tierra;

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

b2ChainShape* Mundo::getChainShape(){
	return shapeTierra;
}
