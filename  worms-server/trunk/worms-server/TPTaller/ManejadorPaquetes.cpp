#include "ManejadorPaquetes.h"

ManejadorPaquetes::ManejadorPaquetes() {
	pack_server.clear();
	pack_client.clear();
}

ManejadorPaquetes::~ManejadorPaquetes() {
}

string ManejadorPaquetes::getPaqueteServer(){
	return pack_server;
}

string ManejadorPaquetes::getPaqueteClient(){
	return pack_client;
}

/*Se crea un string con la información a pasar a cada cliente del siguiente modo:
	Si se carga por primera vez, en la primer posicion se carga un entero con la longitud
	del archivo yaml y luego el archivo.
	A continuación la cantidad de jugadores.
	Luego, el número del jugador y la posición de cada uno de sus personajes.
*/
void ManejadorPaquetes::crearPaqueteServer(bool primera_vez, ifstream *archivo/* TODO Server *servidor */){
	pack_server.clear();
	if(primera_vez){
		this->copiarArchivoEscenario(archivo);
	}


//	Uint8 cantidad_jugadores; /*TODO = servidor->getCantidadJugadores(); */
//	//Se copian la cantidad de jugadores contenida en el servidor (es un byte)
//
//	pack_server += cantidad_jugadores;// TODO checkear si se copia como int o como cadena de chars.
//
//
//	//TODO Jugador *un_jugador;
//	for(Uint8 cont_jug = 0; cont_jug < cantidad_jugadores; cont_jug++){
//		//Se copia la información de cada uno de los jugadores
//		//TODO lo que sigue hay que chequearlo y depende de que el server tenga a los
//		//jugadores
//
//		Jugador un_jugador = &(servidor->getJugador(cont_jug));
//		Uint8 numero_jugador;
//		memcpy(&numero_jugador, &(un_jugador->getNumero()), sizeof(Uint8));
//		pack_server += numero_jugador;
//		for(Uint8 cont_pj = 0; cont_pj < MAX_CANT_PERSONAJES; cont_pj++){
//			Personaje un_personaje = un_jugador->getPersonaje(cont_pj);
//			float32 posicion;
//			memcpy(&posicion, &(un_personaje.getPosition().x, sizeof(float32));
//			pack_server += posicion;
//			memcpy(&posicion, &(un_personaje.getPosition().y, sizeof(float32));
//			pack_server += posicion;
//		}
//		//Teniendo el número, ya se le crea el color internamente en el constructor
//	}
}

//TODO En realidad más que las posiciones, habría que pasarle la accion a realizar
void ManejadorPaquetes::crearPaqueteClient(/*Jugador *un_jugador*//*, string accion*/){
//	pack_client.clear();
//	Uint8 numero_jugador;
//	memcpy(&numero_jugador, &(un_jugador->getNumero()), sizeof(Uint8));
//	pack_client += numero_jugador;
//	for(Uint8 cont_pj = 0; cont_pj < MAX_CANT_PERSONAJES; cont_pj++){
//		Personaje un_personaje = un_jugador->getPersonaje(cont_pj);
//		float32 posicion;
//		memcpy(&posicion, &(un_personaje.getPosition().x, sizeof(float32));
//		pack_client += posicion;
//		memcpy(&posicion, &(un_personaje.getPosition().y, sizeof(float32));
//		pack_client += posicion;
//	}


//Si se le pasara la acción:
//	pack_client.clear();
//	pack_client += accion;
}

void ManejadorPaquetes::copiarArchivoEscenario(ifstream *archivo){
	//TODO tal vez habría que posicionarse al comienzo del archivo para no tener problemas.
	string auxiliar, linea;
	auxiliar.clear();
	while(getline(*archivo, linea)){//Se carga cada línea del archivo
		auxiliar += linea;
	}
	int longitud = auxiliar.length();
	memcpy(&pack_server, &longitud, sizeof(int)); //Para saber cuánto hay que leer
	pack_server += auxiliar;
}


//TODO esto deberia ir en otra clase junto con la funcion levantarYaml.
void ManejadorPaquetes::procesarPaqueteServer(bool primera_vez, string paquete){
	int posicion = 0;
	if(primera_vez){
		levantarYaml(paquete);
		int longitud;
		memcpy(&longitud, &paquete[0], sizeof(int));
		posicion = longitud + sizeof(int);
	}
	//Uint8 cant_jugadores = (Uint8) paquete[posicion];
	posicion++;
}

void ManejadorPaquetes::levantarYaml(string paquete){
	int longitud;
	memcpy(&longitud, &paquete[0], sizeof(int));
	ifstream archivo;
	archivo.open("escenario.yaml", ios::in);
	string yaml;
	memcpy(&yaml, &paquete[sizeof(int)], longitud);
	archivo >> yaml;
}
