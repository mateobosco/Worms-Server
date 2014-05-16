

using namespace std;
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <SDL2/SDL_thread.h>
#include <iostream>
#include <queue>

#include "Socket.h"
#include "Cliente.h"
#include "Paquete.h"


#define MAXJUG 4

#ifndef SERVIDOR_H_
#define SERVIDOR_H_

class Servidor {
public:
	Servidor(int maxCon); //ver puerto
	virtual ~Servidor();

	struct sockaddr_in getDireccion();
	Socket* getSocket();
	int getCantidadMaxConexiones();
	int getCantidadClientes();

	void actualizarPaquete(char paquete[MAX_PACK]);
	void* desencolarPaquete();
	int escucharConexiones();
	int aceptarConexiones();

	int runEnviarInfo(Cliente* cliente);

	int runRecibirInfo(void* paquete);

	int iniciarServidor();
	int runEscucharConexiones();
	int* getVectorClientes();
	int validarSocket(int sock);


private:
	char paqueteEnviar[MAX_PACK];
	queue<void*> paquetesRecibir;
	int cantidadMaxConexiones;
	int cantClientes;
	Socket* listener;
	Cliente* clientes[MAXJUG];
	SDL_mutex *mutex;
	int maxFD;
	const char* puerto;
	int vector_clientes[4];


};

#endif /* SERVIDOR_H_ */
