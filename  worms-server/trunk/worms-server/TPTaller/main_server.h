#ifndef MAIN_SERVER_H_
#define MAIN_SERVER_H_
#include "Juego.h"
#include "Funciones.h"
#include "Servidor.h"

int runServidor(void* serv){
	return ((Servidor*) serv)->runEscucharConexiones();
}

int aceptarConex(void* servidor){
	Servidor* serv = (Servidor*) servidor;
	while(serv->getCantidadClientes() < serv->getCantidadMaxConexiones()){
		int accept = serv->aceptarConexiones();
		if (accept != EXIT_SUCCESS) printf("Error al aceptar\n");
	}
	return 0;
}

int main_server(int argc,char* argv[]){
	int retorno = 0;
	Servidor *servidor = new Servidor(MAXJUG);
	printf("Servidor corriendo\n");

	Juego *juego = new Juego();
	ManejadorPersonajes* manejador_personajes = new ManejadorPersonajes();
	structInicial* paqueteInicial = juego->getPaqueteInicial();


	servidor->actualizarPaquete((char*) paqueteInicial);

	SDL_Thread* listener =  SDL_CreateThread(runServidor,"listener",(void*)servidor);
	SDL_Thread* aceptar = SDL_CreateThread(aceptarConex,"aceptar",(void*)servidor);

	int thread = 0;
	int thread_2 = 0;
	//SDL_WaitThread(listener, &thread);
	//SDL_WaitThread(aceptar, &thread_2);
	if(listener == NULL){
		//ver que hacer
		//log error todo
	}
	int jugadores = servidor->getCantidadClientes();
	printf("CANTIDAD DE JUGADORES: %d \n",jugadores);
	while (jugadores<=0){
		jugadores = servidor->getCantidadClientes();
		printf("CANTIDAD DE JUGADORES: %d \n",jugadores);
		SDL_Delay(2000);
	}

	printf("-----------------------------------------EL SERVIDOR INICIA EL JUEGO-------------------------------------\n");

	while(true){
		int* clientes = servidor->getVectorClientes();
		for (int i=0 ; i < MAXJUG ; i++){
			if (clientes[i] != 0 && juego->getJugadores()[i] != NULL){
				manejador_personajes->AgregarJugador(juego->getMundo(), clientes[i]);
				//clientes[i] = ID_CLIENTE TODO
			}
		}

		juego->getMundo()->setVectorPersonajes(manejador_personajes->getPersonajes(), manejador_personajes->getCantidadPersonajes(), manejador_personajes->getCantidadJugadores());
		juego->getMundo()->setFiguras(juego->getFiguras(), juego->getCantidadFiguras());

		structPaquete* paqueteCiclo = crearPaqueteCiclo(juego->getMundo());
		servidor->actualizarPaquete((char*)paqueteCiclo);
		SDL_Delay(500);
		//destruirPaqueteCiclo(paqueteCiclo);

/*		structEventos* evento;
	    evento = (structEventos*) servidor->desencolarPaquete();
	    juego->aplicarPaquete(evento, manejador_personajes);*/
		juego->getMundo()->step(0.1,100,100);
	}


	logFile.close();
	delete juego;
	delete servidor;
	return retorno;

}
#endif /* MAIN_SERVER_H_ */
