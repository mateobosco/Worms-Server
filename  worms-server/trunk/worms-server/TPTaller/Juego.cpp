#include "Juego.h"



#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/geometries/register/ring.hpp>

Juego::Juego(){
	mundo = NULL;
	cantidad_jugadores = 0;
	escalador = NULL;
	lector = NULL;
	figuras = NULL;
	cantidad_figuras = 0;
	cielo.clear();
	agua = NULL;
	inicial = NULL;
	viento = 0;
	this->cargar();
	manejador = new ManejadorPersonajes();
	for (int i =0; i< 4; i++){
		jugadores[i]=NULL;
	}
	jugador_actual = 0;
	reloj_ronda = 0;
	indice_jugador_turno = 0;
	arma_actual = 0;
	proj_in_air = false;
}

Juego::~Juego(){
	for(size_t i= 0; i < cantidad_figuras ; i++){
		if (figuras != NULL){
			if(figuras[i] != NULL){
				delete figuras[i];
			}
		}
	}
	delete[] figuras;
	delete agua;
	delete lector;
	delete mundo;
	delete manejador;
	delete escalador;
	for(Uint8 i = 0; i < cantidad_jugadores; i++){
		if(jugadores[i] != NULL){
			delete jugadores[i];
		}
	}
	if(arma_actual){
		delete arma_actual;
	}
}

Jugador** Juego::getJugadores(){
	return jugadores;
}

uint8 Juego::getCantidadJugadores(){
	return this->cantidad_jugadores;
}

Mundo* Juego::getMundo(){
	return mundo;
}

Figura** Juego::getFiguras(){
	return figuras;
}

LectorMascara* Juego::getLector(){
	return lector;
}

Agua* Juego::getAgua(){
	return agua;
}

Escalador* Juego::getEscalador(){
	return escalador;
}

string Juego::getCielo() {
	return cielo;
}

uint8 Juego::getCantidadFiguras(){
	return cantidad_figuras;
}

void Juego::cargar() {
	this->abrirLog();
	Cargador *cargador = new Cargador(pathEscTest.c_str());
	Node *nodo_escenario = this->cargaInicial(cargador);
	this->cargaPrincipal(cargador, *nodo_escenario);
	this->viento = cargador->getViento();
	inicial = cargador->getPaqueteInicial();
	delete nodo_escenario;
	delete cargador;
}

// Funciones Privadas:

void Juego::abrirLog(){
	logFile.open("log.txt",ios::out);
	logFile << "Log File. Ejecución Worms" << endl;
	logFile << "   Fecha  | Hora |  Gravedad  | \t Mensaje " << endl;
}

Node* Juego::cargaInicial(Cargador* cargador){
	Node *nodo_escenario =  new Node();
	if(!((cargador->getNodo(cargador->getNodo(),*nodo_escenario,"esc"))||(cargador->getNodo(cargador->getNodo(),*nodo_escenario,"escenario"))||(cargador->getNodo(cargador->getNodo(),*nodo_escenario,"esce"))||(cargador->getNodo(cargador->getNodo(),*nodo_escenario,"escen"))||
			(cargador->getNodo(cargador->getNodo(),*nodo_escenario,"escena"))||(cargador->getNodo(cargador->getNodo(),*nodo_escenario,"escenar"))||(cargador->getNodo(cargador->getNodo(),*nodo_escenario,"escenari"))||
			(cargador->getNodo(cargador->getNodo(),*nodo_escenario,"Esc"))||(cargador->getNodo(cargador->getNodo(),*nodo_escenario,"ESC"))||(cargador->getNodo(cargador->getNodo(),*nodo_escenario,"ESCENARIO"))||
			(cargador->getNodo(cargador->getNodo(),*nodo_escenario,"Escenario"))||(cargador->getNodo(cargador->getNodo(),*nodo_escenario,"Esc")))){
		loguear();
		logFile << "    Error   " << "\t No se encuentra el escenario. Se carga escenario por defecto."<<endl;
		delete cargador;
		cargador = new Cargador(pathDefEs.c_str());
		(*nodo_escenario) = cargador->getNodo()["escenario"];
	}
	return nodo_escenario;
}

void Juego::cargarEscalador(Cargador *cargador, Node nodo_escenario){
	this->escalador = cargador->loadEscalador(nodo_escenario);
	if(!this->escalador){
		loguear();
		logFile << "    Error   " << "\t  No se pudo escalar la información "<< endl;
	}
}

void Juego::cargarAgua(Cargador *cargador, Node nodo_escenario){
	string imagen_agua = pathAgua;
	this->agua = cargador->loadAgua(nodo_escenario, imagen_agua);
	if(!this->agua){
		loguear();
		logFile << "    Error   " << "\t  No se pudo crear el agua. "<< endl;
	}
}

void Juego::cargarMundo(){
	b2Vec2 escalas = b2Vec2(this->escalador->getEscalaX(), this->escalador->getEscalaY());
	this->mundo = new Mundo(b2Vec2(GRAVEDAD_X,GRAVEDAD_Y), this->agua, escalas);
	if(!this->mundo){
		loguear();
		logFile << "    Error   " << "\t  No se pudo crear el mundo."<< endl;
	}
}

string Juego::cargarTierra(Cargador *cargador, Node nodo_escenario){
	string tierra;
	if(!((cargador->loadPath(nodo_escenario, "imagen_tierra",tierra))||(cargador->loadPath(nodo_escenario, "tierra",tierra))||(cargador->loadPath(nodo_escenario, "imagen-tierra",tierra)))){
		loguear();
		logFile << "    Error  " <<"\t No se pudo cargar el path correspondiente a la clave buscada. " << endl;
		tierra = pathDefMas;
	}
	return tierra;
}

void Juego::cargarCielo(Cargador *cargador, Node nodo_escenario){
	string cielo;
	if(!((cargador->loadPath(nodo_escenario, "imagen_cielo",cielo))||(cargador->loadPath(nodo_escenario, "cielo",cielo))||(cargador->loadPath(nodo_escenario, "imagen-cielo",cielo)))){
		loguear();
		logFile << "    Error  " <<"\t No se pudo cargar el path correspondiente a la clave buscada. " << endl;
		cielo = pathDefCielo;
	}
}

void Juego::cargarLector(string tierra){
	this->lector = new LectorMascara(tierra);
	if(!this->lector){
		loguear();
		logFile << "    Error   " << "\t  No se pudo crear el L ector de Máscara. " <<  SDL_GetError()<< endl;
	}
}

void Juego::generarTierra(){
	b2Vec2* vectorTierra = this->lector->LeerMascara(this->escalador);
	this->mundo->Crear_Chains(vectorTierra, this->lector->GetPixelAncho());
//	this->mundo->crearEdge(vectorTierra, this->lector->GetPixelAncho());
//	this->mundo->CrearTierraPoligono(vectorTierra, this->lector->GetPixelAncho(),this->escalador);
}

void Juego::cargarFiguras(Cargador *cargador, Node nodo_escenario){
	Node objetos;
	if((cargador->getNodo(nodo_escenario,objetos,"objetos"))||(cargador->getNodo(nodo_escenario,objetos,"obj"))||
			(cargador->getNodo(nodo_escenario,objetos,"o"))||(cargador->getNodo(nodo_escenario,objetos,"Objetos"))||(cargador->getNodo(nodo_escenario,objetos,"OBJETOS"))){
		this->cantidad_figuras = objetos.size();
		this->figuras = new Figura*[this->cantidad_figuras];
		for(size_t i = 0; i < this->cantidad_figuras; i++){
			this->figuras[i] = cargador->cargarFigura(objetos[i], this->mundo, this->escalador, i + 1);
		}
	}else{
		loguear();
		logFile << "    Warning " << "\t  No hay objetos para cargar "<< endl;
	}
}

//Retorna el mundo cargado y si hubo algún error terminante retorna NULL.
void Juego::cargaPrincipal(Cargador *cargador, Node nodo_escenario){
	cargarEscalador(cargador, nodo_escenario);
	cargarAgua(cargador, nodo_escenario);
	cargarMundo();
	string tierra = cargarTierra(cargador, nodo_escenario);
	cargarCielo(cargador, nodo_escenario);
	cargarLector(tierra);
	cargador->loadViento(nodo_escenario);
	if(this->lector == NULL){
		delete this->mundo;
		this->mundo = NULL;
	}
	generarTierra();
	cargarFiguras(cargador, nodo_escenario);
}

structInicial* Juego::getPaqueteInicial(){
	return this->inicial;
}

void Juego::aplicarPaquete(structEvento* evento, int comenzar){
	if (evento == NULL) return;
//	if (!proj_in_air){ // TODO MODIFICAR PARA ARMAR QUE PERMITEN MOVERSE 3 SEG DESPUES DE DISPARADAS
		if (evento->click_mouse.x != -1){ // recibio un click
			manejador->seleccionarPersonaje(evento->click_mouse, evento->nro_jugador);
		}
		if ((evento->direccion > 0) && (evento->nro_jugador == this->getJugadorActual()) && comenzar ==1){ // PROCESO EL MOVIMIENTO SOLO SI ES SU TURNO
			manejador->moverPersonaje(evento->direccion , evento->nro_jugador);

		}
		if (evento->arma_seleccionada != 0 ){
			for (int j = 0; j < manejador->getCantidadPersonajes(); j++){ // TODO ver si es necesario cant actuales activos.
				Personaje* personaje_actual = manejador->getPersonajes()[j];
				if (! personaje_actual->getMuerto()){
					if( personaje_actual->getSeleccion()[evento->nro_jugador] ){
						printf( " SE LE ASIGNO EL PERSONAJE SELECCIONADO EL ARMA %d \n", evento->arma_seleccionada);
						personaje_actual->setArmaSeleccionada(evento->arma_seleccionada);
						personaje_actual->setAnguloArma(evento->angulo_arma);

						this->setArma(evento->arma_seleccionada, personaje_actual->getPosition(), personaje_actual->getAnguloArma(), evento->direccion );
					}
				}
			}
		}
		if(evento->angulo_arma != 0 && arma_actual != NULL){
			for (int j = 0; j < manejador->getCantidadPersonajes(); j++){ // TODO ver si es necesario cant actuales activos.
				Personaje* personaje_actual = manejador->getPersonajes()[j];
				if (! personaje_actual->getMuerto()){
					if( personaje_actual->getSeleccion()[evento->nro_jugador] ){
						//personaje_actual->setArmaSeleccionada(evento->arma_seleccionada);
						personaje_actual->setAnguloArma(evento->angulo_arma);
						this->arma_actual->setAngulo(personaje_actual->getAnguloArma(), personaje_actual->getOrientacion());
					}
				}
			}
		}
		if((evento->fuerza == 1) && arma_actual!= NULL){
			arma_actual->setFuerza();
		} else
			if((evento->fuerza == 2) && arma_actual != NULL) {
				this->disparar();
				arma_actual->resetFuerza();
			}
//	    if(evento->reset == 1) {
//	    	printf("Reset == 1\n");
//	    	this->resetNivel();
//	    }

//	if(evento->fuerza == 2){
//		printf("Dejó de disparar\n");
//	}
	//else return;
//	}
}

Jugador* Juego::agregarJugador(int id, char* nombre_cliente){
	this->jugadores[id] = new Jugador(mundo,id,manejador, nombre_cliente);
	Personaje** pers = this->jugadores[id]->getPersonajes();
	this->manejador->AgregarJugador(mundo,id, pers);
	this->jugadores_jugando.push_back(this->jugadores[id]);
	return this->jugadores[id];
}

ManejadorPersonajes* Juego::getManejadorPersonajes(){
	return this->manejador;
}




int Juego::getJugadorActual(){
	return indice_jugador_turno;
}

void Juego::pasarTurno(){
	//reloj_ronda = SDL_GetTicks();
	this->resetearRelojRonda();
	//jugador_actual++;
	Jugador* jugador_anterior = this->jugadores[this->jugador_actual];
	jugador_anterior->seleccionarSiguientePersonaje();
	indice_jugador_turno++;
	if(indice_jugador_turno == 1){
		indice_jugador_turno = 0;
	}
	printf(" LLEGA HASTA ACA 1  indice jugador turno es %d \n", indice_jugador_turno);
	Jugador* jugador_actual = jugadores_jugando.at(indice_jugador_turno);
	while(jugador_actual->getConectado() == false){
		//printf(" ENTRA ACAAAAA \n");
		indice_jugador_turno++;

		jugador_actual = jugadores_jugando.at(indice_jugador_turno);
		if (indice_jugador_turno > jugadores_jugando.size()){
			indice_jugador_turno=0;
		}
	}
	//printf(" LLEGA HASTA ACA 2 \n");
	//if(jugador_actual == 2){
	//	jugador_actual =0;
	//}


	if (indice_jugador_turno > jugadores_jugando.size()){
		indice_jugador_turno = 0;
	}
	for(int i = 0; i < jugadores_jugando.size(); i++){
		Jugador* jugador_actual = jugadores_jugando.at(i);
		//Cliente* clienteActual = servidor->getClientes()[i];
		Personaje** vector_personajes = jugador_actual->getPersonajes();
		for (int i = 0; i < 4; i++){
			Personaje* personaje_actual = vector_personajes[i];
			if (personaje_actual->getEnergia() == 0){
				personaje_actual->setMuerto();
			}
		}
		if(jugador_actual->getPerdio()){
			//jugadores_jugando.erase(jugadores_jugando.begin() + i);
		}
	}
}

int Juego::getRelojRonda(){
	return (SDL_GetTicks()-reloj_ronda);
}

void Juego::resetearRelojRonda(){
	reloj_ronda=SDL_GetTicks();
}

void Juego::setArma(int tipo_arma, b2Vec2 posicion, int angulo, int direccion){
	Jugador* jugador_actual;
	if(tipo_arma ==1 ){
		printf(" Creo una Bazooka \n");
		jugador_actual = this->jugadores[this->jugador_actual];
		this->arma_actual = new Bazooka(jugador_actual->getPersonajes()[jugador_actual->getPersonajeSeleccionado()]);
	}
	if(tipo_arma ==2 ){
		printf(" Creo una Granada \n");
		jugador_actual = this->jugadores[this->jugador_actual];
		this->arma_actual = new Granada(jugador_actual->getPersonajes()[jugador_actual->getPersonajeSeleccionado()]);
	}
	if(tipo_arma ==3 ){
		printf(" Creo una Dinamita \n");
		jugador_actual = this->jugadores[this->jugador_actual];
		this->arma_actual = new Dinamita(jugador_actual->getPersonajes()[jugador_actual->getPersonajeSeleccionado()]);
	}
	if(tipo_arma ==4 ){
		printf(" Creo una Granada Holy \n");
		jugador_actual = this->jugadores[this->jugador_actual];
		this->arma_actual = new GranadaHoly(jugador_actual->getPersonajes()[jugador_actual->getPersonajeSeleccionado()]);
	}
	if(tipo_arma ==5 ){
		printf(" Suicida \n");
		jugador_actual = this->jugadores[this->jugador_actual];
		this->arma_actual = new Suicida(jugador_actual->getPersonajes()[jugador_actual->getPersonajeSeleccionado()]);
	}
	if(tipo_arma ==6 ){
		printf(" Patada \n");
		jugador_actual = this->jugadores[this->jugador_actual];
		Personaje* personaje = jugador_actual->getPersonajes()[jugador_actual->getPersonajeSeleccionado()];
		this->arma_actual = new Patada(personaje);
	}
	//this->arma_actual->setTipo(tipo_arma);
	this->arma_actual->setPosicion(posicion);
	this->arma_actual->setAngulo(angulo, direccion);
}

void Juego::setArmaVacia(){
	this->arma_actual->setAngulo(0, 0);
}

void Juego::disparar(){
	if(arma_actual){
		arma_actual->disparar(this->mundo);
		proj_in_air = true;
	} else{
//		printf("No hay arma seleccionada \n");
	}
}

void Juego::checkColisionProyectil(structPaquete* paquete){
	if(proj_in_air){
		this->aplicarViento(arma_actual);
		if(arma_actual->checkImpacto(this->mundo)){
			printf(" ENTROOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO \n");
			proj_in_air = false;
			if (this->arma_actual->getTipo() != 6 ) this->mundo->destruir_cuerpo(arma_actual->getProyectil());
			this->arma_actual->aplicarExplosion(this->manejador);
			manejador->resetDaniadoTurnoActual();
			paquete->radio_explosion=this->arma_actual->getRadioExplosion();
			paquete->posicion_proyectil=arma_actual->getProyectil()->GetPosition();
			printf(" LE MANDO LA POSICION %f, %f \n", paquete->posicion_proyectil.x, paquete->posicion_proyectil.y);
			if (this->arma_actual->getTipo() != 6 ) this->explotarBomba(paquete->posicion_proyectil, paquete->radio_explosion);
			Jugador* jugador_actual=this->jugadores[this->jugador_actual];
			Personaje* personaje_sel = jugador_actual->getPersonajes()[jugador_actual->getPersonajeSeleccionado()];
			personaje_sel->setArmaSeleccionada(0);
			this->pasarTurno();
			if(this->arma_actual){
				delete this->arma_actual;
				this->arma_actual = NULL;
			}
		}
	} else{
		paquete->radio_explosion=-1;
	}
}

void Juego::setPaqueteProyectil(structPaquete *pack){
	pack->show_proyectil = this->proj_in_air;
	if (proj_in_air){
		pack->tipo_proyectil = this->arma_actual->getTipo();
		pack->posicion_proyectil = this->arma_actual->getPosicion();
		pack->direccion_proyectil = this->arma_actual->getDireccion();
		pack->tamanio_proyectil = this->arma_actual->getTamanio();
		pack->potencia = arma_actual->getFuerza();
		if((pack->tipo_proyectil == granada) || (pack->tipo_proyectil == dinamita) || (pack->tipo_proyectil == granada_holy)){
			pack->contador_segundos = this->arma_actual->getContadorSegundos();
			pack->angulo = this->arma_actual->getAngulo();
		}else{
			pack->contador_segundos = -1;
			pack->angulo = 0;
		}
	} else{
		pack->tipo_proyectil = 0;
		pack->posicion_proyectil = b2Vec2(0,0);
		pack->direccion_proyectil = b2Vec2(0,0);
		pack->tamanio_proyectil = b2Vec2(0,0);

	}
}


 class MyQueryCallback : public b2QueryCallback {
  public:
      std::vector<b2Body*> foundBodies;

      bool ReportFixture(b2Fixture* fixture) {
    	  b2Body* body = fixture->GetBody();
    	  b2Shape* shape= fixture->GetShape();

    	  if ((shape->GetType() == 2) && (body->GetType()==0)){ // b2PoligonShape == 2
    		  foundBodies.push_back( body );
    	  }
		  if ((shape->GetType() == 1)){ // b2EdgeShape == 2
			  foundBodies.push_back( body );
		  }
          return true;//keep going to find all fixtures in the query area
      }
  };


 std::vector<b2Body*> queryDestructibleBodies(b2Vec2 position, float radius, b2World* world){
	MyQueryCallback query;
	b2AABB aabb;
	aabb.lowerBound = { position.x - radius, position.y - radius };
	aabb.upperBound = { position.x + radius, position.y + radius };

	world->QueryAABB(&query, aabb);

	return query.foundBodies;
 }

 BOOST_GEOMETRY_REGISTER_POINT_2D(b2Vec2, float32, boost::geometry::cs::cartesian, x, y/*, z*/)

 typedef boost::geometry::model::polygon<b2Vec2> Polygon;
 BOOST_GEOMETRY_REGISTER_RING(Polygon::ring_type)

 class QueryChainDestruir : public b2QueryCallback {
  public:
      std::vector<b2Fixture*> foundFix;
      bool ReportFixture(b2Fixture* fixture) {
    	  b2Shape* shape = fixture->GetShape();
    	  if (shape->GetType() == 3){ // b2ChainShape == 3
    		  vector<b2Fixture*>::iterator it = std::find(foundFix.begin(), foundFix.end(), fixture);
    		  if(it==foundFix.end()){
    			  foundFix.push_back( fixture );
    		  }
    	  }
          return true;//keep going to find all fixtures in the query area
      }
  };



 void Juego::explotarBomba(b2Vec2 posicion, float32 radio){

	QueryChainDestruir query;
	b2AABB aabb;
	aabb.upperBound = b2Vec2(escalador->getEscalaX(),escalador->getEscalaY());
	aabb.lowerBound = b2Vec2(0,0);

	b2World* world = this->getMundo()->devolver_world();
	world->QueryAABB(&query, aabb);

	std::vector<b2Fixture*> chainFixtures = query.foundFix;

	if (chainFixtures.size()==0) return;
	for (size_t t=0 ; t<chainFixtures.size();t++){

		b2Fixture* fixtureList = chainFixtures[t];
		b2Shape* shape = fixtureList->GetShape();
		b2ChainShape* shapeOriginal = (b2ChainShape*) shape;
		b2Vec2* verticesOriginal = shapeOriginal->m_vertices;
		int cantidadOriginal = shapeOriginal->m_count;

		int cantidadExplosion = 15;
		b2Vec2 verticesExplosion[15];
		int anguloInicial = 360/cantidadExplosion;
		int angulo = anguloInicial - 90;
		for (int j = 0 ; j<cantidadExplosion ; j ++ ){
			b2Vec2 punto;
			punto.x = radio * cos( angulo * PI / 180 );
			punto.y = radio * sin( angulo * PI / 180 );
			angulo = angulo + anguloInicial;
			verticesExplosion[j] = punto+posicion;
		}

		using boost::geometry::append;
		using boost::geometry::correct;
		using boost::geometry::dsv;
		Polygon poliOriginal;
		Polygon poliExplosion;

		for (int j=0; j<cantidadExplosion; j++){
			b2Vec2 verExplosion = verticesExplosion[j];
			append(poliExplosion, verExplosion);
		}

		bool incluido = true;

		for (int i=0; i<cantidadOriginal ; i++){
			b2Vec2 verOriginal = verticesOriginal[i];
			append(poliOriginal, verOriginal);
			if (!(boost::geometry::within(verOriginal,poliExplosion))){
				incluido = false;
			}
		}

		correct(poliOriginal);
		correct(poliExplosion);

		if (incluido){
			b2Body* bodyDestruir = chainFixtures[t]->GetBody();
			this->getMundo()->devolver_world()->DestroyBody(bodyDestruir);
			continue;
		}

		std::vector<Polygon> poliNuevo;
		boost::geometry::difference(poliOriginal, poliExplosion, poliNuevo);
		if (poliNuevo.size() == 0 ) return;
		b2Body* bodyDestruir = chainFixtures[t]->GetBody();
		this->getMundo()->devolver_world()->DestroyBody(bodyDestruir);

		for (size_t y=0; y<poliNuevo.size();y++){
			Polygon inter = poliNuevo[y];
			correct(inter);
			std::vector<b2Vec2> const& puntos = inter.outer();
			size_t cantidadNuevo = puntos.size();
			b2Vec2* verticesNuevo = new b2Vec2[cantidadNuevo];
			verticesNuevo[0] = puntos[0];
			for (std::vector<b2Vec2>::size_type k = 1; k < (size_t) cantidadNuevo; k++){
				verticesNuevo[k] = puntos[k];
				b2Vec2 v1 = puntos[k-1];
				b2Vec2 v2 = puntos[k];
				if (b2DistanceSquared(v1, v2) < 0.005*0.005){
					verticesNuevo[k] = verticesNuevo[k] + b2Vec2(0,0.1);
				}
			}
			b2Body* body;
			b2BodyDef bodyDef = b2BodyDef();
			body = this->getMundo()->devolver_world()->CreateBody(&bodyDef);
			b2ChainShape shape;
			shape.CreateChain(verticesNuevo, cantidadNuevo);

			b2FixtureDef fd;
			fd.shape = &shape;
			body->CreateFixture(&fd);
			delete[] verticesNuevo;
		}
	}
    return;
}

void Juego::resetNivel(){
	this->mundo->resetMundo();
	delete this->arma_actual;
	this->arma_actual = NULL;
	for(int i = 0; i < this->cantidad_figuras ; i++){
	this->figuras[i] = NULL;
	}
	this->cantidad_figuras = 0;
	this->cantidad_jugadores = 0;
	this->indice_jugador_turno = 0;
	this->jugador_actual = 0;
	this->reloj_ronda=0;
	this->arma_actual = 0;
	this->proj_in_air = false;
	this->manejador->resetManejador();
	for (int i =0; i< 4; i++){
		this->jugadores[i]=NULL;
	}
}

Arma* Juego::getArmaActual(){
	return arma_actual;
}

void Juego::cargarSiguienteNivel(){
//Volver a enviar structInicial con la nueva info
}

int* Juego::jugadoresActivos(){
	 int i;
	 int vivo = 0; //1 Vivo / 0 muerto
	 int jugadores_vivos[4];
	 for(i = 0; i < this->cantidad_jugadores ;i++){
		 Jugador* jugador_actual = this->jugadores[i];
		 int j;
		 for(j = 0; j < 4 ; j++){
			 Personaje* personaje_actual = jugador_actual->getPersonajes()[j];
			 if(!personaje_actual->getMuerto()){ //si esta vivo setea en 1
				 vivo = 1;
			 }
		 }
		 if(vivo == 1){
			 jugadores_vivos[i] = 1;
		 }else{
			 jugadores_vivos[i] = 0;
		 }

	 }
	 return jugadores_vivos;
 }


class QueryViento : public b2QueryCallback {
	public:
    	bool tocando;


    	bool ReportFixture(b2Fixture* fixture) {
    		tocando = true;
    		return true;//keep going to find all fixtures in the query area
    	}
 };



void Juego::aplicarViento(Arma *arma){
	QueryViento query;
	b2AABB aabb;
	b2Body *proyectil = arma->getProyectil();
	int radio = arma->getRadioExplosion();
	aabb.upperBound = proyectil->GetPosition() - b2Vec2(radio, radio);
	aabb.lowerBound = proyectil->GetPosition() + b2Vec2(radio, radio);
	b2World* world = this->getMundo()->devolver_world();
	world->QueryAABB(&query, aabb);
	b2Vec2 fuerza = {this->viento, 0};
	if(!query.tocando)
		proyectil->ApplyForceToCenter(fuerza, true);
}

