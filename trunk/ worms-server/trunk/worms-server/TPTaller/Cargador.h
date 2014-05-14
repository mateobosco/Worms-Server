#ifndef CARGADOR_H_
#define CARGADOR_H_

#include <yaml-cpp/yaml.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <exception>

#include "circulo.h"
#include "poligono.h"
#include "rectangulo.h"
#include "Paquete.h"
#include "Constantes.h"

extern ofstream logFile;
extern void generarFechaYHora(char*,char*);
extern void loguear();

using namespace std;
using namespace YAML;

class Cargador{
	public:

		Cargador(const char* filename);
		~Cargador();

		bool load(Mundo mundo); //b2World
		Escalador* loadEscalador(Node nodo);
		bool loadPath(Node nodo, const char* clave,string& resultado);
		Circulo* loadCirculo(Node nodo, Mundo* mundo, Escalador *escalador,size_t i);
		Poligono* loadPoligono(Node nodo, Mundo* mundo, Escalador *escalador,size_t i,string tipo);
		Rectangulo* loadRectangulo(Node nodo, Mundo* mundo, Escalador *escalador,size_t i);
		Agua* loadAgua(Node nodo,string pathAgua);
		Figura* cargarFigura(Node nodo, Mundo* mundo, Escalador *escalador,size_t i);
		Node getNodo();
		bool getNodo(const Node nodoO,Node &nodoD, const char* clave);
		ifstream* getStream();
		structInicial* getPaqueteInicial();


	private:
		ifstream f_input;
		Node nodo_inicial;
		structInicial *datos;

		SDL_Color cargarColor(string color,size_t i);
		bool validarColor(string color);
		int cantidadLados(const Node nodo,size_t i,string tipo);
		uint convertirToInt(string cadena);
		b2Vec2 obtenerPosicion(Node nodo,size_t i);
		bool obtenerEstatico(Node nodo,size_t i);
		void minuscula(string &cadena);
		float32 obtenerEscala(Node nodo,size_t i);
		SDL_Color obtenerColor(Node nodo,size_t i);
		float32 obtenerRotacion(Node nodo,size_t i);
		float32 obtenerMasa(Node nodo,size_t i);
		float32 obtenerAncho(Node nodo,size_t i);
		float32 obtenerAlto(Node nodo,size_t i);

};






#endif /* CARGADOR_H_ */


