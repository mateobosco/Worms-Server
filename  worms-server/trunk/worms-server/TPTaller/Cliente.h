#ifndef CLIENTE_H_
#define CLIENTE_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <SDL2/SDL_thread.h>
#include "Socket.h"
#include "Paquete.h"

#define MAX_DATOS 100 // max number of bytes we can get at once

class Cliente{
	private:
		const char *name_client;
		Socket* socket_cl;
		SDL_mutex *mutex;
		char paquete_recibir[MAX_PACK];
		char paquete_enviar[MAX_PACK];
		int id;
		static int cant_clientes;
		structInicial* paqueteInicial;
		bool conectado;
		bool enviarpaquete;

	public:
		Cliente(const char *name, const char *ip_sv, const char* puerto);
		Cliente(int fd);
		~Cliente();
		int conectar();
		int enviar(char *mensaje, size_t longData);
		int runRecibirInfo();
		int run();
		int runEnviarInfo();
		Socket* getSocket();
		int getID();
		const char* getNombre();
		char* getPaquete();
		structInicial* getPaqueteInicial();
		void setConexion(bool estado);
		void actualizarPaquete(structEvento* evento);


};



#endif /* CLIENTE_H_ */
