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
		while(!server->getFinalizar() && server->getCantidadClientes() < server->getCantidadMaxConexiones()){
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
	//enviarPaqueteInicial(servidor, paqueteInicial);
	//servidor->actualizarPaquete((char*) paqueteInicial);

	SDL_Thread* listener =  SDL_CreateThread(runServidor,"listener",(void*)servidor);
	SDL_Thread* aceptar = SDL_CreateThread(aceptarConex,"aceptar",(void*)servidor);

	servidor->setThreadEscuchar(listener);
	servidor->setThreadAceptar(aceptar);

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
	int* clientes = servidor->getVectorClientes();
	//for (int i=0 ; i < servidor->getCantidadClientes() ; i++){
	//	if (clientes[i] != 0 && juego->getJugadores()[i] != NULL){
	//
	//		//clientes[i] = ID_CLIENTE TODO
	//	}
	//}

	//for (int i=0 ; i < servidor->getCantidadClientes() ; i++){
	//	int* clientes = servidor->getVectorClientes();
	//	if (clientes[i] != -1 && juego->getJugadores()[i] == NULL){
		//printf("AGREGO UN JUGADOR \n");
	//		juego->agregarJugador(i);
			//manejador_personajes->AgregarJugador(juego->getMundo(), i);
			//clientes[i] = i;
	//	}
	//}
	//juego->agregarJugador(3);


	//manejador_personajes->AgregarJugador(juego->getMundo(), 1); // esto deberia ir adentro del while que cuando se conecta un jugador le agregue los personajes


	//TODO LO QUE HAY ABAJO LO PUSE PARA QUE PUEDA DIBUJAR Y QUE ESPERE UN RATO
	//ANTES DE INICIAR LA CAIDA DE LOS OBJETOS, PARA PODER VERLO EN EL
	//CLIENTE
	juego->getMundo()->setVectorPersonajes(manejador_personajes->getPersonajes(), manejador_personajes->getCantidadPersonajes(), manejador_personajes->getCantidadJugadores());
	juego->getMundo()->setFiguras(juego->getFiguras(), juego->getCantidadFiguras());

	structPaquete* paqueteCiclo = crearPaqueteCiclo(juego->getMundo());
	//printf(" Voy a enviar un paquete con %d figuras \n", paqueteCiclo->cantidad_figuras);
	//printf(" Voy a enviar un paquete con %d personajes \n", paqueteCiclo->cantidad_personajes);
	structFigura* vector = paqueteCiclo->vector_figuras;
	structFigura paqueteFigura = vector[0];
	b2Vec2 posicion = paqueteFigura.vector_vertices[2];
	//printf(" Envia estas posiciones: (%f, %f) \n ", posicion.x,posicion.y);
	servidor->actualizarPaquete((char*)paqueteCiclo);
	juego->getMundo()->step(0.1,1,1);

	SDL_Delay(5000);
	//TODO ACA ES DONDE TERMINA EL CODIGO INNECESARIO.

	//while(servidor->getCantidadClientesActivos() == 0);

	while(!servidor->getFinalizar()){
		//printf(" LA CANTIDAD DE CLIENTES ES %d \n", servidor->getCantidadClientes());
		for (int i=0 ; i < servidor->getCantidadClientes() ; i++){
			int* clientes = servidor->getVectorClientes();
			//printf(" EL CLIENTES[%i] es %d \n", i,clientes[i]);

			if (clientes[i] != -1 && juego->getJugadores()[i] == NULL){
				printf("AGREGO UN JUGADORRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR con el id %d \n",i);
				SDL_Delay(500);

				Cliente* clienteActual = servidor->getClientes()[i];
				char* nombre = clienteActual->getNombre();
				//char nombre2[50];
				//strcpy(nombre2, nombre);
				printf(" NOMBREEEE 2 es %s\n", nombre);
				Jugador* jug = juego->agregarJugador(i, nombre);

				clienteActual->setJugador(jug);
				//manejador_personajes->AgregarJugador(juego->getMundo(), i);
				//clientes[i] = i;
			}
		}
		juego->getMundo()->setVectorPersonajes(manejador_personajes->getPersonajes(), manejador_personajes->getCantidadPersonajes(), manejador_personajes->getCantidadJugadores());
		juego->getMundo()->setFiguras(juego->getFiguras(), juego->getCantidadFiguras());

		structPaquete* paqueteCiclo = crearPaqueteCiclo(juego->getMundo());
		servidor->actualizarPaquete((char*)paqueteCiclo);

		destruirPaqueteCiclo(paqueteCiclo);

		structEvento* evento =NULL;
	    while(evento == NULL){
	    	evento = (structEvento*) servidor->desencolarPaquete();
	    }

//	    printf (" RECIBE EL ID DEL PIBITO : %d \n", evento->nro_jugador);


	    if(evento!=NULL) {
	    	juego->aplicarPaquete(evento);
	    	free(evento);
	    }
	    SDL_Delay(25);

		juego->getMundo()->step(0.05,100,100);
		juego->getMundo()->comprobar_nivel_agua();
//		if(servidor->getCantidadClientesActivos() == 0){
//			servidor->setFinalizar(true);
//			printf("Se esperarán 15 segundos por si algún cliente se conecta.");
//			SDL_Delay(15000);
//			if(servidor->getCantidadClientesActivos() != 0){
//				servidor->setFinalizar(false);
//			}
//		}
	}


	//
	logFile.close();
	delete juego;
	delete servidor;
	return retorno;

}
#endif /* MAIN_SERVER_H_ */
