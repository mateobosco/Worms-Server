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
	servidor->setPaqueteInicial((char*) paqueteInicial);
	//enviarPaqueteInicial(servidor, paqueteInicial);
	//servidor->actualizarPaquete((char*) paqueteInicial);

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
	int* clientes = servidor->getVectorClientes();
	//for (int i=0 ; i < servidor->getCantidadClientes() ; i++){
	//	if (clientes[i] != 0 && juego->getJugadores()[i] != NULL){
	//
	//		//clientes[i] = ID_CLIENTE TODO
	//	}
	//}
	manejador_personajes->AgregarJugador(juego->getMundo(), 0);


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


	while(true){
		juego->getMundo()->setVectorPersonajes(manejador_personajes->getPersonajes(), manejador_personajes->getCantidadPersonajes(), manejador_personajes->getCantidadJugadores());
		juego->getMundo()->setFiguras(juego->getFiguras(), juego->getCantidadFiguras());

		structPaquete* paqueteCiclo = crearPaqueteCiclo(juego->getMundo());
		servidor->actualizarPaquete((char*)paqueteCiclo);
		SDL_Delay(25);
		//destruirPaqueteCiclo(paqueteCiclo);

		structEvento* evento;
	    evento = (structEvento*) servidor->desencolarPaquete();
//	    printf (" RECIBE EL ID DEL PIBITO : %d \n", evento->nro_jugador);
	    juego->aplicarPaquete(evento, manejador_personajes);

		juego->getMundo()->step(0.05,100,100);
		juego->getMundo()->comprobar_nivel_agua();
	}


	//
	logFile.close();
	delete juego;
	delete servidor;
	return retorno;

}
#endif /* MAIN_SERVER_H_ */
