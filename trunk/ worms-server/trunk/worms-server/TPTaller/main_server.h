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

void cargarNuevoCliente(Servidor *servidor, Juego *juego, ManejadorPersonajes *manejador_personajes){
	if(servidor->getHayClienteNuevo()){
		for (int i=0 ; i < servidor->getCantidadClientes() ; i++){
			int* clientes = servidor->getVectorClientes();
			if (clientes[i] != -1 && juego->getJugadores()[i] == NULL){
				SDL_Delay(500);
				Cliente* clienteActual = servidor->getClientes()[i];
				char *nombre = clienteActual->getNombre();
				Jugador* jug = juego->agregarJugador(i, nombre);
				clienteActual->setJugador(jug);
			}
		}
		servidor->clienteNuevoCargado();
		juego->getMundo()->setVectorPersonajes(manejador_personajes->getPersonajes(), manejador_personajes->getCantidadPersonajes(), manejador_personajes->getCantidadJugadores());
		juego->getMundo()->setFiguras(juego->getFiguras(), juego->getCantidadFiguras());
	}
}

int main_server(int argc,char* argv[]){
	printf("TAMANO DE PAQUETE %d \n", (int) sizeof(structPaquete));
	int retorno = 0;
	Servidor *servidor = new Servidor(MAX_CANT_JUGADORES);
	printf("Servidor corriendo\n");


	SDL_Thread* listener =  SDL_CreateThread(runServidor,"listener",(void*)servidor);
	SDL_Thread* aceptar = SDL_CreateThread(aceptarConex,"aceptar",(void*)servidor);

	servidor->setThreadEscuchar(listener);
	servidor->setThreadAceptar(aceptar);

	Juego *juego = new Juego();
	ManejadorPersonajes* manejador_personajes = juego->getManejadorPersonajes();
	structInicial* paqueteInicial = juego->getPaqueteInicial();
	servidor->setPaqueteInicial((char*) paqueteInicial);

	int jugadores = servidor->getCantidadClientes();
	while (jugadores<=0){
		jugadores = servidor->getCantidadClientes();
		SDL_Delay(2000);
	}

	printf("-----------------------------------------EL SERVIDOR INICIA EL JUEGO-------------------------------------\n");
	juego->getMundo()->setVectorPersonajes(manejador_personajes->getPersonajes(), manejador_personajes->getCantidadPersonajes(), manejador_personajes->getCantidadJugadores());
	juego->getMundo()->setFiguras(juego->getFiguras(), juego->getCantidadFiguras());
	int jugadores_necesarios = CANT_NECESARIA_JUGADORES; //4
	SDL_Delay(2000);

	int comenzar=0;
	bool stop = true;
	SDL_mutex *un_mutex = SDL_CreateMutex();
	char winners[LONGITUD_WINNERS];
	winners[0] = '\0';
	int check_winner = -1;
	int cant_winners = -1;
	bool empezo_explosion = false;
	bool termino_explosion = false;
	uint32 inicio, diferencia;
	while(!servidor->getFinalizar()){

		cargarNuevoCliente(servidor, juego, manejador_personajes);

		if(stop){
			if(jugadores_necesarios <= servidor->getCantidadClientesActivos()){
				stop = false;
				comenzar=1;
				juego->resetearRelojRonda();
				SDL_Delay(2000);
			}
			else
				continue;
		}

		int nro_jugador_actual = juego->getJugadorActual();
		Jugador* jugador_actual = juego->getJugadores()[nro_jugador_actual];
		juego->dormirPersonajes(jugador_actual->getPersonajes()[jugador_actual->getPersonajeSeleccionado()]);
		char* nombre1 = jugador_actual->getNombre();
		structPaquete* paqueteCiclo = crearPaqueteCiclo(juego, juego->getMundo(), servidor->getMensajeMostrar(), nro_jugador_actual, comenzar, juego->getRelojRonda(), nombre1, winners, cant_winners, juego->getResetear());
		juego->setPaqueteProyectil(paqueteCiclo);
		juego->checkColisionProyectil(paqueteCiclo);
		if (paqueteCiclo->resetear) {
			servidor->encolarExplosion(paqueteCiclo);
		}


		if(contieneSonido(paqueteCiclo)){
			servidor->encolarExplosion(paqueteCiclo);
		}
		if(paqueteCiclo->radio_explosion != 0 && paqueteCiclo->radio_explosion != -1 && servidor->getTamanioColaExplosion()==0){
			printf(" ENCOLA LA EXPLOSION\n");
			servidor->encolarExplosion(paqueteCiclo);
			servidor->agregarExplosion(paqueteCiclo->posicion_proyectil ,paqueteCiclo->radio_explosion);
			servidor->actualizarArmasPaqueteInicial(juego);

		}
//		if (paqueteCiclo->cant_ganadores > 0) servidor->encolarExplosion(paqueteCiclo);
		servidor->actualizarPaquete((char*)paqueteCiclo);
		destruirPaqueteCiclo(paqueteCiclo);
//		winners[0] = '\0';
		structEvento* evento =NULL;
	    evento = (structEvento*) servidor->desencolarPaquete();

	    if(evento!=NULL) {
//		    if(evento->arma_seleccionada != 0){
//		    }
	    	juego->aplicarPaquete(evento, comenzar);
	    	if(evento->arma_seleccionada <= 6 && evento->nro_jugador <=4 && evento->direccion != -9 && evento->direccion == -1 && evento->direccion == 0 && evento->direccion == 1){
	    		//printf(" PUDO SER LEIDO ESTA MIERDA y es %d \n", evento->arma_seleccionada);
	    		//printf(" PUDO SER LEIDO ESTA MIERDA y jug %d \n", evento->nro_jugador);
	    		//printf(" PUDO SER LEIDO ESTA MIERDA y alet %d \n", evento->aleatorio);
	    		//printf(" PUDO SER LEIDO ESTA MIERDA y direcc %d \n", evento->direccion);
	    		//printf(" PUDO SER LEIDO ESTA MIERDA y fuerza %d \n", evento->fuerza);
	    		free(evento);
	    	}

	    	evento = NULL;
	    }

	    if((!empezo_explosion) && (!termino_explosion)){
//	    	if(!servidor->getRecibir()){
//				printf("Está en el volver de recibir.\n");
//	    		servidor->volverARecibir();
//	    	}
			if(juego->getRelojRonda() > 60000 && comenzar==1){
				//juego->resetearRelojRonda();
				juego->pasarTurno();
			}
			Jugador* jugador_actual2 = juego->getJugadores()[juego->getJugadorActual()];
			if(jugador_actual2->getPersonajes()[jugador_actual2->getPersonajeSeleccionado()]->getMuerto()){
				for(int i = 0; i <= juego->getCantidadJugadores(); i++)
					juego->pasarTurno();
			}
			 juego->getMundo()->comprobar_nivel_agua(juego->getArmaActual(), juego->getProjInAir());
			check_winner = juego->checkGanador();
			switch(check_winner){
				case -1: //No hay ganador
						break;
				case 0: //Hay empate
						for(int i = juego->getTotalPerdedores(); i < juego->getCantidadJugadores(); i++){
							char auxiliar[MAX_NAME_USER + 1];
							sprintf(auxiliar, "%s,", juego->getJugadores()[juego->getPerdedores()[i]]->getNombre());
//							int longitud = strlen(auxiliar);
//							auxiliar[longitud] = ',';
//							auxiliar[longitud + 1] = '\0';
							//strcpy(auxiliar, juego->getJugadores()[juego->getPerdedores()[i]]->getNombre());
							strcat(winners, auxiliar);
							printf("Entró al for: %i", i);
							printf("Winners dentro del for: %s", winners);
						}
						winners[strlen(winners) - 1] = '\0';
						cant_winners = (juego->getCantidadJugadores() - juego->getTotalPerdedores());
//						printf("Cant_Jugadores: %i\nCant_Perdedores: %i\n", winners);
						printf("Cant_winners: %i\nWinners: %s\n", cant_winners,winners);
						SDL_Delay(10000);
						break;
				case 1: //Hay ganador
						for(int i = 0; i < MAX_CANT_JUGADORES; i++){
							int j;
							for(j = 0; j < juego->getTotalPerdedores(); j++){
								if(juego->getPerdedores()[j] == i){
									break;
								}
							}
							if(j == juego->getTotalPerdedores()){
								check_winner = i;
								strcpy(winners, juego->getJugadores()[check_winner]->getNombre());

								break;
							}
						}
						cant_winners = 1;
						break;
			}
	    } else{
//	    	if(servidor->getRecibir()){
//	    		printf("Está en el dejar de recibir.\n");
//	    		inicio = SDL_GetTicks();
//	    		servidor->dejarDeRecibir();
//	    	}
	    	if(termino_explosion){
//				diferencia = SDL_GetTicks() - inicio;
//				if(diferencia >= 7000){
//					printf("Está en el if antes.\n");
					empezo_explosion = false;
					termino_explosion = false;
//					SDL_Delay(1000);
//					printf("Está en el if después.\n");
//				}
	    	}
	    }


//
//		if ((check_winner != -1 && servidor->getCantidadClientesActivos() >= CANT_NECESARIA_JUGADORES)/*|| reiniciar*/){
////			strcpy(winner, juego->getJugadores()[numero_winner]->getNombre());	153				printf("RESETEA EL NIVEL EL SERVER, en maim server \n");
////			printf("Ganó el jugador: %s\n", winner);	154				juego->resetNivel();
////			//todo falta reiniciar el nivel	155				check_winner = -1;
////			//juego->resetNivel();	156				cant_winners = -1;
////		}	157				winners[0] = '\0';
//			juego->setResetear(true);
//			juego->getMundo()->setVectorPersonajes(manejador_personajes->getPersonajes(), manejador_personajes->getCantidadPersonajes(), manejador_personajes->getCantidadJugadores());
//			juego->getMundo()->setFiguras(juego->getFiguras(), juego->getCantidadFiguras());
////			reiniciar = false;
//		}


		if ((check_winner != -1 && servidor->getCantidadClientesActivos() >= CANT_NECESARIA_JUGADORES)/*|| reiniciar*/){
//			strcpy(winner, juego->getJugadores()[numero_winner]->getNombre());	153				printf("RESETEA EL NIVEL EL SERVER, en maim server \n");
//			printf("Ganó el jugador: %s\n", winner);	154				juego->resetNivel();
//			//todo falta reiniciar el nivel	155				check_winner = -1;
//			//juego->resetNivel();	156				cant_winners = -1;
//		}	157				winners[0] = '\0';

//			if (winners[0] != '\0') printf("GANADOR5 %s \n", winners);
			printf("RESETEA EL NIVEL EL SERVER, en maim server \n");
			juego->resetNivel();
//			if (juego->getResetear()) printf("EN EL JUEGO DICE QUE HAY QUE RESETEAR \n");
			check_winner = -1;
//			cant_winners = -1;
//			winners[0] = '\0';
			juego->setResetear(true);
			juego->getMundo()->setVectorPersonajes(manejador_personajes->getPersonajes(), manejador_personajes->getCantidadPersonajes(), manejador_personajes->getCantidadJugadores());
			juego->getMundo()->setFiguras(juego->getFiguras(), juego->getCantidadFiguras());
//			reiniciar = false;
			servidor->reiniciarExplosionesPaquete();
		}


		SDL_Delay(10);
		juego->getMundo()->step(0.025, 4, 2);
	}
	logFile.close();
	if(juego) delete juego; juego = NULL;
	if(servidor) delete servidor; servidor = NULL;
	SDL_DestroyMutex(un_mutex);
	return retorno;
}

#endif /* MAIN_SERVER_H_ */
