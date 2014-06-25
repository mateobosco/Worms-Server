#include "Dibujador.h"
#include "circulo.h"
#include "poligono.h"
#include "rectangulo.h"
#include "Personaje.h"
#include "Paquete.h"
#include "Juego.h"


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
	this->texturederecha = NULL;
	this->textureizquierda = NULL;
	this->texturederechaNEGRO = NULL;
	this->textureizquierdaNEGRO = NULL;
	this->flechitaroja = NULL;
	this->window = NULL;
	this->contador_cerrarse = 10;
}

Dibujador::Dibujador(SDL_Renderer* renderer, Escalador* esc){
	this->escalador = esc;
	this->window=NULL;
	this->renderizador = renderer;
	this->escalado_x = esc->getEscalaX();
	this->escalado_y = esc->getEscalaY();
	this->textureCielo = NULL;
	this->textureAgua = NULL;
	this->textureTierra = NULL;
	this->texturederecha = NULL;
	this->textureizquierda = NULL;
	this->texturederechaNEGRO = NULL;
	this->textureizquierdaNEGRO = NULL;
	this->flechitaroja = NULL;
	this->contador_cerrarse = 10;
}

Dibujador::~Dibujador(){
	if(this->escalador) delete this->escalador; this->escalador = NULL;
	if(this->textureAgua) SDL_DestroyTexture(this->textureAgua); this->textureAgua = NULL;
	if(this->textureCielo) SDL_DestroyTexture(this->textureCielo); this->textureCielo = NULL;
	if (this->textureTierra) SDL_DestroyTexture(textureTierra); this->textureTierra = NULL;
	if (this->texturederecha) SDL_DestroyTexture(texturederecha); this->texturederecha = NULL;
	if (this->textureizquierda) SDL_DestroyTexture(textureizquierda); this->textureizquierda = NULL;
	if (this->texturederechaNEGRO) SDL_DestroyTexture(texturederechaNEGRO); this->texturederechaNEGRO = NULL;
	if (this->textureizquierdaNEGRO) SDL_DestroyTexture(textureizquierdaNEGRO); this->textureizquierdaNEGRO = NULL;
	if (this->surfaceTierra) SDL_FreeSurface(this->surfaceTierra); this->surfaceTierra = NULL;
	this->close();
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
                        SDL_Color color, int fontSize){
	if (TTF_Init() == -1){
	    std::cout << TTF_GetError() << std::endl;
	}
	TTF_Font *font = nullptr;
	font = TTF_OpenFont(fontFile.c_str(), fontSize);
	if (font == nullptr)
        throw std::runtime_error("Error al cargar font: " + fontFile + TTF_GetError());
    SDL_Surface* surf = TTF_RenderText_Blended(font, message.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(this->renderizador, surf);
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
    return texture;
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
	SDL_Texture *image;
	std::string nombre_jugador;
	SDL_Color color = { 0, 0, 0 };

	if (personaje->getSeleccionado()){
		nombre_jugador = "Juan";

		image = RenderText( nombre_jugador, "TPTaller/imagenes/abecedarionegrita.ttf", color, 52); // despues preguntar el nombre de cada uno

		renderTexture2(image, this->renderizador, x - 30 ,y - 70 , 80, 80 );
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
	SDL_Texture *background = loadTextureTierra(path.c_str(), this->renderizador);
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

SDL_Texture* Dibujador::loadTexture(const std::string &path, SDL_Renderer *ren){
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface == NULL){
        loguear();
        logFile << "No se pudo cargar la imagen: " << path.c_str() << "! SDL_image Error: " <<  IMG_GetError() << endl;
    } else{
        newTexture = SDL_CreateTextureFromSurface(ren, loadedSurface);
        if(newTexture == NULL){
        	loguear();
        	logFile << "No se pudo crear textura de: " << path.c_str() << "! SDL Error: " <<  SDL_GetError() << endl;
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

SDL_Texture* Dibujador::loadTextureTierra(const std::string &path, SDL_Renderer *ren){
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    this->surfaceTierra = loadedSurface;

    if(loadedSurface == NULL){
        loguear();
        logFile << "No se pudo cargar la imagen: " << path.c_str() << "! SDL_image Error: " <<  IMG_GetError() << endl;
    } else{
        newTexture = SDL_CreateTextureFromSurface(ren, loadedSurface);
        if(newTexture == NULL){
        	loguear();
        	logFile << "No se pudo crear textura de: " << path.c_str() << "! SDL Error: " <<  SDL_GetError() << endl;
        }
    }
    return newTexture;
}

void Dibujador::iniciarFondo(Agua* agua, std::string pathCielo, std::string pathTierra){
	float nivelAgua = escalador->aplicarZoomY(agua->GetNivel()) - escalador->getOffsetY();
	this->dibujarAgua(escalador, agua);
	this->dibujar_cielo(escalador, pathCielo, nivelAgua);
	this->dibujar_tierra(escalador, pathTierra);
}



 class QueryDibujarTierraEdge : public b2QueryCallback {
  public:
      std::vector<b2Shape*> foundBodies;

      bool ReportFixture(b2Fixture* fixture) {
    	  b2Shape* shape= fixture->GetShape();

    	  if (shape->GetType() == 1){ // b2EdgeShape == 1
    		  foundBodies.push_back( shape );
    	  }
          return true;//keep going to find all fixtures in the query area
      }
  };


 class QueryViento : public b2QueryCallback {
  public:
      bool tocando;

      bool ReportFixture(b2Fixture* fixture) {
		  tocando = true;
          return true;//keep going to find all fixtures in the query area
      }
  };

void Dibujador::dibujarFondo(Agua* agua, Juego* juego){
	this->renderTexture(textureCielo, renderizador,0 , 0, escalador->getPixelX(), escalador->getPixelY() );
	this->renderTexture(textureAgua, renderizador, 0, agua->GetNivel()*(escalador->getPixelY()/escalador->getEscalaY()) , escalador->getPixelX(), escalador->getPixelY());
	this->renderTexture(textureTierra, renderizador, 0 , 0, escalador->getPixelX() , escalador->getPixelY());
//	this->dibujarTierraEdge(juego);
//	this->dibujarTierraPoligono(juego);
	this->dibujarTierraChain(juego);


}

void Dibujador::dibujarTierraEdge(Juego* juego){
	QueryDibujarTierraEdge query;
	b2AABB aabb;
	aabb.upperBound = b2Vec2(escalador->getEscalaX(),escalador->getEscalaY());
	aabb.lowerBound = b2Vec2(0,0);

	b2World* world = juego->getMundo()->devolver_world();
	world->QueryAABB(&query, aabb);

	std::vector<b2Shape*> dibujables = query.foundBodies;


	for (size_t i = 0 ; i<dibujables.size() ; i++){
		b2Shape* shape = dibujables[i];
		b2EdgeShape* edge = (b2EdgeShape*) shape;
		b2Vec2 escalado1 = edge->m_vertex2;
		b2Vec2* aux1 = escalador->aplicarZoomPosicion(escalado1);
		b2Vec2 escalado2 = edge->m_vertex1;
		b2Vec2* aux2 = escalador->aplicarZoomPosicion(escalado2);
		b2Vec2 pixelado1 = *aux1;
		b2Vec2 pixelado2 = *aux2;

		SDL_RenderDrawLine(renderizador,pixelado1.x, pixelado1.y, pixelado2.x, pixelado2.y);

	}
}


class QueryDibujarTierraPoligono : public b2QueryCallback {
 public:
     std::vector<b2Body*> foundBodies;

     bool ReportFixture(b2Fixture* fixture) {
   	  b2Shape* shape= fixture->GetShape();
   	  b2Body* body = fixture->GetBody();

   	  if ((shape->GetType() == 2) && (body->GetType() == 0 )){ // b2PolygonShape == 2 && b2StaticBody == 0
   		  foundBodies.push_back( body );
   	  }
         return true;//keep going to find all fixtures in the query area
     }
 };

class QueryDibujarTierraChain : public b2QueryCallback {
 public:
     std::vector<b2Body*> foundBodies;

     bool ReportFixture(b2Fixture* fixture) {
   	  b2Shape* shape= fixture->GetShape();
   	  b2Body* body = fixture->GetBody();

   	  if ((shape->GetType() == 1)){ // b2PolygonShape == 2 && b2StaticBody == 0
   		  foundBodies.push_back( body );
   	  }
         return true;//keep going to find all fixtures in the query area
     }
 };

void Dibujador::dibujarTierraPoligono(Juego* juego){
	QueryDibujarTierraPoligono query;
	b2AABB aabb;
	aabb.upperBound = b2Vec2(escalador->getEscalaX(),escalador->getEscalaY());
	aabb.lowerBound = b2Vec2(0,0);

	b2World* world = juego->getMundo()->devolver_world();
	world->QueryAABB(&query, aabb);

	std::vector<b2Body*> dibujables = query.foundBodies;


	for (size_t i = 0 ; i<dibujables.size() ; i++){
		b2Body* body = dibujables[i];
		b2Shape* shape = body->GetFixtureList()->GetShape();
		b2PolygonShape* poli = (b2PolygonShape*) shape;
		b2Vec2 escalado0 = body->GetWorldPoint(poli->GetVertex(0));
		b2Vec2 escalado1 = body->GetWorldPoint(poli->GetVertex(1));
		b2Vec2 escalado2 = body->GetWorldPoint(poli->GetVertex(2));
		b2Vec2 escalado3 = body->GetWorldPoint(poli->GetVertex(3));


		b2Vec2* aux0 = escalador->aplicarZoomPosicion(escalado0);
		b2Vec2* aux1 = escalador->aplicarZoomPosicion(escalado1);
		b2Vec2* aux2= escalador->aplicarZoomPosicion(escalado2);
		b2Vec2* aux3 = escalador->aplicarZoomPosicion(escalado3);

		b2Vec2 pixelado0 = *aux0;
		b2Vec2 pixelado1 = *aux1;
		b2Vec2 pixelado2 = *aux2;
		b2Vec2 pixelado3 = *aux3;

		SDL_RenderDrawLine(renderizador,pixelado0.x, pixelado0.y, pixelado1.x, pixelado1.y);
		SDL_RenderDrawLine(renderizador,pixelado1.x, pixelado1.y, pixelado2.x, pixelado2.y);
		SDL_RenderDrawLine(renderizador,pixelado2.x, pixelado2.y, pixelado3.x, pixelado3.y);
		SDL_RenderDrawLine(renderizador,pixelado3.x, pixelado3.y, pixelado0.x, pixelado0.y);


	}

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
		delete posicionVentanada;

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

void Dibujador::dibujarPaquetePersonaje(structPersonaje paquete, char* nombre_jugador, bool duenio, int cliente_id, float aux){

	int dir = paquete.direccion;
	b2Vec2 tam = paquete.tamano;

	SDL_Texture* gusanito;
	if (dir ==1){
		gusanito = this->texturederecha;
	}
	if (dir == -1){
		gusanito = this->textureizquierda;
	}
	if (dir ==1 && paquete.conectado == 0){
		gusanito = this->texturederechaNEGRO;
	}
	if (dir == -1 && paquete.conectado == 0){
		gusanito = this->textureizquierdaNEGRO;
	}

	b2Vec2 posicion = paquete.posicion;
	b2Vec2* posicionVentanada = escalador->aplicarZoomPosicion(posicion);
	int anchoPX = escalador->aplicarZoomX( tam.x);
	int altoPX = escalador->aplicarZoomY(tam.y);
	int x = posicionVentanada->x - anchoPX/2;
	int y = posicionVentanada->y - altoPX/2;
	int w = anchoPX;
	int h = altoPX;
	SDL_Texture *image;
	SDL_Color vectorcolores[4];
	vectorcolores[0] = { 0, 0, 0 };
	vectorcolores[1]= { 255, 0, 0 };
	vectorcolores[2] = { 0, 255, 0 };
	vectorcolores[3] = { 0, 0, 255 };

	std::string nombre_a_imprimir = string(paquete.nombre_cliente);
	image = RenderText(paquete.nombre_cliente, "TPTaller/imagenes/Hilarious.ttf", vectorcolores[paquete.id_jugador], 20); // despues preguntar el nombre de cada uno
	renderTexture2(image, this->renderizador, x - 30 ,y - 30 , 80 , 30 );
	if (image) SDL_DestroyTexture(image);
	if(paquete.seleccionado[cliente_id] == 1){
		this->
		renderTexture2(flechitaroja, this->renderizador,(x - 30), ((y)*aux/100)+(y-120), 80, 80);
	}
	renderTexture2(gusanito, this->renderizador, x ,y ,w , h );

	delete posicionVentanada;
}

void Dibujador::dibujarPaquete(structPaquete* paquete, char* nombre_cliente, int cliente_id, float aux){
//	int figuras = paquete->cantidad_figuras;
	int personajes = paquete->cantidad_personajes;
//	for (int i = 0 ; i < figuras ; i++ ){
//		structFigura* vector = paquete->vector_figuras;
//		this->dibujarPaqueteFigura(vector[i]);
//	}
	SDL_Color color = {0,0,0};
	SDL_Texture* nombre = RenderText(nombre_cliente, "TPTaller/imagenes/Hilarious.ttf", color, 20); // despues preguntar el nombre de cada uno
	renderTexture2(nombre, this->renderizador, 0 , 0 , 100, 30 );
	if (paquete->mensaje_mostrar != NULL){
		SDL_Texture* cartel = RenderText(paquete->mensaje_mostrar, "TPTaller/imagenes/Hilarious.ttf", color, 20); // despues preguntar el nombre de cada uno
		renderTexture2(cartel, this->renderizador, 0 , this->escalado_x/2 , 200, 30 );
		SDL_DestroyTexture(cartel);
	}
	structPersonaje* vector1 = paquete->vector_personajes;
	for (int j = 0 ; j < personajes ; j ++){
		if (cliente_id == vector1[j].id_jugador){
			this->dibujarPaquetePersonaje(vector1[j], nombre_cliente, true, cliente_id, aux ); // es propio
		} else{
			this->dibujarPaquetePersonaje(vector1[j], nombre_cliente, false, cliente_id, aux); // no es propio
		}
	}
}

bool Dibujador::init(){
	bool success = true;
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		loguear();
		logFile << "    Error   " << "\t  SDL No pudo inicializar! SDL Error: " <<  SDL_GetError()<< endl;
		success = false;
	} else{
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")){
			loguear();
			logFile << "    Warning " << "\t  Linear texture filtering no habilitado! " <<  endl;
			success = false;
		}
		this->window = SDL_CreateWindow("WORMS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, escalador->getVentanaX() , escalador->getVentanaY(), SDL_WINDOW_SHOWN);
		if(this->window == NULL){
			loguear();
			logFile << "    Error   " << "\t  La ventana no pudo ser creada! SDL Error: " <<  SDL_GetError()<< endl;
			success = false;
		}
		else{
			this->renderizador = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if(this->renderizador == NULL)	{
				loguear();
				logFile << "    Error   " << "\t  Renderer no pudo ser creado! SDL Error: " <<  SDL_GetError()<< endl;
				success = false;
			} else{
				SDL_SetRenderDrawColor(this->renderizador, 0xFF, 0xFF, 0xFF, 0xFF);
				int imgFlags = (IMG_INIT_PNG |IMG_INIT_JPG); //
				int iniciadas = IMG_Init(imgFlags);
				if((iniciadas & imgFlags) != imgFlags){
					loguear();
					logFile << "    Error   " << "\t  SDL_image no puedo ser inicializado! SDL_image Error: " <<  SDL_GetError()<< endl;
					success = false;
				}
			}
		}
	}
	this->texturederecha = loadTexture("TPTaller/imagenes/gusanitoderecha.png", this->renderizador);
	this->textureizquierda = loadTexture("TPTaller/imagenes/gusanitoizquierda.png", this->renderizador);
	this->texturederechaNEGRO = loadTexture("TPTaller/imagenes/gusanitonegroder.png", this->renderizador);
	this->textureizquierdaNEGRO = loadTexture("TPTaller/imagenes/gusanitonegroizq.png" , this->renderizador);
	this->flechitaroja = loadTexture("TPTaller/imagenes/flechitaroja.png", this->renderizador);
	return success;
}

void Dibujador::close(){
	SDL_DestroyRenderer(renderizador);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Dibujador::dibujarMensaje(){
	char mensaje[90];
	sprintf (mensaje, "Se cerro el server, la ventana se cerrara en: %d segundos", this->contador_cerrarse);
	this->contador_cerrarse--;
	SDL_Delay(1000);
	SDL_Color color = {0,0,0};
	SDL_Texture* mensaje_final = RenderText(mensaje, "TPTaller/imagenes/Hilarious.ttf", color, 60);
	renderTexture2(mensaje_final, this->renderizador, (this->escalado_x / 2)*2.5  , (this->escalado_y /2)*6 , 500, 50 );
	SDL_DestroyTexture(mensaje_final);
}

int Dibujador::getContadorCerrarse(){
	return this->contador_cerrarse;
}

class QueryDibujarTierraConChain : public b2QueryCallback {
 public:
     std::vector<b2ChainShape*> foundBodies;

     bool ReportFixture(b2Fixture* fixture) {
   	  b2Shape* shape= fixture->GetShape();

   	  if (shape->GetType() == 3){ // b2ChainShape == 3
   		  b2ChainShape* chain = (b2ChainShape*) shape;
		  vector<b2ChainShape*>::iterator it = std::find(foundBodies.begin(), foundBodies.end(), chain);
		  if(it==foundBodies.end()){
			  foundBodies.push_back( chain );
		  }
   	  }
         return true;//keep going to find all fixtures in the query area
     }
 };

void Dibujador::dibujarTierraChain(Juego* juego){

	QueryDibujarTierraConChain query;
	b2AABB aabb;
	aabb.upperBound = b2Vec2(escalador->getEscalaX(),escalador->getEscalaY());
	aabb.lowerBound = b2Vec2(0,0);

	b2World* world = juego->getMundo()->devolver_world();
	world->QueryAABB(&query, aabb);

	std::vector<b2ChainShape*> dibujables = query.foundBodies;

//	 Mundo* mundo = juego->getMundo()
	if (dibujables.size()==0) return;

	for (size_t k = 0; k < dibujables.size(); k++){

		b2ChainShape* shapeOriginal = dibujables[k];
		b2Vec2* verticesOriginal = shapeOriginal->m_vertices;
		int cantidadOriginal = shapeOriginal->m_count;

		for (int i = 0 ; i<cantidadOriginal-1 ; i++){

			b2Vec2 escalado1 = verticesOriginal[i];
			b2Vec2* aux1 = escalador->aplicarZoomPosicion(escalado1);
			b2Vec2 escalado2 = verticesOriginal[i+1];
			b2Vec2* aux2 = escalador->aplicarZoomPosicion(escalado2);
			b2Vec2 pixelado1 = *aux1;
			b2Vec2 pixelado2 = *aux2;

			SDL_RenderDrawLine(renderizador,pixelado1.x, pixelado1.y, pixelado2.x, pixelado2.y);

		}

	}

}



void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}


void Dibujador::borrarExplosion(b2Vec2 posicion, float32 radio){

	SDL_Surface* surf = this->surfaceTierra;
	int alto = surf->h;
	int ancho = surf->w;
	Uint32* pixeles = (Uint32*) surf->pixels;

	int pos[2];
	pos[0] = posicion.x * (ancho / escalador->getEscalaX());
	pos[1] = posicion.y * (alto / escalador->getEscalaY());

	int rad[2];
	rad[0] = radio * (ancho / escalador->getEscalaX());
	rad[1] = radio * (alto / escalador->getEscalaY());

	int pixel[2];

	for (int i= 0-rad[0]; i<rad[0]; i++){
		for (int j=0-rad[1]; j<rad[1]; j++){
			pixel[0] = pos[0] + i;
			pixel[1] = pos[1] + j;

			float32 x = (( ((float32) i*i) / ((float32) rad[0]*rad[0])));
			float32 y = (( ((float32) j*j) / ((float32) rad[1]*rad[1])));
			if ( (x+y) <= 1) {
				putpixel(surfaceTierra,pixel[0],pixel[1],0);
			}
		}
	}
	SDL_UpdateTexture(this->textureTierra,NULL,pixeles,this->surfaceTierra->pitch);

}
