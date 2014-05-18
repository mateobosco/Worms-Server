

#include "Dibujador.h"
#include "circulo.h"
#include "poligono.h"
#include "rectangulo.h"
#include "Personaje.h"
#include "Paquete.h"

#include "SDL2/SDL_ttf.h"
#include <stdio.h>
#include <string>
#include <sstream>

Dibujador::Dibujador(){
	this->escalador = NULL;
	this->renderizador = NULL;
	this->escalado_x = 1;
	this->escalado_y = 1;
	this->textureCielo = NULL;
	this->textureAgua = NULL;
	this->textureTierra = NULL;
}

Dibujador::Dibujador(SDL_Renderer* renderer, Escalador* esc){
	this->escalador = esc;
	this->renderizador = renderer;
	this->escalado_x = esc->getEscalaX();
	this->escalado_y = esc->getEscalaY();
	this->textureCielo = NULL;
	this->textureAgua = NULL;
	this->textureTierra = NULL;
}

Dibujador::~Dibujador(){
	delete this->escalador;
	if (textureCielo) SDL_DestroyTexture(textureCielo);
	if (textureAgua) SDL_DestroyTexture(textureAgua);
	if (textureTierra) SDL_DestroyTexture(textureTierra);
}

// Retorna: 0- Exito. Negativo- Fracaso.
int Dibujador::limpiarFondo(){
	// Setea el fondo en blanco.
	SDL_SetRenderDrawColor(this->renderizador, 0xFF, 0xFF, 0xFF, 0xFF);
	return SDL_RenderClear(this->renderizador);
}

void Dibujador::dibujarFiguras(Figura** figuras, int cantidad){
	for (int32 i = 0; i < cantidad; ++i){
		if (figuras[i] != NULL) figuras[i]->dibujar(this);
	}
	this->actualizar();
}

void Dibujador::dibujarPersonajes(Personaje** personajes, int cantidad){
	for (int32 i = 0; i < cantidad; ++i){
		if (personajes[i] != NULL) personajes[i]->dibujar(this);
	}
	//this->actualizar();
}


int Dibujador::dibujarCirculo(Circulo* circulo){
	SDL_Color color = circulo->getColor();
	float32 radio = circulo->getRadio();
	Uint16 rad_pix_x = (Uint16) escalador->aplicarZoomX(radio);
	Uint16 rad_pix_y = (Uint16) escalador->aplicarZoomY(radio);
	b2Vec2 posicion = circulo->obtenerPosicion();
	b2Vec2* posicionVentanada = this->escalador->aplicarZoomPosicion(posicion);

	int retorno = filledEllipseRGBA( renderizador, posicionVentanada->x, posicionVentanada->y,
								rad_pix_x, rad_pix_y, color.r, color.g, color.b, CIRC_OPACIDAD);
	delete posicionVentanada;
	return retorno;
}

int Dibujador::dibujarPoligono(Poligono* poligono){
	SDL_Color color = poligono->getColor();
	int cantidad_lados = poligono->getCantVertices();
	float32* vecX = poligono->getVecX();
	float32* vecY = poligono->getVecY();
	Sint16* vecXventanado = this->escalador->aplicarZoomXVector(vecX, cantidad_lados);
	Sint16* vecYventanado = this->escalador->aplicarZoomYVector(vecY,cantidad_lados);
	int retorno =  filledPolygonRGBA(this->renderizador, vecXventanado, vecYventanado, cantidad_lados,
									color.r, color.g, color.b, POLI_OPACIDAD);
	delete[] vecX;
	delete[] vecY;
	delete[] vecYventanado;
	delete[] vecXventanado;
	return retorno;
}

int Dibujador::dibujarRectangulo(Rectangulo* rectangulo){
	SDL_Color color = rectangulo->getColor();
	float32* vecX = rectangulo->getVecX();
	float32* vecY = rectangulo->getVecY();
	Sint16* vecXventanado = this->escalador->aplicarZoomXVector(vecX , 4);
	Sint16* vecYventanado = this->escalador->aplicarZoomYVector(vecY , 4);
	int retorno = filledPolygonRGBA(this->renderizador, vecXventanado, vecYventanado, 4,
									color.r, color.g, color.b, RECT_OPACIDAD);
	delete[] vecX;
	delete[] vecY;
	delete[] vecYventanado;
	delete[] vecXventanado;
	return retorno;
}
SDL_Texture* Dibujador::RenderText(std::string message, std::string fontFile,
                        SDL_Color color, int fontSize)
{
//    //Open the font
//
//	if (TTF_Init() == -1){
//	    std::cout << TTF_GetError() << std::endl;
//	}
//	TTF_Font *font = nullptr;
//	font = TTF_OpenFont(fontFile.c_str(), fontSize);
//	//TTF_Font* font = TTF_OpenFont( "TPTaller/imagenes/abecedarionegrita.ttf", 28 );
//	if (font == nullptr)
//       // throw std::runtime_error("Failed to load font: " + fontFile + TTF_GetError());
//
//    //Render the message to an SDL_Surface, as that's what TTF_RenderText_X returns
//    SDL_Surface* surf = TTF_RenderText_Blended(font, message.c_str(), color);
//    SDL_Texture* texture = SDL_CreateTextureFromSurface(this->renderizador, surf);
//    //Clean up unneeded stuff
//    SDL_FreeSurface(surf);
//    TTF_CloseFont(font);
//
//    return texture;
}

SDL_Texture* Dibujador::dibujarPersonaje2(Personaje* personaje){
	SDL_Texture *gusanito = loadTexture(personaje->getDirImagen(), this->renderizador);
	b2Vec2 posicion = personaje->getPosition();
	b2Vec2* posicionVentanada = escalador->aplicarZoomPosicion(posicion);
	int anchoPX = escalador->aplicarZoomX( personaje->getAncho());
	int altoPX = escalador->aplicarZoomY(personaje->getAlto());
	int x = posicionVentanada->x - anchoPX/2;
	int y = posicionVentanada->y - altoPX/2;
	int w = anchoPX;
	int h = altoPX;
	char* direccion;
	//TTF_Font* gFont = TTF_OpenFont( "TPTaller/imagenes/lazy.ttf", 28 );
	//std::string textureText = "Some Text";
	SDL_Color textColor;
	textColor.r = 0;
	textColor.g = 0;
	textColor.b = 0;
	SDL_Texture *image;
	std::string nombre_jugador;
	SDL_Color color = { 0, 0, 0 };

	//SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );

	if (personaje->getSeleccionado()){
		nombre_jugador = "Juan";
		// DEBERIA DECIR nombre_jugador = personaje->nombreJugador;
		//SDL_Texture *cartel = loadTexture(direccion, this->renderizador);
		//renderTexture2(cartel, this->renderizador, x - 30 ,y - 60 , 80, 80 );
		image = RenderText( nombre_jugador, "TPTaller/imagenes/abecedarionegrita.ttf", color, 52); // despues preguntar el nombre de cada uno

		renderTexture2(image, this->renderizador, x - 30 ,y - 70 , 80, 80 );
		//if (cartel) SDL_DestroyTexture(cartel);
		if (image) SDL_DestroyTexture(image);
	}
	renderTexture2(gusanito, this->renderizador, x ,y ,w , h );
	delete[] posicionVentanada;
	if (gusanito) SDL_DestroyTexture(gusanito);
	return gusanito;
}



SDL_Renderer* Dibujador::getRenderizador(){
	return this->renderizador;
}

void Dibujador::renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h){
	SDL_Rect dst;
	int zoom =  escalador->getZoom();
	dst.x = escalador->aplicarZoomXaPix(x) ;
	dst.y = escalador->aplicarZoomYaPix(y) ;
	dst.w = w*((float32)zoom/100);
	dst.h = h*((float32)zoom/100);

	if(SDL_RenderCopy(ren, tex, NULL, &dst)!=0){
		loguear();
		logFile <<"    Error    " <<"\t RenderCopy falló " << endl;
	}
}

void Dibujador::renderTexture2(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h){
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;

	if(SDL_RenderCopy(ren, tex, NULL, &dst)!=0){
		loguear();
		logFile <<"    Error    " <<"\t RenderCopy falló " << endl;
	}
}

SDL_Texture* Dibujador::dibujar_cielo(Escalador* escalador, std::string path, int hasta){
	SDL_Texture *background = loadTexture(path.c_str(), this->renderizador);
	if(background == NULL){
		background = loadTexture(pathDefCielo.c_str(),this->renderizador);
		loguear();
		logFile <<"    Error    " <<"\t No se pudo cargar textura de fondo del cielo. Se carga por defecto el cielo de ubicación '"<<pathDefCielo<<"'." << endl;
	}
	textureCielo = background;
	renderTexture(background, this->renderizador, 0-escalador->getOffsetX() , 0-escalador->getOffsetY(), escalador->getPixelX(), hasta);
	return background;
}

SDL_Texture* Dibujador::dibujarAgua(Escalador* escalador, Agua* agua ){
	SDL_Texture *background = loadTexture(agua->GetPath(), this->renderizador);
	if(background == NULL){
		background = loadTexture(pathDefAgua,this->renderizador);
		loguear();
		logFile << "    Error    " <<"\t No se pudo cargar textura de agua. Se carga por defecto agua de ubicación '"<<pathDefAgua<<"'." << endl;
	}
	textureAgua = background;
	int pixelY = escalador->aplicarZoomY(agua->GetNivel()) + escalador->getOffsetY();
	renderTexture(background, this->renderizador,0- escalador->getOffsetX(), pixelY, escalador->getPixelX(), escalador->getPixelY());
	return background;
}

SDL_Texture* Dibujador::dibujar_tierra(Escalador* escalador, std::string path){
	SDL_Texture *background = loadTexture(path.c_str(), this->renderizador);
	if(background == NULL){
		loguear();
		logFile << "    Error    " <<"\t No se pudo cargar textura de fondo " << endl;
	}
	textureTierra = background;
	renderTexture(background, this->renderizador, 0-escalador->getOffsetX() , 0-escalador->getOffsetY(), escalador->getPixelX(), escalador->getPixelY());
	return background;
}

void Dibujador::actualizar(){
	SDL_RenderPresent(this->renderizador);
}

SDL_Texture* Dibujador::loadTexture(const std::string &file, SDL_Renderer *ren){
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
	if(texture == NULL) {
		loguear();
		logFile << "    Error    " <<"\t No se pudo cargar textura del file: " <<file<< endl;
	}
	return texture;
}

void Dibujador::iniciarFondo(Agua* agua, std::string pathCielo, std::string pathTierra){
	float nivelAgua = escalador->aplicarZoomY(agua->GetNivel()) - escalador->getOffsetY();
	this->dibujarAgua(escalador, agua);
	this->dibujar_cielo(escalador, pathCielo, nivelAgua);
	this->dibujar_tierra(escalador, pathTierra);
}

void Dibujador::dibujarFondo(Agua* agua){
	this->renderTexture(textureCielo, renderizador,0 , 0, escalador->getPixelX(), escalador->getPixelY() );
	this->renderTexture(textureAgua, renderizador, 0, agua->GetNivel()*(escalador->getPixelY()/escalador->getEscalaY()) , escalador->getPixelX(), escalador->getPixelY());
	this->renderTexture(textureTierra, renderizador, 0 , 0, escalador->getPixelX() , escalador->getPixelY());
}



int Dibujador::dibujarPaqueteFigura(structFigura figura){
	int cantidad = figura.cantidad;
	SDL_Color color = figura.color;
	int retorno;

	if ( cantidad == 2){
		float32 radio = figura.vector_vertices[1].x;
		Uint16 rad_pix_x = (Uint16) escalador->aplicarZoomX(radio);
		Uint16 rad_pix_y = (Uint16) escalador->aplicarZoomY(radio);
		b2Vec2 posicion = figura.vector_vertices[0];
		b2Vec2* posicionVentanada = this->escalador->aplicarZoomPosicion(posicion);
		retorno = filledEllipseRGBA( renderizador, posicionVentanada->x, posicionVentanada->y,
										rad_pix_x, rad_pix_y, color.r, color.g, color.b, CIRC_OPACIDAD);

	}
	if (cantidad > 2){
		int cantidad_lados = cantidad;
		float32* vecX = new float32[cantidad];
		float32* vecY = new float32[cantidad];
		for (int i=0; i < cantidad_lados; i++){
			vecX[i] = figura.vector_vertices[i].x;
			vecY[i] = figura.vector_vertices[i].y;
		}

		Sint16* vecXventanado = this->escalador->aplicarZoomXVector(vecX, cantidad_lados);
		Sint16* vecYventanado = this->escalador->aplicarZoomYVector(vecY,cantidad_lados);
		retorno =  filledPolygonRGBA(this->renderizador, vecXventanado, vecYventanado, cantidad_lados,
										color.r, color.g, color.b, POLI_OPACIDAD);
		delete[] vecX;
		delete[] vecY;
		delete[] vecYventanado;
		delete[] vecXventanado;
	}
	return retorno;
}

int Dibujador::dibujarPaquetePersonaje(structPersonaje paquete){
	char* path = "TPTaller/imagenes/gusanitoderecha.png";
	int dir = paquete.direccion;
	if (dir == 0) path = "TPTaller/imagenes/gusanitoderecha.png";
	if (dir == 1) path = "TPTaller/imagenes/gusanitoizquierda.png";
	b2Vec2 tam = paquete.tamano;

	SDL_Texture *gusanito = loadTexture(path, this->renderizador);
	b2Vec2 posicion = paquete.posicion;
	b2Vec2* posicionVentanada = escalador->aplicarZoomPosicion(posicion);
	int anchoPX = escalador->aplicarZoomX( tam.x);
	int altoPX = escalador->aplicarZoomY(tam.y);
	int x = posicionVentanada->x - anchoPX/2;
	int y = posicionVentanada->y - altoPX/2;
	int w = anchoPX;
	int h = altoPX;
	SDL_Texture *image;
	SDL_Color color = { 255, 255, 255 };
	image = RenderText(" Juan ", "TPTaller/imagenes/lazy.ttf", color, 52); // despues preguntar el nombre de cada uno
	renderTexture2(gusanito, this->renderizador, x ,y ,w , h );
	renderTexture2(image, this->renderizador, x - 30 ,y - 60 , 80, 80 );
	if (image) SDL_DestroyTexture(image);
	delete[] posicionVentanada;
	if (gusanito) SDL_DestroyTexture(gusanito);
	return 1;
}

void Dibujador::dibujarPaquete(structPaquete* paquete){
	int figuras = paquete->cantidad_figuras;
	int personajes = paquete->cantidad_personajes;
	for (int i = 0 ; i < figuras ; i++ ){
		this->dibujarPaqueteFigura(paquete->vector_figuras[i]);
	}
	for (int j = 0 ; j < personajes ; j ++){
		this->dibujarPaquetePersonaje(paquete->vector_personajes[j]);
	}
}
