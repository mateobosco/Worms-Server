#include "Cargador.h"

#define triangulo 3
#define cuadrado 4
#define rectangulo 4
#define pentagono 5
#define hexagono 6

using namespace std;

Cargador::Cargador(const char* filename){
	this->datos = new structInicial;
	(this->f_input).open(filename,ios::in);
	if(!f_input){
		loguear();
		logFile <<"    Warning "<<"\t El archivo yaml '"<< filename<<"' no se puede cargar. Se carga por defecto el archivo en ubicación:  " << pathDefEs << endl;
		(this->f_input).open(pathDefEs.c_str(),ios::in);
	}
	try{
		this->nodo_inicial = Load(f_input);
	}
	catch(Exception &e){
		(this->f_input).open(pathDefEs.c_str(),ios::in);
		this->nodo_inicial = Load(f_input);
		loguear();
		logFile <<"    Error  " << "\t El escenario no pudo ser cargado en un nodo. Se carga el escenario de '"<<pathDefEs<< "' por defecto." << endl;
	}
}

Cargador::~Cargador(){
	try{
		this->f_input.close();
	}catch(Exception &e){
		loguear();
		logFile << "    Warning" << "\t El archivo de escenario no se pudo cerrar." << endl;
	}
	this->nodo_inicial = NULL;
	//delete this->datos; todo
}

Node Cargador::getNodo(){
	return this->nodo_inicial;
}

bool Cargador::getNodo(const Node nodoO,Node &nodoD, const char* clave){
	try{
		nodoD = nodoO[clave];
		if((nodoD.size()>0)){
			return true;
		}
	}catch(Exception &e){
		if(nodoD.remove(clave))	return false;
	}
	if(nodoD.remove(clave))	return false;
	return false;

}

ifstream* Cargador::getStream(){
	return &(this->f_input);
}

Figura* Cargador::cargarFigura(Node nodo, Mundo* mundo, Escalador* escalador, size_t i){
	Node nodo_aux;
	if((nodo_aux = nodo["tipo"])||(nodo_aux = nodo["t"])||(nodo_aux = nodo["ti"])||(nodo_aux = nodo["tip"])||(nodo_aux = nodo["TIPO"])||
		(nodo_aux = nodo["TI"])||(nodo_aux = nodo["TIP"])){
		try{
			string tipo = nodo_aux.as<string>();
			if((tipo.compare(0,2,"ci") ==0)){
				Circulo* circulo = loadCirculo(nodo, mundo, escalador, i);
				return circulo;
			}else if((tipo.compare(0,1,"r")==0)){
				Rectangulo* rect = loadRectangulo(nodo,mundo, escalador,i);
				return rect;
			}else{
				Poligono* poligono = loadPoligono(nodo, mundo, escalador,i,tipo);
				return poligono;
			}
		}catch(Exception &e){
			Circulo* circulo = loadCirculo(nodo, mundo, escalador,i);
			loguear();
			logFile << "    Warning " <<"\t No se puede identificar figura a cargar según característica tipo. Por defecto se dibuja un círculo para la figura número: "<<i << endl;
			return circulo;
		}
	}else { //SI no esta tipo me fijo si tiene como caracteristica ancho o alto y es un rectangulo

		Node nodo_a;
		if((nodo_a = nodo["ancho"])||(nodo_a = nodo["an"])||(nodo_a = nodo["al"])||(nodo_a = nodo["alto"])){
			Rectangulo* rect = loadRectangulo(nodo,mundo, escalador,i);
			loguear();
			logFile << "    Warning " <<"\t No se puede identificar figura a cargar según característica tipo. Como tiene característica ancho o alto se grafica un rectángulo para la figura número: "<<i<< endl;
			return rect;
		}

		else{ //SIno por default es  un circulo.
			Circulo* circulo = loadCirculo(nodo, mundo, escalador,i);
			loguear();
			logFile << "    Warning " <<"\t No se puede identificar figura a cargar según característica tipo. Por defecto se dibuja un círculo para la figura número: "<<i << endl;
			return circulo;
		}
	}
	return NULL;
}

Agua* Cargador::loadAgua(Node nodo,string pathAgua ){
	Node nodoAux;
	float32 nivelAgua;
	if((nodoAux = nodo["nivel_agua"])||(nodoAux = nodo["agua"])||(nodoAux = nodo["nivel"])||(nodoAux = nodo["nivel-agua"])){
		try{
			nivelAgua = nodoAux.as<float32>();
		}catch(Exception &e){
			loguear();
			logFile << "    Warning " <<"\t No se pudo cargar el nivel de agua. Por defecto seteamos nivel = 40 " << endl;
			nivelAgua = 40;
		}
	}else{
		loguear();
		logFile <<"    Warning " <<"\t No se pudo cargar el nivel de agua. Por defecto seteamos nivel = 40 " << endl;
		nivelAgua = 40;
	}
	Agua* agua = new Agua(nivelAgua,pathAgua);
	strcpy(datos->agua,pathAgua.c_str());
	datos->nivel_agua = nivelAgua;
	return agua;
}

Escalador* Cargador::loadEscalador(Node nodo){
	Node nodoAux;
	float32 anchoPx;
	float32 altoPx;
	float32 anchoUn;
	float32 altoUn;
	int ancho_esc, alto_esc;
	if((nodoAux = nodo["ancho-px"])||(nodoAux = nodo["ancho_px"])||(nodoAux = nodo["ancho-pixeles"])||
			(nodoAux = nodo["ancho_pixeles"])||(nodoAux = nodo["anchopx"])){
		try{
			anchoPx = nodoAux.as<float32>();
		}catch(Exception &e){
			anchoPx = 800.0;
			loguear();
			logFile <<"    Warning " <<"\t No se pudo cargar el ancho de pantalla en pixeles. Por defecto seteamos ancho-px = 800 " << endl;
		}
	}else{
		anchoPx = 800.0;
		loguear();
		logFile <<"    Warning " <<"\t No se pudo cargar el ancho de pantalla en pixeles. Por defecto seteamos ancho-px = 800 " << endl;
	}
	if((nodoAux = nodo["alto-px"])||(nodoAux = nodo["alto_px"])||(nodoAux = nodo["alto-pixeles"])||
						(nodoAux = nodo["alto_pixeles"])||(nodoAux = nodo["altopx"])){
		try{
			altoPx = nodoAux.as<float32>();
		}catch(Exception &e){
			altoPx = 600.0;
			loguear();
			logFile << "    Warning " <<"\t No se pudo cargar el alto de pantalla en pixeles. Por defecto seteamos alto-px = 600 " << endl;
		}
	}else{
		altoPx = 600.0;
		loguear();
		logFile << "    Warning " <<"\t No se pudo cargar el alto de pantalla en pixeles. Por defecto seteamos alto-px = 600 " << endl;
	}
	if((nodoAux = nodo["ancho-un"])||(nodoAux = nodo["ancho_un"])||(nodoAux = nodo["ancho-unidades"])||
								(nodoAux = nodo["ancho_unidades"])||(nodoAux = nodo["anchoun"])){
		try{
			anchoUn = nodoAux.as<float32>();
		}catch(Exception &e){
			anchoUn = 100.0;
			loguear();
			logFile << "    Warning " <<"\t No se pudo cargar el ancho de pantalla en unidades. Por defecto seteamos ancho-un = 100 " << endl;
		}
	}else{
		anchoUn = 100.0;
		loguear();
		logFile << "    Warning " <<"\t No se pudo cargar el ancho de pantalla en unidades. Por defecto seteamos ancho-un = 100 " << endl;
	}
	if((nodoAux = nodo["alto-un"])||(nodoAux = nodo["alto_un"])||(nodoAux = nodo["alto-unidades"])||
								(nodoAux = nodo["alto_unidades"])||(nodoAux = nodo["altoun"])){
		try{
			altoUn = nodoAux.as<float32>();
		}catch(Exception &e){
			altoUn = 100.0;
			loguear();
			logFile << "    Warning " <<"\t No se pudo cargar el alto de pantalla en unidades. Por defecto seteamos alto-un = 80 " << endl;
		}
	}else{
		altoUn = 100.0;
		loguear();
		logFile << "    Warning " <<"\t No se pudo cargar el alto de pantalla en unidades. Por defecto seteamos alto-un = 80 " << endl;
	}

	if((nodoAux = nodo["ancho-esc"])||(nodoAux = nodo["ancho_esc"])||(nodoAux = nodo["ancho-escenario"])||
								(nodoAux = nodo["ancho_escenario"])||(nodoAux = nodo["anchoesc"])){
		try{
			ancho_esc = nodoAux.as<int>();
		}catch(Exception &e){
			ancho_esc = ANCHO_ESC;
			loguear();
			logFile << "    Warning " <<"\t No se pudo cargar el alto del escenario en pixeles. Por defecto seteamos alto-esc = 1000 " << endl;
		}
	}else{
		ancho_esc = ANCHO_ESC;
		loguear();
		logFile << "    Warning " <<"\t No se pudo cargar el alto del escenario en pixeless. Por defecto seteamos alto-esc = 1000 " << endl;
	}

	if((nodoAux = nodo["alto-esc"])||(nodoAux = nodo["alto_esc"])||(nodoAux = nodo["alto-escenario"])||
								(nodoAux = nodo["alto_escenario"])||(nodoAux = nodo["altoesc"])){
		try{
			alto_esc = nodoAux.as<int>();
		}catch(Exception &e){
			alto_esc = ALTO_ESC;
			loguear();
			logFile << "    Warning " <<"\t No se pudo cargar el alto del escenario en pixeles. Por defecto seteamos alto-esc = 1000 " << endl;
		}
	}else{
		alto_esc = ALTO_ESC;
		loguear();
		logFile << "    Warning " <<"\t No se pudo cargar el alto del escenario en pixeless. Por defecto seteamos alto-esc = 1000 " << endl;
	}

	Escalador* escalador = new Escalador((int)anchoPx,(int)altoPx,anchoUn,altoUn,(int) ancho_esc, (int) alto_esc);
	datos->ancho_unidades = anchoUn;
	datos->alto_unidades = altoUn;
	datos->ancho_ventana = (int) anchoPx;
	datos->alto_ventana = (int) altoPx;
	datos->ancho_escenario = (int) ancho_esc;
	datos->alto_escenario = (int) alto_esc;

	return escalador;

}

bool Cargador::loadPath(Node nodo,const char* clave,string &resultado){
	string path;
	try{
		path = nodo[clave].as<string>();
		resultado = path;
		if(strcmp(clave, "tierra") == 0){
			strcpy(datos->tierra,path.c_str());
		} else{
			strcpy(datos->cielo,path.c_str());
		}
		return true;
	}catch(Exception &e){
		return false;
	}
}

Rectangulo* Cargador::loadRectangulo(Node nodo, Mundo* mundo, Escalador *escalador, size_t i){ //size_t i se usa para identificar que num de elemento es, para el log
	b2Vec2 posicion = obtenerPosicion(nodo, i);
	bool estatico = obtenerEstatico(nodo,i);
	float32 ancho = obtenerAncho(nodo,i);
	float32 alto = obtenerAlto(nodo,i);
	SDL_Color color = obtenerColor(nodo,i);
	float32 masa = obtenerMasa(nodo,i);
	float32 rotacion = obtenerRotacion(nodo,i);
	Rectangulo* rect = new Rectangulo(mundo, posicion,ancho,alto,estatico,color,masa,rotacion);
	if (mundo->checkOverlap(rect, escalador,i)){
		return rect;
	} else{
		delete rect;
		return NULL;
	}
}

Circulo* Cargador::loadCirculo (Node nodo, Mundo* mundo, Escalador *escalador,size_t i){
	b2Vec2 posicion = obtenerPosicion(nodo,i);
	bool estatico = obtenerEstatico(nodo,i);
	SDL_Color color = obtenerColor(nodo,i);
	float32 masa = obtenerMasa(nodo,i);
	float32 rotacion = obtenerRotacion(nodo,i);
	float32 escala = obtenerEscala(nodo,i);
	Circulo* circulo = new Circulo(mundo, posicion,(int)escala, estatico, color, masa, rotacion);
	if (mundo->checkOverlap(circulo, escalador,i)){
		return circulo;
	} else{
		delete circulo;
		return NULL;
	}
}

Poligono* Cargador::loadPoligono (Node nodo, Mundo* mundo, Escalador *escalador, size_t i,string tipo){
	b2Vec2 posicion = obtenerPosicion(nodo,i);
	bool estatico = obtenerEstatico(nodo,i);
	int cantLados = this->cantidadLados(nodo,i,tipo);
	float32 escala = obtenerEscala(nodo,i);
	SDL_Color color = obtenerColor(nodo,i);
	float32 masa = obtenerMasa(nodo,i) ;
	float32 rotacion = obtenerRotacion(nodo,i);
	Poligono* poligono = new Poligono(mundo, posicion, escala, cantLados,estatico,color, masa,rotacion);
	if (mundo->checkOverlap(poligono, escalador,i)){
		return poligono;
	} else{
		delete poligono;
		return NULL;
	}
}

b2Vec2 Cargador::obtenerPosicion(Node nodo, size_t i){
	float32 posX,posY;
	Node x,y;
	b2Vec2 posicion;
	if((x = nodo["x"]) || (x = nodo["X"])){
		try{
			posX = x.as<float32>();
		}catch(Exception &e){
			posX = 5;
			loguear();
			logFile << "    Warning " <<"\t No se pudo encontrar la posición X del objeto número: " << i<<". Por defecto seteamos x = 5 " << endl;
		}
	}
	else{
		posX = 5; //posicion por defecto
		loguear();
		logFile << "    Warning " <<"\t No se pudo encontrar la posición X del objeto número: " << i<<". Por defecto seteamos x = 5 " << endl;
	}
	if((y = nodo["y"]) || (y = nodo["Y"])){
		try{
			posY = y.as<float32>();
		}catch(Exception &e){
			posY = 1; //posicion por defecto
			loguear();
			logFile << "    Warning " <<"\t No se pudo encontrar la posición y del objeto número: "<<i<<". Por defecto seteamos y = 1 " << endl;
		}
	}
	else{
		posY = 1; //posicion por defecto
		loguear();
		logFile << "    Warning " <<"\t No se pudo encontrar la posición y del objeto número: "<<i<<". Por defecto seteamos y = 1 " << endl;
	}
	posicion = b2Vec2(posX,posY);
	return posicion;
}

void Cargador::minuscula(string &cadena){
	transform(cadena.begin(), cadena.end(), cadena.begin(), ::tolower);
}

bool Cargador::obtenerEstatico(Node nodo, size_t i){
	string estatico;
	Node nodo_estatico;
	bool estatic;
	if((nodo_estatico = nodo["estatico"]) ||(nodo_estatico = nodo["Estatico"])||(nodo_estatico = nodo["ESTATICO"])||(nodo_estatico = nodo["est"])){
		try{
			estatico = nodo_estatico.as<string>();
		}catch(Exception &e){
			estatic = true; //Si es estático o no se encuentra algo válido setea true
			loguear();
			logFile << "    Warning " <<"\t No se pudo identificar si el objeto número: "<<i<<" es estático. Por defecto es estático " << endl;
		}
	}
	minuscula(estatico);
	if((estatico.compare("no")==0) || (estatico.compare("n")==0) ){
			estatic = false;
	}else if((estatico.compare("yes")==0) || (estatico.compare("y")==0)|| (estatico.compare("s")==0) || (estatico.compare("si")==0)){
		estatic = true;
	}else{
		estatic = true; //Si es estático o no se encuentra algo válido setea true
		loguear();
		logFile << "    Warning " <<"\t No se pudo identificar si el objeto número: "<<i<<" es estático. Por defecto es estático " << endl;
	}
	return estatic;
}

float32 Cargador::obtenerEscala(Node nodo,size_t i){
	Node nodo_escala;
	float32 escala;
	if((nodo_escala = nodo["escala"])||(nodo_escala = nodo["Escala"])||(nodo_escala = nodo["ESCALA"])||(nodo_escala = nodo["esc"])||(nodo_escala = nodo["ESC"])){
		try{
			escala = nodo_escala.as<float32>();
		}catch(Exception &e){
			escala = 1.0;
			loguear();
			logFile << "    Warning " <<"\t No se pudo encontrar la escala del objeto número: "<< i<<". Por defecto seteamos escala = 1 " << endl;
		}
	}else{ //Si no encuentra la clave escala por defecto selecciona 1.
		escala = 1.0;
		loguear();
		logFile << "    Warning " <<"\t No se pudo encontrar la escala del objeto número: "<< i<<". Por defecto seteamos escala = 1 " << endl;
	}
	return escala;

}

SDL_Color Cargador::obtenerColor(Node nodo,size_t i){
	Node nodo_color;
	SDL_Color rgbColor;
	string color;
	if((nodo_color = nodo["color"])||(nodo_color = nodo["co"])||(nodo_color = nodo["col"])||(nodo_color = nodo["colo"])
			||(nodo_color = nodo["COLOR"])||(nodo_color = nodo["COLO"])||(nodo_color = nodo["C"])||(nodo_color = nodo["COL"])||
			(nodo_color = nodo["c"])||(nodo_color = nodo["CO"])||(nodo_color = nodo["Color"])||(nodo_color = nodo["Col"])||(nodo_color = nodo["Co"])||(nodo_color = nodo["Colo"])){
		try{
			color = nodo_color.as<string>();
			rgbColor = cargarColor(color,i);
		}catch(Exception &e){
			rgbColor.r = 0;
			rgbColor.g = 255;
			rgbColor.b = 0;
			loguear();
			logFile << "    Warning " <<"\t No se pudo encontrar la característica color del objeto número: "<<i<<". Por defecto seteamos color verde " << endl;
		}


	}else{ //No encuentra la clave. Color por defecto verde
		rgbColor.r = 0;
		rgbColor.g = 255;
		rgbColor.b = 0;
		loguear();
		logFile << "    Warning " <<"\t No se pudo encontrar la característica color del objeto número: "<<i<<". Por defecto seteamos color verde " << endl;
	}
	return rgbColor;

}

float32 Cargador::obtenerMasa(Node nodo,size_t i){
	Node nodo_masa;
	float32 masa;
	if((nodo_masa = nodo["masa"])||(nodo_masa = nodo["Masa"])||(nodo_masa = nodo["MASA"])||(nodo_masa = nodo["m"])||
			(nodo_masa = nodo["ma"])||(nodo_masa = nodo["mas"])||(nodo_masa = nodo["M"])||(nodo_masa = nodo["MA"])||
			(nodo_masa = nodo["MAS"])||(nodo_masa = nodo["Ma"])||(nodo_masa = nodo["Mas"])){
		try{
			masa = nodo_masa.as<float32>();
		}catch(Exception &e){
			masa = 1.0;
			loguear();
			logFile << "    Warning " <<"\t No se pudo encontrar la característica masa del objeto número: "<<i<<". Por defecto seteamos masa = 1 " << endl;
		}
	}
	else{ //No encuentra masa por default asignamos masa = 1
		masa = 1.0;
		loguear();
		logFile << "    Warning " <<"\t No se pudo encontrar la característica masa del objeto número: "<<i<<". Por defecto seteamos masa = 1 " << endl;
	}
	return masa;
}

float32 Cargador::obtenerRotacion(Node nodo,size_t i){
	Node nodo_rot;
	float32 rot;
	if((nodo_rot = nodo["rotacion"])||(nodo_rot = nodo["Rotacion"])||(nodo_rot = nodo["ROTACION"])||(nodo_rot = nodo["rot"])||(nodo_rot = nodo["ROT"])){
		try{
			rot = nodo_rot.as<float32>();
		}catch(Exception &e){
			rot = 0.0;
			loguear();
			logFile << "    Warning " <<"\t No se pudo encontrar la característica rotación del objeto número: "<<i<<". Por defecto seteamos rotación = 0 " << endl;
		}
	}else { //No encuentra rotación por default 0
		rot = 0.0;
		loguear();
		logFile << "    Warning " <<"\t No se pudo encontrar la característica rotación del objeto número: "<<i<<". Por defecto seteamos rotación = 0 " << endl;
	}
	return rot;
}

float32 Cargador::obtenerAncho(Node nodo,size_t i){
	Node nodo_ancho;
	float32 ancho;
	if((nodo_ancho = nodo["ancho"])||(nodo_ancho = nodo["Ancho"])||(nodo_ancho = nodo["ANCHO"])||(nodo_ancho = nodo["an"])){
		try{
			ancho = nodo_ancho.as<float32>();
		}catch(Exception &e){
			ancho = 3.0;
			loguear();
			logFile << "    Warning " <<"\t No se pudo encontrar la característica ancho del objeto número: "<<i<<". Por defecto seteamos ancho = 3 " << endl;
		}
	}else { //No encuentra ancho por default 3
		ancho = 3.0;
		loguear();
		logFile << "    Warning " <<"\t No se pudo encontrar la característica ancho del objeto número: "<<i<<". Por defecto seteamos ancho = 3 " << endl;
	}
	return ancho;
}

float32 Cargador::obtenerAlto(Node nodo,size_t i){
	Node nodo_alto;
	float32 alto;
	if((nodo_alto = nodo["alto"])||(nodo_alto = nodo["Alto"])||(nodo_alto = nodo["ALTO"])||(nodo_alto = nodo["al"])){
		try{
			alto = nodo_alto.as<float32>();
		}catch(Exception &e){
			alto = 2.0;
			loguear();
			logFile <<  "    Warning " <<"\t No se pudo encontrar la característica alto del objeto número: "<<i<<". Por defecto seteamos alto = 2 " << endl;
		}
	}else { //No encuentra alto por default 2
		alto = 2.0;
		loguear();
		logFile <<  "    Warning " <<"\t No se pudo encontrar la característica alto del objeto número: "<<i<<". Por defecto seteamos alto = 2 " << endl;
	}
	return alto;
}

uint Cargador::convertirToInt(string cadena){
    uint numValue;
    stringstream stream;
    stream << hex << cadena;
    stream >> numValue;
    return numValue;
}


bool Cargador::validarColor(string color){
	size_t largo;
	largo = color.size();
	bool valido = false;
	if(largo ==6){
		for(size_t i = 0; i < largo; i++){
			if(isxdigit(color[i])){
				valido = true;
			}else{
				return false;
			}
		}
	}
	return valido;
}

SDL_Color Cargador::cargarColor(string color,size_t i){
	size_t len = color.size();
	string c = color.substr(1,len);
	SDL_Color rgbColor;
	if(validarColor(c)){
		int colorInt = convertirToInt(c);
		uint8 r = (colorInt >> 16);
		uint8 g = (colorInt >> 8);
		uint8 b = (colorInt );
		rgbColor.r = (r  & 0xFF) ;
		rgbColor.g = (g & 0xFF) ;
		rgbColor.b = (b & 0xFF);
	}else{ //Si el color que entró no es válido seteo verde por defecto e informo en log
		rgbColor.r = 0;
		rgbColor.g = 255;
		rgbColor.b = 0;

		loguear();
		logFile <<"    Warning " <<"\t El valor de color del objeto número: "<<i<<" es inválido. Por defecto seteamos color verde " << endl;
	}
	return rgbColor;
}

int Cargador::cantidadLados(const Node nodo,size_t i,string tipo){
	if(tipo.compare(0,1,"t")==0){
		return triangulo;
	}else if(tipo.compare(0,1,"c")==0){
		return cuadrado;
	}else if(tipo.compare(0,1,"r")==0){
		return rectangulo;
	}else if(tipo.compare(0,1,"p")==0){
		return pentagono;
	}else if(tipo.compare(0,1,"h")==0){
		return hexagono;
	}else{
		loguear();
		logFile << "    Warning " <<"\t No se pudo identificar la cantidad de lados del polígono número: "<<i<<". Por defecto seteamos cantidad de lados = 4 " << endl;
		return cuadrado;
	}
	return 0;
}

structInicial* Cargador::getPaqueteInicial(){
	return this->datos;
}

void Cargador::loadViento(Node nodo){
	float32 wind;
	Node nodoAux = nodo["viento"];
	if(nodoAux){
		try{
			wind = nodoAux.as<float32>();
		}catch(Exception &e){
			wind = 0.0;
			loguear();
			logFile << "    Warning " <<"\t No se pudo cargar el viento. Por defecto lo seteamos en 0. " << endl;
		}
	}else{
		wind = 0.0;
		loguear();
		logFile << "    Warning " <<"\t No se pudo cargar el viento. Por defecto lo seteamos en 0. " << endl;
	}
	datos->viento = wind;
}

float32 Cargador::getViento(){
	return datos->viento;
}
