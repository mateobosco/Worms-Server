#ifndef MAIN_SERVER_H_
#define MAIN_SERVER_H_
#include "Juego.h"
#include "Funciones.h"
#include "Servidor.h"

int runServidor(void* serv){
	return ((Servidor*) serv)->runEscucharConexiones();
}

int main_server(int argc,char* argv[]){
	int retorno = 0;
	Servidor *servidor = new Servidor(MAXJUG);
	printf("Servidor corriendo\n");

	SDL_Thread* listener =  SDL_CreateThread(runServidor,"listener",(void*)servidor);
	int thread = 0;
//	SDL_WaitThread(listener, &thread);
	if(listener == NULL){
		//ver que hacer
		//log error todo
	}
	int contador = 0;
	while (servidor->getCantidadClientes() < 1 || contador < 10000000){
		contador ++;
	}


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
	    juego->getMundo()->step(0.1,100,100);

	}

	logFile.close();
	delete juego;
	return retorno;

}
#endif /* MAIN_SERVER_H_ */
