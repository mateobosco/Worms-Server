#ifndef DIBUJADOR_H_
#define DIBUJADOR_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_pixels.h>

#include "Agua.h"


extern ofstream logFile;
extern void generarFechaYHora(char*,char*);
extern void loguear();

class Dibujable;
class Figura;
class Circulo;
class Poligono;
class Rectangulo;
class Personaje;
struct structFigura;
struct structPersonaje;
struct structPaquete;

class Dibujador {

public:
	Dibujador();
	Dibujador(SDL_Renderer* renderer, Escalador* escalador);
	~Dibujador();
	int limpiarFondo();
	void dibujarFiguras(Figura** figuras, int cantidad);
	void dibujarPersonajes(Personaje** personajes, int cantidad);

	int dibujarCirculo(Circulo* circulo);
	int dibujarPoligono(Poligono* poligono);
	int dibujarRectangulo(Rectangulo* rectangulo);
	int dibujarPersonaje(Personaje* personaje);
	SDL_Texture* dibujarPersonaje2(Personaje* personaje);
	SDL_Renderer* getRenderizador();
	void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);
	void renderTexture2(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);
	SDL_Texture* dibujar_cielo(Escalador* escalador, std::string path, int hasta);
	SDL_Texture* dibujarAgua(Escalador* escalador, Agua* agua);
	SDL_Texture* dibujar_tierra(Escalador* escalador, std::string path);
	void iniciarFondo(Agua* agua, std::string pathCielo, std::string pathTierra);
	void dibujarFondo(Agua* agua);
	int dibujarPaqueteFigura(structFigura paquete);
	int dibujarPaquetePersonaje(structPersonaje paquete);
	SDL_Texture* RenderText(std::string message, std::string fontFile,  SDL_Color color, int fontSize);

	void dibujarPaquete(structPaquete* paquete);


protected:
	Escalador* escalador;
	SDL_Renderer *renderizador;
	float32 escalado_x, escalado_y;

private:
	void actualizar();
	SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);
	SDL_Texture* textureCielo;
	SDL_Texture* textureAgua;
	SDL_Texture* textureTierra;
};


#endif /* DIBUJADOR_H_ */
