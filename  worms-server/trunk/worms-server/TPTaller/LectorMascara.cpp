#include "LectorMascara.h"

LectorMascara::LectorMascara(string& path) {

	this->superficie = IMG_Load( path.c_str() );
	this->path = path;
	if(this->superficie == NULL){
		loguear();
		logFile << "    Error   " <<"\t No se puede cargar la máscara '"<<path<<"'. Se carga '"<<pathDefMas<<"' por defecto." << endl;
		this->superficie = IMG_Load(pathDefMas.c_str());
		this->path = pathDefMas;
	}

	this->PIXEL_ANCHO =  superficie->w;

	this->PIXEL_ALTO =  superficie->h;

}

LectorMascara::~LectorMascara() {
	SDL_FreeSurface(this->superficie);
}

Uint32 getpixel2(SDL_Surface *surface, int x, int y){
   int bpp = surface->format->BytesPerPixel;
   Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

   switch(bpp) {
   case 1: //lee 1 byte por pixel
       return *p;

   case 2://lee 2 bytes por pixel
       return *(Uint16 *)p;

   case 3: //lee 3 bytes por pixel
       if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
           return p[0] << 16 | p[1] << 8 | p[2];
       else
           return p[0] | p[1] << 8 | p[2] << 16;

   case 4: //lee 4 bytes por pixel

       return *(Uint32 *)p;

   default:
       return 0;
   }
}

b2Vec2* LectorMascara::LeerMascara(Escalador* escalador){

	Uint32 *pixels = (Uint32 *) superficie->pixels; // Agarra los pixeles de la superficie
	if(!pixels)return NULL;
	int i;

	b2Vec2* tierra_escalada = new b2Vec2[this->PIXEL_ANCHO];
	vector<int> tierra ;
	for (i = 0; i < this->PIXEL_ANCHO; i++){
		tierra.push_back(32000);
	}
	Uint32 negro = 0 ;
	Uint32 transparente = 0;
	if (pixels[0] == transparente){
	}
	if ( this->superficie->format->BytesPerPixel == 3){
		negro = 0;
	}
	if ( superficie->format->BytesPerPixel == 4){
			negro = 0xff000000;
	}
	for (i = 0; i < this->PIXEL_ANCHO; i++){
			tierra_escalada[i].x = i * (escalador->getEscalaX()/ escalador->getPixelX());
			tierra_escalada[i].y = escalador->getEscalaY();
	}
	for (int i = 0; i<this->PIXEL_ANCHO ; i++){
		int j;
		for(j= 0 ; j< PIXEL_ALTO; j++){
			if ( (getpixel2(superficie, i, j ) != transparente)  && (tierra[i] > (j)) ){
				tierra[i] = j;
				tierra_escalada[i].x = i * (escalador->getEscalaX() / this->PIXEL_ANCHO) ;
				tierra_escalada[i].y = j * (escalador->getEscalaY() / this->PIXEL_ALTO ) ;
			}
		}
	}
	for (i = 0; i < this->PIXEL_ANCHO; i++){
		if (tierra_escalada[i].y == (escalador->getEscalaY())){
			tierra_escalada[i].y = (escalador->getEscalaY()) *1.5;
		}
	}
	return tierra_escalada; //ver delete todo
}

SDL_Surface* LectorMascara :: GetSurface(){
	return this->superficie;
}

int LectorMascara::GetPixelAncho(){
	return this->PIXEL_ANCHO;
}
int LectorMascara::GetPixelAlto(){
	return this->PIXEL_ALTO;
}

void LectorMascara::setMascaraPorDefecto(){
	this->superficie = IMG_Load( pathDefMas.c_str());
	this->path = pathDefMas;
	if(this->superficie == NULL){
		loguear();
		logFile << "    Error   " <<"\t No se puede cargar la máscara. " << endl;
	}
	this->PIXEL_ANCHO =  superficie->w;
	this->PIXEL_ALTO =  superficie->h;
}

string LectorMascara::getPath(){
	return this->path;
}
