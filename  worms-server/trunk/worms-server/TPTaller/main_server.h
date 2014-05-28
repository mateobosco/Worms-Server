#ifndef MAIN_SERVER_H_
#define MAIN_SERVER_H_
#include "Juego.h"
#include "Funciones.h"
#include "Servidor.h"

int runServidor(void* serv){
	return ((Servidor*) serv)->runEscucharConexiones();
}

int aceptarConex(void* servidor){
	Servidor* server = (Servidor*) servidor;
	while(!server->getFinalizar()){
		int accept = server->aceptarConexiones();
		if (accept != EXIT_SUCCESS) printf("Error al aceptar\n");
	}
	return 0;
}

int main_server(int argc,char* argv[]){
	int retorno = 0;
	Servidor *servidor = new Servidor(MAX_CANT_JUGADORES);
	printf("Servidor corriendo\n");

	Juego *juego = new Juego();
	ManejadorPersonajes* manejador_personajes = juego->getManejadorPersonajes();
	structInicial* paqueteInicial = juego->getPaqueteInicial();
	servidor->setPaqueteInicial((char*) paqueteInicial);

	SDL_Thread* listener =  SDL_CreateThread(runServidor,"listener",(void*)servidor);
	SDL_Thread* aceptar = SDL_CreateThread(aceptarConex,"aceptar",(void*)servidor);

	servidor->setThreadEscuchar(listener);
	servidor->setThreadAceptar(aceptar);

	if(listener == NULL){
		//ver que hacer
		//log error todo
	}
	int jugadores = servidor->getCantidadClientes();
	while (jugadores<=0){
		jugadores = servidor->getCantidadClientes();
		SDL_Delay(2000);
	}

	printf("-----------------------------------------EL SERVIDOR INICIA EL JUEGO-------------------------------------\n");
	juego->getMundo()->setVectorPersonajes(manejador_personajes->getPersonajes(), manejador_personajes->getCantidadPersonajes(), manejador_personajes->getCantidadJugadores());
	juego->getMundo()->setFiguras(juego->getFiguras(), juego->getCantidadFiguras());

	structPaquete* paqueteCiclo = crearPaqueteCiclo(juego->getMundo(), servidor->getMensajeMostrar());
	structFigura* vector = paqueteCiclo->vector_figuras;
	servidor->actualizarPaquete((char*)paqueteCiclo);
	juego->getMundo()->step(0.1,1,1);

	SDL_Delay(2000);
	//TODO ACA ES DONDE TERMINA EL CODIGO INNECESARIO.

	SDL_mutex *un_mutex = SDL_CreateMutex();
	while(!servidor->getFinalizar()){
		for (int i=0 ; i < servidor->getCantidadClientes() ; i++){
			int* clientes = servidor->getVectorClientes();

			if (clientes[i] != -1 && juego->getJugadores()[i] == NULL){
				SDL_Delay(500);

				Cliente* clienteActual = servidor->getClientes()[i];
				char* nombre = clienteActual->getNombre();
				Jugador* jug = juego->agregarJugador(i, nombre);

				clienteActual->setJugador(jug);

			}
		}
		juego->getMundo()->setVectorPersonajes(manejador_personajes->getPersonajes(), manejador_personajes->getCantidadPersonajes(), manejador_personajes->getCantidadJugadores());
		juego->getMundo()->setFiguras(juego->getFiguras(), juego->getCantidadFiguras());
		structPaquete* paqueteCiclo = crearPaqueteCiclo(juego->getMundo(), servidor->getMensajeMostrar());

		servidor->actualizarPaquete((char*)paqueteCiclo);

		destruirPaqueteCiclo(paqueteCiclo);

		structEvento* evento =NULL;
//	    while(evento == NULL){
		SDL_LockMutex(un_mutex);
		evento = (structEvento*) servidor->desencolarPaquete();
		SDL_UnlockMutex(un_mutex);
//	    }

	    if(evento!=NULL) {
	    	juego->aplicarPaquete(evento);
	    	free(evento);
	    }
	    SDL_Delay(25);

		juego->getMundo()->step(0.05,100,100);
		juego->getMundo()->comprobar_nivel_agua();

	}


	logFile.close();
	delete juego;
	delete servidor;
	return retorno;
	SDL_DestroyMutex(un_mutex);

}
#endif /* MAIN_SERVER_H_ */
