/*#include <yaml-cpp/yaml.h>

#include "Dibujador.h"
#include "Cargador.h"
//#include "Personaje.h"
#include "ManejadorPersonajes.h"
#include "Paquete.h"
#include "Funciones.h"


int main(){
	int retorno = 0;
	logFile.open("log.txt",ios::out);
	logFile << "Log File. Ejecución Worms" << endl;
	logFile << "   Fecha  | Hora |  Gravedad  | \t Mensaje " << endl;

	Cargador* cargador = new Cargador("TPTaller/prueba_escenario.yaml");
	Node nodo_escenario;
	if(!((cargador->getNodo(cargador->getNodo(),nodo_escenario,"esc"))||(cargador->getNodo(cargador->getNodo(),nodo_escenario,"escenario"))||(cargador->getNodo(cargador->getNodo(),nodo_escenario,"esce"))||(cargador->getNodo(cargador->getNodo(),nodo_escenario,"escen"))||
			(cargador->getNodo(cargador->getNodo(),nodo_escenario,"escena"))||(cargador->getNodo(cargador->getNodo(),nodo_escenario,"escenar"))||(cargador->getNodo(cargador->getNodo(),nodo_escenario,"escenari"))||
			(cargador->getNodo(cargador->getNodo(),nodo_escenario,"Esc"))||(cargador->getNodo(cargador->getNodo(),nodo_escenario,"ESC"))||(cargador->getNodo(cargador->getNodo(),nodo_escenario,"ESCENARIO"))||
			(cargador->getNodo(cargador->getNodo(),nodo_escenario,"Escenario"))||(cargador->getNodo(cargador->getNodo(),nodo_escenario,"Esc")))){
		loguear();
		logFile << "    Error   " << "\t No se encuentra el escenario. Se carga escenario por defecto."<<endl;
		delete cargador;
		cargador = new Cargador(pathDefEs.c_str());
		nodo_escenario = cargador->getNodo()["escenario"];

	}

	Escalador* escalador = cargador->loadEscalador(nodo_escenario);
	if(!escalador){
		loguear();
		logFile << "    Error   " << "\t  No se pudo escalar la información "<< endl;
	}
	string imagen_agua = "TPTaller/imagenes/aguaAzul.png";
	Agua* agua = cargador->loadAgua(nodo_escenario,imagen_agua);
	if(!agua){
		loguear();
		logFile << "    Error   " << "\t  No se pudo crear el agua. "<< endl;
	}
	b2Vec2 escalas = b2Vec2(escalador->getEscalaX(), escalador->getEscalaY());
	Mundo* mundo = new Mundo(b2Vec2(0.0,9.8),agua, escalas);
	if(!mundo){
		loguear();
		logFile << "    Error   " << "\t  No se pudo crear el mundo."<< endl;
	}
	string tierra;
	string cielo;
	if(!((cargador->loadPath(nodo_escenario, "imagen_tierra",tierra))||(cargador->loadPath(nodo_escenario, "tierra",tierra))||(cargador->loadPath(nodo_escenario, "imagen-tierra",tierra)))){
		loguear();
		logFile << "    Error  " <<"\t No se pudo cargar el path correspondiente a la clave buscada. " << endl;
		tierra = pathDefMas;
	}
	if(!((cargador->loadPath(nodo_escenario, "imagen_cielo",cielo))||(cargador->loadPath(nodo_escenario, "cielo",cielo))||(cargador->loadPath(nodo_escenario, "imagen-cielo",cielo)))){
		loguear();
		logFile << "    Error  " <<"\t No se pudo cargar el path correspondiente a la clave buscada. " << endl;
		cielo = pathDefCielo;
	}
	if(!init(escalador->getVentanaX(),escalador->getVentanaY())){
		loguear();
		logFile <<"    Error   " << "\t  Inicialización inválida " << endl;
		return -1;
		}

	LectorMascara* lector = new LectorMascara(tierra);
	if(!lector){
		loguear();
		logFile << "    Error   " << "\t  No se pudo crear el Lector de Máscara. " <<  SDL_GetError()<< endl;
		return -1;
	}
	b2Vec2* vectorTierra = lector->LeerMascara(escalador);
		if(!vectorTierra){
			tierra = pathDefMas;
			loguear();
			logFile <<  "    Error   " << "\t No se pudo obtener posiciones de la máscara '"<<lector->getPath()<<"'. Se carga por defecto '"<<tierra<<"'."<< endl;
			lector->setMascaraPorDefecto();
			vectorTierra = lector->LeerMascara(escalador);

		}
	mundo->Crear_Chains(vectorTierra, lector->GetPixelAncho());
	Figura** figuras;
	Node objetos;
	size_t cantidad=0;
	if((cargador->getNodo(nodo_escenario,objetos,"objetos"))||(cargador->getNodo(nodo_escenario,objetos,"obj"))||
			(cargador->getNodo(nodo_escenario,objetos,"o"))||(cargador->getNodo(nodo_escenario,objetos,"Objetos"))||(cargador->getNodo(nodo_escenario,objetos,"OBJETOS"))){
		cantidad= objetos.size();
		figuras = new Figura*[cantidad];

		for(size_t i = 0; i < cantidad; i++){
			figuras[i] = cargador->cargarFigura(objetos[i], mundo, escalador, i+1);

		}
	}else{
		loguear();
		logFile << "    Warning " << "\t  No hay objetos para cargar "<< endl;
	}
	Dibujador dibujador = Dibujador(gRenderer, escalador);
	SDL_Event event;

	for(int i = 0; i < 322; i++) { // inicializa todas en falso
	   KEYS[i] = false;
	}

	int cantidad_jugadores = 1;
	int cantidad_personajes = 4;
	ManejadorPersonajes* manejador_personajes = new ManejadorPersonajes();

	manejador_personajes->AgregarJugador(mundo, 1);
	mundo->setVectorPersonajes(manejador_personajes->getPersonajes(), manejador_personajes->getCantidadPersonajes(), cantidad_jugadores);
	mundo->setFiguras(figuras, cantidad);



	SDL_Color color2;
	Poligono* poligono = new Poligono(mundo, b2Vec2(50,20),4,5,false,color2,4,90);
	Personaje* personaje = new Personaje(mundo,2);




	dibujador.iniciarFondo(agua, cielo, lector->getPath());
	dibujador.dibujarPersonajes(manejador_personajes->getPersonajes(),  manejador_personajes->getCantidadPersonajes());
	dibujador.dibujarFiguras(figuras,cantidad);

	int* posicion_mouse_click = (int*)malloc (sizeof(int)*2);
	int* posicion_mouse_scroll = (int*)malloc (sizeof(int)*2);
	int* posicion_mouse_movimiento = (int*)malloc (sizeof(int)*2);
	posicion_mouse_click[0] = -1;
	posicion_mouse_click[1] = -1;
	KEYS[SDLK_s] = true;
	while(KEYS[SDLK_ESCAPE] == false){
		if (posicion_mouse_click[0]>0 && posicion_mouse_click[1]>0 && posicion_mouse_click[0] < escalador->getVentanaX() && posicion_mouse_click[1] < escalador->getVentanaY()){
			b2Vec2 aux = b2Vec2(posicion_mouse_click[0], posicion_mouse_click[1]);
			b2Vec2 mouse_escalado = escalador->escalarPosicion(aux);
			if (mouse_escalado.x>0 && mouse_escalado.y>0 && mouse_escalado.x < escalador->getEscalaX() && mouse_escalado.y < escalador->getEscalaY()){
				//manejador_personajes->seleccionarPersonaje(mouse_escalado);
			}
		}
		escalador->hacerZoom(posicion_mouse_scroll);
		posicion_mouse_scroll[2]=0;
		escalador->moverVentana(posicion_mouse_movimiento);
		dibujador.dibujarFondo(agua);
		dibujador.dibujarPersonajes(manejador_personajes->getPersonajes(), manejador_personajes->getCantidadPersonajes());

//		structFigura* paquete = crearPaqueteFigura(poligono);
//		structPersonaje* paquete1 = crearPaquetePersonaje(personaje);
//		dibujador.dibujarPaqueteFigura(&paquete);
//		dibujador.dibujarPaquetePersonaje(&paquete1);
		//destruirPaquetePersonaje(paquete1);
		//destruirPaqueteFigura(paquete);



		int id_jugador = 1;
		//manejador_personajes->moverPersonaje(event, KEYS, id_jugador);
		dibujador.dibujarFiguras(figuras,cantidad);
		if(KEYS[SDLK_p]) {
			mundo->dormir_figuras(figuras);
			KEYS[SDLK_p] = false;
			while((KEYS[SDLK_p] == false) && (KEYS[SDLK_ESCAPE] == false) && (KEYS[SDLK_s] == false)){
				keyboard(event, posicion_mouse_movimiento,posicion_mouse_click,posicion_mouse_scroll);
			}
			mundo->despertar_figuras(figuras);
			KEYS[SDLK_p] = false;
		}

		if(KEYS[SDLK_s]){
			size_t z;
			for(z = 0; z < cantidad; z++){
				Figura* figura_actual = figuras[z];
				if (figura_actual != NULL){
					figura_actual->setPosicionInicial();
					figura_actual->getBody()->SetLinearVelocity(b2Vec2(0,0));
					figura_actual->getBody()->SetAngularVelocity(0);
					figura_actual->getBody()->SetAwake(true);
				}
			}
			KEYS[SDLK_s] = false;
			dibujador.dibujarFondo(agua);
			dibujador.dibujarPersonajes(manejador_personajes->getPersonajes(), cantidad_personajes * cantidad_jugadores);
			dibujador.dibujarFiguras(figuras,cantidad);


//			structFigura* paquete = crearPaqueteFigura(poligono);
//			structPersonaje* paquete1 = crearPaquetePersonaje(personaje);
//			dibujador.dibujarPaqueteFigura(&paquete);
//			dibujador.dibujarPaquetePersonaje(&paquete1);
			//destruirPaquetePersonaje(paquete1);
			//destruirPaqueteFigura(paquete);


			while((KEYS[SDLK_s] == false) && (KEYS[SDLK_ESCAPE] == false)){
				keyboard(event, posicion_mouse_movimiento,posicion_mouse_click,posicion_mouse_scroll);
			}
			KEYS[SDLK_s] = false;
		}
		mundo->devolver_world()->Step(0.05,100,100);
		mundo->comprobar_nivel_agua();
		keyboard(event, posicion_mouse_movimiento,posicion_mouse_click,posicion_mouse_scroll);
	}
	for(size_t i= 0; i< cantidad ; i++){
		if (figuras != NULL){
			if( figuras[i] != NULL ){
				delete figuras[i];
			}
		}
	}
	delete agua;
	delete lector;
	delete[] figuras;
	delete mundo;
	delete cargador;
	delete manejador_personajes;
	//delete dibujador; //creo que no hace falta porque no es un puntero y muere al final de la funcion
	free(posicion_mouse_movimiento);
	free(posicion_mouse_scroll);
	free(posicion_mouse_click);
	//close();
	logFile.close();
	return retorno;
}*/
