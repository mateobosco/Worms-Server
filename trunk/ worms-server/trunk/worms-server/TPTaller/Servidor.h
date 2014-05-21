

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

	void setPaqueteInicial(char paquete[MAX_PACK]);
	void actualizarPaquete(char paquete[MAX_PACK]);
	void* desencolarPaquete();
	int escucharConexiones();
	int aceptarConexiones();

	int runEnviarInfo(Cliente* cliente);

	int runEnviarInfoInicial(Cliente* cliente);

	int runRecibirInfo(void* paquete);

	int iniciarServidor();
	int runEscucharConexiones();
	int* getVectorClientes();
	int validarSocket(int sock);
	int validarCliente(Cliente* cliente);

	int recibirNombre(Cliente *client);
	int checkNuevoCliente(Cliente *client);

	bool getFinalizar();
	void setThreadEscuchar(SDL_Thread *listen);
	void setThreadAceptar(SDL_Thread *accept);

	void setAceptado(bool aceptar);

private:
	char paqueteEnviar[MAX_PACK];
	char paqueteInicial[MAX_PACK];
	queue<void*> paquetesRecibir;
	int cantidadMaxConexiones;
	int cantClientes;
	int clientesActivos;
	Socket* listener;
	Cliente* clientes[MAX_CANT_JUGADORES];
	Cliente* clientesDesconectados[MAX_CANT_JUGADORES]; //ver tamanio todo [Nahue: si no se usa-> ELIMINAR]
	SDL_mutex *mutex;
	int maxFD;
	const char* puerto;
	int vector_clientes[4];
	bool enviar;
	bool finalizar;
	SDL_Thread *escuchar;
	SDL_Thread *aceptar;

};

#endif /* SERVIDOR_H_ */
