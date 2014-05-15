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
		printf("Thread de aceptar conexiones\n");
		serv->aceptarConexiones();
	}
	return 0;
}

int main_server(int argc,char* argv[]){
	int retorno = 0;
	Servidor *servidor = new Servidor(MAXJUG);
	printf("Servidor corriendo\n");

	SDL_Thread* listener =  SDL_CreateThread(runServidor,"listener",(void*)servidor);
	SDL_Thread* aceptar = SDL_CreateThread(aceptarConex,"aceptar",(void*)servidor);

	int thread = 0;
	SDL_WaitThread(listener, &thread);
	if(listener == NULL){
		//ver que hacer
		//log error todo
	}

/*
	Juego *juego = new Juego();
	ManejadorPersonajes* manejador_personajes = new ManejadorPersonajes();
	structInicial* paqueteInicial = juego->getPaqueteInicial();

	while(true){
		int* clientes = servidor->getVectorClientes();
		for (int i=0 ; i < MAXJUG ; i++){
			if (clientes[i] != 0 && juego->getJugadores()[i] != NULL){
				manejador_personajes->AgregarJugador(juego->getMundo(), clientes[i]);
				//Cliente* cliente_actual = clientes[i];
				//servidor->enviarInformacion(cliente_actual->getSocket(),paqueteInicial, sizeof(structInicial));
			}
		}

		juego->getMundo()->setVectorPersonajes(manejador_personajes->getPersonajes(), manejador_personajes->getCantidadPersonajes(), manejador_personajes->getCantidadJugadores());
		juego->getMundo()->setFiguras(juego->getFiguras(), juego->getCantidadFiguras());

		structPaquete* paqueteCiclo = crearPaqueteCiclo(juego->getMundo());
		for (int i = 0; i < MAXJUG ; i++){
			if (clientes[i] != 0 && juego->getJugadores()[i] != NULL){
				//Cliente* cliente_actual = clientes[i];
				//servidor->actualizarPaquete(paquteCiclo);
				//servidor->enviarInformacion(cliente_actual->getSocket(), paqueteCiclo, sizeof(structPaquete));
			}
		}
		destruirPaqueteCiclo(paqueteCiclo);
		structEventos* evento;
	    evento = (structEventos*) servidor->desencolarPaquete();
	    juego->aplicarPaquete(evento, manejador_personajes);










	}





	logFile.close();
	delete juego; */
	return retorno;

}
#endif /* MAIN_SERVER_H_ */
