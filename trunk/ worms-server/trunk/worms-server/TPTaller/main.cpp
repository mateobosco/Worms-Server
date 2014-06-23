#include "main_server.h"
#include "main_client.h"



int mainPrueba(int argc,char* argv[]){
	int retorno = 0;

	Juego *juego = new Juego();
	ManejadorPersonajes* manejador_personajes = juego->getManejadorPersonajes();
	structInicial* paqueteInicial = juego->getPaqueteInicial();
	Escalador* escalador = new Escalador(paqueteInicial->ancho_ventana , paqueteInicial->alto_ventana,
			paqueteInicial->ancho_unidades, paqueteInicial->alto_unidades,
			paqueteInicial->ancho_escenario,paqueteInicial->alto_escenario);

	Dibujador* dibujador = new Dibujador(NULL, escalador );
	juego->getMundo()->setVectorPersonajes(manejador_personajes->getPersonajes(), manejador_personajes->getCantidadPersonajes(), manejador_personajes->getCantidadJugadores());
	juego->getMundo()->setFiguras(juego->getFiguras(), juego->getCantidadFiguras());
	dibujador->init();
	dibujador->iniciarFondo(juego->getAgua(),paqueteInicial->cielo, paqueteInicial->tierra);

	int* posicion_mouse_click = (int*)malloc (sizeof(int)*2);
	memset(posicion_mouse_click,'\0',2);
	int* posicion_mouse_scroll = (int*)malloc (sizeof(int)*3);
	memset(posicion_mouse_scroll,'\0',3);
	int* posicion_mouse_movimiento = (int*)malloc (sizeof(int)*2);
	memset(posicion_mouse_movimiento,'\0',2);
	posicion_mouse_click[0] = -1;
	posicion_mouse_click[1] = -1;
	SDL_Event event;
	Arma* bomba = new Arma();

	while(KEYS[SDLK_ESCAPE] == false){
		posicion_mouse_click[0] = -1;
		posicion_mouse_click[1] = -1;
		keyboard(event, posicion_mouse_movimiento,posicion_mouse_click,posicion_mouse_scroll);
		escalador->moverVentana(posicion_mouse_movimiento);
		escalador->hacerZoom(posicion_mouse_scroll);
		dibujador->dibujarFondo(juego->getAgua(),juego);
		dibujador->dibujarFiguras(juego->getFiguras(),juego->getCantidadFiguras());
		b2Vec2 pos = b2Vec2(posicion_mouse_click[0], posicion_mouse_click[1]);
		if (posicion_mouse_click[0] != -1){
			bomba->setPosicion(escalador->escalarPosicion(pos));
			bomba->disparar(juego->getMundo());
			bomba->setPosicion(escalador->escalarPosicion(pos));
//			bomba->aplicarExplosion();
			juego->explotarBomba(escalador->escalarPosicion(pos), 3);
			dibujador->borrarExplosion(escalador->escalarPosicion(pos), 3);
		}
		juego->getMundo()->setVectorPersonajes(manejador_personajes->getPersonajes(), manejador_personajes->getCantidadPersonajes(), manejador_personajes->getCantidadJugadores());
		juego->getMundo()->setFiguras(juego->getFiguras(), juego->getCantidadFiguras());

		juego->getMundo()->step(0.05,100,100);
//		juego->getMundo()->comprobar_nivel_agua();
		//dibujador->actualizar();
		posicion_mouse_scroll[2] = 0;

	}
	dibujador->close();

	logFile.close();
	delete juego;
	return retorno;

}


int main(int argc,char* argv[]){

	main_server(argc, argv);
	//mainPrueba(argc,argv);
}
