#ifndef FUNCIONES_H_
#define FUNCIONES_H_

#include <yaml-cpp/yaml.h>

#include "Dibujador.h"
#include "Cargador.h"
#include "ManejadorPersonajes.h"

ofstream logFile;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
bool KEYS[322];

bool init(int ancho, int alto){
	bool success = true;
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		loguear();
		logFile << "    Error   " << "\t  SDL No pudo inicializar! SDL Error: " <<  SDL_GetError()<< endl;
		success = false;
	}
	else{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ){
			loguear();
			logFile << "    Warning " << "\t  Linear texture filtering no habilitado! " <<  endl;
			success = false;
		}
		gWindow = SDL_CreateWindow( "WORMS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ancho , alto, SDL_WINDOW_SHOWN );
		if( gWindow == NULL ){
			loguear();
			logFile << "    Error   " << "\t  La ventana no pudo ser creada! SDL Error: " <<  SDL_GetError()<< endl;
			success = false;
		}
		else{
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )	{
				loguear();
				logFile << "    Error   " << "\t  Renderer no pudo ser creado! SDL Error: " <<  SDL_GetError()<< endl;
				success = false;
			}
			else{
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				int imgFlags = (IMG_INIT_PNG |IMG_INIT_JPG ); //
				int iniciadas = IMG_Init( imgFlags );
				if((iniciadas & imgFlags) != imgFlags ){
					loguear();
					logFile << "    Error   " << "\t  SDL_image no puedo ser inicializado! SDL_image Error: " <<  SDL_GetError()<< endl;
					success = false;
				}
			}
		}
	}
	return success;
}

//Retorna -1 en caso de error.
int crearVentana(Escalador *escalador){
	if(!init(escalador->getVentanaX(),escalador->getVentanaY())){
		loguear();
		logFile <<"    Error   " << "\t  Inicialización inválida " << endl;
		return -1;
	}
	return 0;
}

//Genera la fecha y la hora obtenida desde el sistema.
void generarFechaYHora(char* fecha, char* hora){
	char* anio = new char[5];
	memset(anio,'\0',5);
	char* mes = new char[3];
	memset(mes,'\0',3);
	char* dia = new char[3];
	memset(dia,'\0',3);
	char* horas = new char[3];
	memset(horas,'\0',3);
	char* minutos = new char[3];
	memset(minutos,'\0',3);

	time_t tiempo = time(NULL);
	struct tm* info_tiempo = localtime(&tiempo);
	strftime(anio, sizeof(anio), "%Y", info_tiempo);
	strftime(mes, sizeof(mes), "%m", info_tiempo);
	strftime(dia, sizeof(dia), "%d", info_tiempo);
	strftime(horas, sizeof(horas), "%H", info_tiempo);
	strftime(minutos, sizeof(minutos), "%M", info_tiempo);

	memcpy(fecha, dia,3);
	strcat(fecha,"/");
	strcat(fecha, mes);
	strcat(fecha,"/");
	strcat(fecha, anio);
	memcpy(hora,horas,3);
	strcat(hora,":");
	strcat(hora, minutos);

	delete[] anio;
	delete[] mes;
	delete[] dia;
	delete[] horas;
	delete[] minutos;
}

void loguear(){
	char* fecha = new char[12];
	char* hora = new char[8];
	generarFechaYHora(fecha,hora);
	logFile << fecha << " " << hora ;
	delete[] fecha;
	delete[] hora;
}


void keyboard(SDL_Event event , int* posicion_mouse_movimiento, int* posicion_mouse_click, int* posicion_mouse_wheel) {
	int numero;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			break;
		case SDL_KEYDOWN:
			numero = event.key.keysym.sym;
			if (numero == SDLK_RIGHT){
				KEYS[100] = true;
				break;
			}
			if (numero == SDLK_LEFT){
				KEYS[101] = true;
				break;
			}
			if (numero == SDLK_UP){
				KEYS[102] = true;
				break;
			}
			if (event.key.keysym.sym == SDLK_DOWN){
				KEYS[103] = true;
				break;
			}
			if(numero < 1000){
				//printf("ENTRA ACAAAA 4 \n");
				KEYS[numero] = true;
			}
			break;
		case SDL_KEYUP:
           if (event.key.keysym.sym == SDLK_RIGHT){
				KEYS[100] = false;
				break;
			}
			if (event.key.keysym.sym == SDLK_LEFT){
				KEYS[101] = false;
				break;
			}
			if (event.key.keysym.sym == SDLK_UP){
				KEYS[102] = false;
				break;
			}
			if (event.key.keysym.sym == SDLK_DOWN){
				KEYS[103] = false;
				break;
			}

			KEYS[event.key.keysym.sym] = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			int x, y;
			SDL_GetMouseState( &x, &y );
			posicion_mouse_click[0] = x;
			posicion_mouse_click[1] = y;
			//printf("LA posicion detectada fue %d, %d \n ", x,y);
			break;
		case SDL_MOUSEMOTION:
			int w, z;
			SDL_GetMouseState( &w, &z );
			posicion_mouse_movimiento[0] = w;
			posicion_mouse_movimiento[1] = z;
			//printf("LA posicion detectada fue %d, %d \n ", w,z);
			break;
		case SDL_MOUSEWHEEL:
			int a, b,d;
			SDL_GetMouseState( &a, &b );
			SDL_MouseWheelEvent wheel = event.wheel;
			d = (int) wheel.y;
			posicion_mouse_wheel[0] = a;
			posicion_mouse_wheel[1] = b;
			posicion_mouse_wheel[2] = d;
			break;
			//printf("LA posicion detectada fue (%d, %d),%d, %d  \n ", a,b,c,d);

		//default:
			//break;
		}
   }
}

int checkCantParametros(int cant_parametros){
	if(cant_parametros >= 4){
		return EXIT_SUCCESS;
	}
//	TODO Logear ERROR: Cantidad de parametros insuficientes.
	return EXIT_FAILURE;
}
#endif /* FUNCIONES_H_ */
