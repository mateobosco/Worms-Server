#ifndef MAIN_SERVER_H_
#define MAIN_SERVER_H_
#include "Juego.h"
#include "Funciones.h"
#include "Servidor.h"
#include <sys/time.h>

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
	//juego->getMundo()->step(0.1,1,1);
	int jugadores_necesarios = 1;
	SDL_Delay(2000);

	int comenzar=0;
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


		if (jugadores_necesarios == servidor->getCantidadClientesActivos() && comenzar==0){
			comenzar=1;
			juego->resetearRelojRonda();

		}
		int nro_jugador_actual = juego->getJugadorActual();
		Jugador* jugador_actual = juego->getJugadores()[nro_jugador_actual];
		char* nombre1 = jugador_actual->getNombre();
		//printf(" MANDO EL NOMBRE %s \n", nombre1);
		Arma* arma_actual=juego->getArmaActual();
//		int nro_arma;
//		if(arma_actual){
//			printf(" ========================== \n");
//			nro_arma=arma_actual->getTipo();
//
//		}
//		else{
//			nro_arma=0;
//		}
		structPaquete* paqueteCiclo = crearPaqueteCiclo(juego->getMundo(), servidor->getMensajeMostrar(), nro_jugador_actual, comenzar, juego->getRelojRonda(), nombre1);
		juego->setPaqueteProyectil(paqueteCiclo);
		//printf(" EL TAMANIO DEL STACK PROYECTIL ES %d \n", servidor->getTamanioColaExplosion());
		juego->checkColisionProyectil(paqueteCiclo);
		if(paqueteCiclo->radio_explosion != 0 && paqueteCiclo->radio_explosion != -1 && servidor->getTamanioColaExplosion()==0){
			printf(" LO encolo y el radio es %d \n", paqueteCiclo->radio_explosion);

			servidor->encolarExplosion(paqueteCiclo);
		}

		servidor->actualizarPaquete((char*)paqueteCiclo);

		destruirPaqueteCiclo(paqueteCiclo);

		structEvento* evento =NULL;
	    evento = (structEvento*) servidor->desencolarPaquete();


	    if(evento!=NULL) {
	    	juego->aplicarPaquete(evento, comenzar);
	    	free(evento);
	    }
	    //printf(" TIMER: %d \n", juego->getRelojRonda());
	    if(juego->getRelojRonda() > 60000 && comenzar==1){
	    	printf(" RESETEO EL RELOJJJ \n");
	    	printf(" TIMER: %d \n", juego->getRelojRonda()/1000);
	    	//juego->resetearRelojRonda();
	    	juego->pasarTurno();
	    }
	    Jugador* jugador_actual2 = juego->getJugadores()[juego->getJugadorActual()];
	    if(jugador_actual2->getPersonajes()[jugador_actual2->getPersonajeSeleccionado()]->getMuerto()){
	    	juego->pasarTurno();
	    }

	    SDL_Delay(10);


		juego->getMundo()->comprobar_nivel_agua();

		juego->getMundo()->step(0.025,100,100);

	}
	logFile.close();
	delete juego;
	delete servidor;
	SDL_DestroyMutex(un_mutex);
	return retorno;
}

#endif /* MAIN_SERVER_H_ */
