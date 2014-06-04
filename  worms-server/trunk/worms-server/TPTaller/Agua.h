#ifndef AGUA_H_
#define AGUA_H_

#include <SDL2/SDL_pixels.h>

#include "LectorMascara.h"

class Agua {
	public:
		Agua(float32 nivel_agua, string &path);
		float32 GetNivel();
		SDL_Surface* GetSurface();
		string GetPath();
		~Agua();

	protected:
		SDL_Surface* surface;
		float32 nivel;
		string path;
};

#endif /* AGUA_H */
