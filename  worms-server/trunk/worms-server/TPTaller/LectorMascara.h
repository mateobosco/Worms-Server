#ifndef LECTORMASCARA_H_
#define LECTORMASCARA_H_

#include <vector>
#include <cstdio>
#include <iostream>
#include <exception>
#include <SDL2/SDL_image.h>

#include "Escalador.h"
#include "Constantes.h"

extern void generarFechaYHora(char*, char*);
extern ofstream logFile;
extern void loguear();


class LectorMascara {
public:
	LectorMascara(std::string& path);
	b2Vec2* LeerMascara(Escalador* escalador);
	int GetPixelAncho();
	void setMascaraPorDefecto();
	~LectorMascara();
	SDL_Surface* GetSurface();
	string getPath();

private:
	string path;
	SDL_Surface* superficie;
	int PIXEL_ALTO;
	int PIXEL_ANCHO;
};

#endif /* LECTORMASCARA_H_ */
