#include "Personaje.h"

int random(int n){
    static std::mt19937 generator(std::time(0));
    std::uniform_int_distribution<> distribution(0, n-1);
    return distribution(generator);
}

Personaje::Personaje(Mundo* mundo, Uint8 numero_jugador, char* nombre_client) {
	angulo_arma = 0;
	conectado = 1;
	arma_seleccionada =0;
	energia = ENERGIA_WORM;
	nombre_cliente = nombre_client;
	muerto = false;
	shape = NULL;
	radio = 0;
	dir_imagen = "TPTaller/imagenes/gusanitoderecha.png";
	nro_jugador = numero_jugador;
	b2Vec2 escalas = mundo->GetEscalas();
	alto = escalas.y / 50;
	ancho = escalas.x / 60;
	b2Vec2* vertices_tierra = mundo->GetVerticesTierra();
	bool posicion_correcta = false;
	b2Vec2 posicion;
	//b2Vec2 aux=b2Vec2(10, 10);
	while (!posicion_correcta){
		posicion.x+=10;
		//aux+=10;
		int indice = random( (mundo->GetCantVerticesTierra()));
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

	shape2 = new b2CircleShape();
	shape2->m_radius = ancho/2;
	b2Filter filtro = b2Filter();
	filtro.groupIndex = INDICE_GRUPO;

	b2FixtureDef fd; // creo un fixture
	fd.filter = filtro;
	fd.restitution = 0;
	fd.friction = 0.5;
	fd.shape = shape2;
	body->CreateFixture(&fd); // al body le pongo la fixture creada
	body->SetAwake(false);
	for (int i = 0 ; i<4; i++){
		seleccionado[i] = false;
	}
	orientacion = 1;
	daniado_turno_actual = false;
	this->movio = 0;
	this->salto = 0;
}

Personaje::~Personaje() {
	b2World* mundo = body->GetWorld();
	mundo->DestroyBody(body);
}

void Personaje::mover(b2Vec2 direccion){
	float32 modulo = 0.20;
	b2Vec2 velocidad = {0, 0};
	this->getBody()->SetLinearVelocity(velocidad);
	b2Vec2 fuerza = b2Vec2(modulo * direccion.x, modulo* direccion.y ); // TODO ver cuanto aplicarle
	if (direccion.x > 0) orientacion = 1;
	if (direccion.x < 0) orientacion = -1;
	body->ApplyLinearImpulse(fuerza, body->GetWorldCenter(), true );
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
	return vector_y;
}



void Personaje::leermovimiento(int direccion, int id_jugador){
	if (this->nro_jugador == id_jugador && seleccionado[id_jugador]){
		if (direccion == 3 && body->GetLinearVelocity().x < 0.7){ // para la derecha
			dir_imagen = "TPTaller/imagenes/gusanitoderecha.png";
			orientacion=1;
			this->movio = 1;
			this->mover(b2Vec2(2,0));
			return;
		}
		if (direccion == 1 && body->GetLinearVelocity().x > -0.7 ){ // para la izquierda
			dir_imagen = "TPTaller/imagenes/gusanitoizquierda.png";
			orientacion=-1;
			this->movio = 1;
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
		if((direccion == 2  && resultado) || ((direccion == 2) && (body->GetContactList() != NULL))){ // para arriba
			this->mover(b2Vec2(0,-3));
			this->salto = 1;
			return;
		}
		if ( (direccion == 4  && body->GetLinearVelocity().x <0.7 ) && (body->GetContactList() != NULL) ){ // para arriba a la derecha
			this->mover(b2Vec2(2,-3));
			this->salto = 1;
//			if(body->GetLinearVelocity().y == 0)
//				this->mover(b2Vec2(2,-3));
//			else
//				this->mover(b2Vec2(1,-1));
//					return;
		}
		if ( (direccion == 5 && body->GetLinearVelocity().x >-0.7 ) && (body->GetContactList() != NULL)){ // para arriba a la izq
			this->mover(b2Vec2(-2,-3));
			this->salto = 1;
//			if(body->GetLinearVelocity().y == 0)
//				this->mover(b2Vec2(-2,-3));
//			else
//				this->mover(b2Vec2(-1,-1));
//				return;

		}
		if((direccion == 5) && (body->GetContactList() != NULL)){ // para arriba a la izq
			this->mover(b2Vec2(-2,-3));
			this->salto = 1;

		}
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
	energia = 0;
	muerto = true;
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

int Personaje::getEnergia(){
	return energia;
}

void Personaje::setArmaSeleccionada(int nro_arma){
	this->arma_seleccionada = nro_arma;

}

int Personaje::getArmaSeleccionada(){
	return this->arma_seleccionada;
}


int Personaje::getAnguloArma(){
	return this->angulo_arma;
}

void Personaje::setAnguloArma(int angulo){
	if((angulo_arma == -90) && (angulo < 0)){
		return;
	}
	if((angulo_arma == 90) && (angulo > 0)){
		return;
	}
	this->angulo_arma += angulo;
}

void Personaje::quitarEnergia(int descontar){
	energia -= descontar;
	if(energia <= 0){
		energia = 0;
		this->setMuerto();
	}
}

bool Personaje::getDaniadoTurnoActual(){
	return daniado_turno_actual;
}

void Personaje::setDaniadoTurnoActual(bool estado){
	daniado_turno_actual = estado;
}

int Personaje::getSalto(){
	return this->salto;
}

int Personaje::getMovio(){
	return this->movio;
}

void Personaje::setSalto(int estado){
	this->salto = estado;
}

void Personaje::setMovio(int estado){
	this->movio = estado;
}
