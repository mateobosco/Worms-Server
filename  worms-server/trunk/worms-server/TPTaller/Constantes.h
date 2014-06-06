#ifndef CONSTANTES_H_
#define CONSTANTES_H_

#include <string>

using namespace std;

// Coeficientes de:
#define PI 3.1415
const int TILE_SIZE = 40;
const float M2P=20;
const float P2M=1/M2P;
const string pathAgua = "TPTaller/imagenes/aguita.png";
const string pathDefAgua = "TPTaller/imagenes/aguita.png";
const string pathEscTest = "TPTaller/prueba_escenario.yaml";
const string pathDefEs = "TPTaller/escenarioDef.yaml";
const string pathDefMas = "TPTaller/imagenes/mascara.png";
const string pathDefCielo = "TPTaller/imagenes/cieloN.jpg";

#define GRAVEDAD_X 0.0f
#define GRAVEDAD_Y 9.8f
#define INDICE_GRUPO 8

//Del circulo
#define CIRC_OPACIDAD 0xFF
#define CIRC_INERCIA_ROT -10.0f
#define CIRC_RESTITUCION 0.2
#define CIRC_FRICCION 0.15
#define CIRC_DENSIDAD 1
#define CIRC_CENTRO_MASA_X 0
#define CIRC_CENTRO_MASA_Y 0

//Del Poligono
#define POLI_OPACIDAD 0xFF
#define POLI_INERCIA_ROT -10.0f
#define POLI_RESTITUCION 0.2
#define POLI_FRICCION 0.15
#define POLI_DENSIDAD 1
#define POLI_CENTRO_MASA_X 0
#define POLI_CENTRO_MASA_Y 0

//Del Rectangulo
#define RECT_OPACIDAD 0xFF
#define RECT_INERCIA_ROT -10.0f
#define RECT_RESTITUCION 0.2
#define RECT_FRICCION 0.15
#define RECT_DENSIDAD 1
#define RECT_CENTRO_MASA_X 0
#define RECT_CENTRO_MASA_Y 0

#define OPACIDAD 255
#define MAX_CANT_JUGADORES 4
#define MAX_CANT_PERSONAJES 4
#define ANCHO_ESC 1000
#define ALTO_ESC 800

// Posicion en el arreglo ARGV.
#define MAX_NAME_USER 32
#define MAX_IP 16
#define MAX_PORT 6
#define POS_NAME_USER 1
#define POS_IP 2
#define POS_PORT 3

#define ERROR -1
#define SOCKET_CLIENTE 28
#define MAX_PACK 3000
#define MAX_MENSAJE 50

#endif /* CONSTANTES_H_ */
