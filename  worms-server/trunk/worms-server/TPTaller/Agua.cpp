#include "Dibujador.h"
#include "Escalador.h"
#include "LectorMascara.h"

#include "Agua.h"


Agua::Agua(float32 nivel_agua, string &origen){
	this->nivel = nivel_agua;
	this->surface = IMG_Load( origen.c_str() );
	if(this->surface == NULL){
		loguear();
		logFile << "  Warning " <<"\t Imagen de agua no fue cargada. Path incorrecto. Por defecto se carga imagen de agua de:" << pathDefAgua << endl;
		this->path = pathDefAgua;
	}else{
		this->path = origen;
	}
}

Agua::~Agua(){
}

float32 Agua::GetNivel(){
	return this->nivel;
}

SDL_Surface* Agua::GetSurface(){
	return this->surface;
}

string Agua::GetPath(){
	return this->path;
}

