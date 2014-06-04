#ifndef SOCKET_H_
#define SOCKET_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdio>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

#include "Constantes.h"

#define PUERTO "9300"
using namespace std;

class Socket {
public:
	Socket(const char *ip, const char *puerto);
	Socket(const char *puerto, int sockfd); //Constructor cuando ya fue creado el socket previamente
	virtual ~Socket();

	// Conecta el socket a una dirección y puerto destino.
	int conectar();

	int EnlazarYEscuchar(int cantMaxCon);

	// Configura el socket para recibir conexiones en la dirección y puerto
	int escuchar(int maxConexiones);

	// Acepta una conexión entrante.
	int aceptar();

	// Envía datos a través del socket de forma completa.
	// Devuelve 0 si se ha realizado el envio correctamente o -1 en caso
	// de error.
	int enviar(const void* dato, size_t longDato);

	// Recibe datos a través del socket.
	// Devuelve el número de bytes que han sido leidos o 0 (cero) si el
	// host remoto a cerrado la conexión.
	int recibir(char* buffer, int longBuffer);

	// Cierra el socket.
	int cerrar();

	// Devuelve true si el socket se encuentra activo o false en su
	// defecto.
	bool estaActivo();
	int getFD();
	void setInfo(struct sockaddr* sock );
	struct addrinfo* getInfo();
private:
	int sockFD; //File descriptor asociado al socket
	const char *puerto;
	struct addrinfo *info;
	bool activo;
};

#endif /* SOCKET_H_ */
