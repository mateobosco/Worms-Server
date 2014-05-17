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
	//printf(" EN EL SERVIDOR, EL PAQUETE INICIAL EL ANCHO DEL MAPA ES %f \n", paqueteInicial->ancho_escenario);
	servidor->actualizarPaquete(paqueteInicial);

	SDL_Thread* listener =  SDL_CreateThread(runServidor,"listener",(void*)servidor);
	SDL_Thread* aceptar = SDL_CreateThread(aceptarConex,"aceptar",(void*)servidor);

	int thread = 0;
	int thread_2 = 0;
	SDL_WaitThread(listener, &thread);
	SDL_WaitThread(aceptar, &thread_2);
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
	delete servidor;
	return retorno;

}
#endif /* MAIN_SERVER_H_ */
