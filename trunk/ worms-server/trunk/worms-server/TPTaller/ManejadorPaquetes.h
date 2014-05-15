/*
 * ManejadorPaquetes.h
 *
 *  Created on: 29/04/2014
 *      Author: facundo
 */

#ifndef MANEJADORPAQUETES_H_
#define MANEJADORPAQUETES_H_

#include <string>
#include <fstream>
#include <cstring>

//#include "Servidor.h" TODO

using namespace std;

class ManejadorPaquetes{
	public:
		//	Devuelve el paquete actual.
		string getPaqueteServer();
		string getPaqueteClient();

		// TODO Falta completar con los parámetros adecuados.
		void crearPaqueteServer(bool primera_vez, ifstream *archivo/* Server *servidor */);
		void crearPaqueteClient(/*Jugador *un_jugador*/);

		void procesarPaqueteServer(bool primera_vez, string paquete);

		ManejadorPaquetes();
		~ManejadorPaquetes();

	private:
		string pack_server; // El último creado.
		string pack_client; // El último creado.
		void copiarArchivoEscenario(ifstream *archivo);
		void levantarYaml(string paquete);
};

#endif /* MANEJADORPAQUETES_H_ */
