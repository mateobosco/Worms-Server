//#include <yaml-cpp/yaml.h>
//
//#include "Dibujador.h"
//#include "Cargador.h"
////#include "Personaje.h"
//#include "ManejadorPersonajes.h"
//#include "PaqueteDibujar.h"
//
//ofstream logFile;
//
////Genera la fecha y la hora obtenida desde el sistema.
//void generarFechaYHora(char* fecha, char* hora){
//	char* anio = new char[5];
//	memset(anio,'\0',5);
//	char* mes = new char[3];
//	memset(mes,'\0',3);
//	char* dia = new char[3];
//	memset(dia,'\0',3);
//	char* horas = new char[3];
//	memset(horas,'\0',3);
//	char* minutos = new char[3];
//	memset(minutos,'\0',3);
//
//	time_t tiempo = time(NULL);
//	struct tm* info_tiempo = localtime(&tiempo);
//	strftime(anio, sizeof(anio), "%Y", info_tiempo);
//	strftime(mes, sizeof(mes), "%m", info_tiempo);
//	strftime(dia, sizeof(dia), "%d", info_tiempo);
//	strftime(horas, sizeof(horas), "%H", info_tiempo);
//	strftime(minutos, sizeof(minutos), "%M", info_tiempo);
//
//	memcpy(fecha, dia,3);
//	strcat(fecha,"/");
//	strcat(fecha, mes);
//	strcat(fecha,"/");
//	strcat(fecha, anio);
//	memcpy(hora,horas,3);
//	strcat(hora,":");
//	strcat(hora, minutos);
//
//	delete[] anio;
//	delete[] mes;
//	delete[] dia;
//	delete[] horas;
//	delete[] minutos;
//}
//
//void loguear(){
//	char* fecha = new char[12];
//	char* hora = new char[8];
//	generarFechaYHora(fecha,hora);
//	logFile << fecha << " " << hora ;
//	delete[] fecha;
//	delete[] hora;
//}
//
//bool KEYS[322];
//
//SDL_Window* gWindow = NULL;
//SDL_Renderer* gRenderer = NULL;
//
//bool init(int ancho, int alto){
//	bool success = true;
//	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
//	{
//		loguear();
//		logFile << "    Error   " << "\t  SDL No pudo inicializar! SDL Error: " <<  SDL_GetError()<< endl;
//		success = false;
//	}
//	else{
//		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ){
//			loguear();
//			logFile << "    Warning " << "\t  Linear texture filtering no habilitado! " <<  endl;
//			success = false;
//		}
//		gWindow = SDL_CreateWindow( "WORMS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ancho , alto, SDL_WINDOW_SHOWN );
//		if( gWindow == NULL ){
//			loguear();
//			logFile << "    Error   " << "\t  La ventana no pudo ser creada! SDL Error: " <<  SDL_GetError()<< endl;
//			success = false;
//		}
//		else{
//			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
//			if( gRenderer == NULL )	{
//				loguear();
//				logFile << "    Error   " << "\t  Renderer no pudo ser creado! SDL Error: " <<  SDL_GetError()<< endl;
//				success = false;
//			}
//			else{
//				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
//				int imgFlags = (IMG_INIT_PNG |IMG_INIT_JPG ); //
//				int iniciadas = IMG_Init( imgFlags );
//				if((iniciadas & imgFlags) != imgFlags ){
//					loguear();
//					logFile << "    Error   " << "\t  SDL_image no puedo ser inicializado! SDL_image Error: " <<  SDL_GetError()<< endl;
//					success = false;
//				}
//			}
//		}
//	}
//	return success;
//}
//
//void close(){
//	SDL_DestroyRenderer(gRenderer);
//	SDL_DestroyWindow(gWindow);
//	gRenderer = NULL;
//	gWindow = NULL;
//	SDL_Quit();
//}
//
//void keyboard(SDL_Event event , int* posicion_mouse_movimiento, int* posicion_mouse_click, int* posicion_mouse_wheel) {
//	int numero;
//	while (SDL_PollEvent(&event)) {
//		switch (event.type) {
//		case SDL_QUIT:
//			break;
//		case SDL_KEYDOWN:
//			numero = event.key.keysym.sym;
//			if (numero == SDLK_RIGHT){
//				KEYS[100] = true;
//				break;
//			}
//			if (numero == SDLK_LEFT){
//				KEYS[101] = true;
//				break;
//			}
//			if (numero == SDLK_UP){
//				KEYS[102] = true;
//				break;
//			}
//			if (event.key.keysym.sym == SDLK_DOWN){
//				KEYS[103] = true;
//				break;
//			}
//			if(numero < 1000){
//				//printf("ENTRA ACAAAA 4 \n");
//				KEYS[numero] = true;
//			}
//			break;
//		case SDL_KEYUP:
//           if (event.key.keysym.sym == SDLK_RIGHT){
//				KEYS[100] = false;
//				break;
//			}
//			if (event.key.keysym.sym == SDLK_LEFT){
//				KEYS[101] = false;
//				break;
//			}
//			if (event.key.keysym.sym == SDLK_UP){
//				KEYS[102] = false;
//				break;
//			}
//			if (event.key.keysym.sym == SDLK_DOWN){
//				KEYS[103] = false;
//				break;
//			}
//
//			KEYS[event.key.keysym.sym] = false;
//			break;
//		case SDL_MOUSEBUTTONDOWN:
//			int x, y;
//			SDL_GetMouseState( &x, &y );
//			posicion_mouse_click[0] = x;
//			posicion_mouse_click[1] = y;
//			//printf("LA posicion detectada fue %d, %d \n ", x,y);
//			break;
//		case SDL_MOUSEMOTION:
//			int w, z;
//			SDL_GetMouseState( &w, &z );
//			posicion_mouse_movimiento[0] = w;
//			posicion_mouse_movimiento[1] = z;
//			//printf("LA posicion detectada fue %d, %d \n ", w,z);
//			break;
//		case SDL_MOUSEWHEEL:
//			int a, b,d;
//			SDL_GetMouseState( &a, &b );
//			SDL_MouseWheelEvent wheel = event.wheel;
//			d = (int) wheel.y;
//			posicion_mouse_wheel[0] = a;
//			posicion_mouse_wheel[1] = b;
//			posicion_mouse_wheel[2] = d;
//			break;
//			//printf("LA posicion detectada fue (%d, %d),%d, %d  \n ", a,b,c,d);
//
//		//default:
//			//break;
//		}
//   }
//}
//
//int main(){
//	int retorno = 0;
//	logFile.open("log.txt",ios::out);
//	logFile << "Log File. Ejecución Worms" << endl;
//	logFile << "   Fecha  | Hora |  Gravedad  | \t Mensaje " << endl;
//
//	Cargador* cargador = new Cargador("TPTaller/prueba_escenario.yaml");
//	Node nodo_escenario;
//	if(!((cargador->getNodo(cargador->getNodo(),nodo_escenario,"esc"))||(cargador->getNodo(cargador->getNodo(),nodo_escenario,"escenario"))||(cargador->getNodo(cargador->getNodo(),nodo_escenario,"esce"))||(cargador->getNodo(cargador->getNodo(),nodo_escenario,"escen"))||
//			(cargador->getNodo(cargador->getNodo(),nodo_escenario,"escena"))||(cargador->getNodo(cargador->getNodo(),nodo_escenario,"escenar"))||(cargador->getNodo(cargador->getNodo(),nodo_escenario,"escenari"))||
//			(cargador->getNodo(cargador->getNodo(),nodo_escenario,"Esc"))||(cargador->getNodo(cargador->getNodo(),nodo_escenario,"ESC"))||(cargador->getNodo(cargador->getNodo(),nodo_escenario,"ESCENARIO"))||
//			(cargador->getNodo(cargador->getNodo(),nodo_escenario,"Escenario"))||(cargador->getNodo(cargador->getNodo(),nodo_escenario,"Esc")))){
//		loguear();
//		logFile << "    Error   " << "\t No se encuentra el escenario. Se carga escenario por defecto."<<endl;
//		delete cargador;
//		cargador = new Cargador(pathDefEs.c_str());
//		nodo_escenario = cargador->getNodo()["escenario"];
//
//	}
//
//	Escalador* escalador = cargador->loadEscalador(nodo_escenario);
//	if(!escalador){
//		loguear();
//		logFile << "    Error   " << "\t  No se pudo escalar la información "<< endl;
//	}
//	string imagen_agua = agua;
//	Agua* agua = cargador->loadAgua(nodo_escenario,imagen_agua);
//	if(!agua){
//		loguear();
//		logFile << "    Error   " << "\t  No se pudo crear el agua. "<< endl;
//	}
//	b2Vec2 escalas = b2Vec2(escalador->getEscalaX(), escalador->getEscalaY());
//	Mundo* mundo = new Mundo(b2Vec2(0.0,9.8),agua, escalas);
//	if(!mundo){
//		loguear();
//		logFile << "    Error   " << "\t  No se pudo crear el mundo."<< endl;
//	}
//	string tierra;
//	string cielo;
//	if(!((cargador->loadPath(nodo_escenario, "imagen_tierra",tierra))||(cargador->loadPath(nodo_escenario, "tierra",tierra))||(cargador->loadPath(nodo_escenario, "imagen-tierra",tierra)))){
//		loguear();
//		logFile << "    Error  " <<"\t No se pudo cargar el path correspondiente a la clave buscada. " << endl;
//		tierra = pathDefMas;
//	}
//	if(!((cargador->loadPath(nodo_escenario, "imagen_cielo",cielo))||(cargador->loadPath(nodo_escenario, "cielo",cielo))||(cargador->loadPath(nodo_escenario, "imagen-cielo",cielo)))){
//		loguear();
//		logFile << "    Error  " <<"\t No se pudo cargar el path correspondiente a la clave buscada. " << endl;
//		cielo = pathDefCielo;
//	}
//	if(!init(escalador->getVentanaX(),escalador->getVentanaY())){
//		loguear();
//		logFile <<"    Error   " << "\t  Inicialización inválida " << endl;
//		return -1;
//		}
//
//	LectorMascara* lector = new LectorMascara(tierra);
//	if(!lector){
//		loguear();
//		logFile << "    Error   " << "\t  No se pudo crear el Lector de Máscara. " <<  SDL_GetError()<< endl;
//		return -1;
//	}
//	b2Vec2* vectorTierra = lector->LeerMascara(escalador);
//		if(!vectorTierra){
//			tierra = pathDefMas;
//			loguear();
//			logFile <<  "    Error   " << "\t No se pudo obtener posiciones de la máscara '"<<lector->getPath()<<"'. Se carga por defecto '"<<tierra<<"'."<< endl;
//			lector->setMascaraPorDefecto();
//			vectorTierra = lector->LeerMascara(escalador);
//
//		}
//	mundo->Crear_Chains(vectorTierra, lector->GetPixelAncho());
//	Figura** figuras;
//	Node objetos;
//	size_t cantidad=0;
//	if((cargador->getNodo(nodo_escenario,objetos,"objetos"))||(cargador->getNodo(nodo_escenario,objetos,"obj"))||
//			(cargador->getNodo(nodo_escenario,objetos,"o"))||(cargador->getNodo(nodo_escenario,objetos,"Objetos"))||(cargador->getNodo(nodo_escenario,objetos,"OBJETOS"))){
//		cantidad= objetos.size();
//		figuras = new Figura*[cantidad];
//
//		for(size_t i = 0; i < cantidad; i++){
//			figuras[i] = cargador->cargarFigura(objetos[i], mundo, escalador, i+1);
//
//		}
//	}else{
//		loguear();
//		logFile << "    Warning " << "\t  No hay objetos para cargar "<< endl;
//	}
//	Dibujador dibujador = Dibujador(gRenderer, escalador);
//	SDL_Event event;
//
//	for(int i = 0; i < 322; i++) { // inicializa todas en falso
//	   KEYS[i] = false;
//	}
//
//	int cantidad_jugadores = 1;
//	int cantidad_personajes = 4;
//	ManejadorPersonajes* manejador_personajes = new ManejadorPersonajes();
//
//	manejador_personajes->AgregarJugador(mundo, 1);
//	mundo->setVectorPersonajes(manejador_personajes->getPersonajes(), manejador_personajes->getCantidadPersonajes(), cantidad_jugadores);
//	mundo->setFiguras(figuras, cantidad);
//
//
//
//	SDL_Color color2;
//	Poligono* poligono = new Poligono(mundo, b2Vec2(50,20),4,5,false,color2,4,90);
//	Personaje* personaje = new Personaje(mundo,2);
//
//
//
//
//	dibujador.iniciarFondo(agua, cielo, lector->getPath());
//	dibujador.dibujarPersonajes(manejador_personajes->getPersonajes(),  manejador_personajes->getCantidadPersonajes());
//	dibujador.dibujarFiguras(figuras,cantidad);
//
//	int* posicion_mouse_click = (int*)malloc (sizeof(int)*2);
//	int* posicion_mouse_scroll = (int*)malloc (sizeof(int)*2);
//	int* posicion_mouse_movimiento = (int*)malloc (sizeof(int)*2);
//	posicion_mouse_click[0] = -1;
//	posicion_mouse_click[1] = -1;
//	KEYS[SDLK_s] = true;
//	while(KEYS[SDLK_ESCAPE] == false){
//		if (posicion_mouse_click[0]>0 && posicion_mouse_click[1]>0 && posicion_mouse_click[0] < escalador->getVentanaX() && posicion_mouse_click[1] < escalador->getVentanaY()){
//			b2Vec2 aux = b2Vec2(posicion_mouse_click[0], posicion_mouse_click[1]);
//			b2Vec2 mouse_escalado = escalador->escalarPosicion(aux);
//			if (mouse_escalado.x>0 && mouse_escalado.y>0 && mouse_escalado.x < escalador->getEscalaX() && mouse_escalado.y < escalador->getEscalaY()){
//				manejador_personajes->seleccionarPersonaje(mouse_escalado);
//			}
//		}
//		escalador->hacerZoom(posicion_mouse_scroll);
//		posicion_mouse_scroll[2]=0;
//		escalador->moverVentana(posicion_mouse_movimiento);
//		dibujador.dibujarFondo(agua);
//		dibujador.dibujarPersonajes(manejador_personajes->getPersonajes(), manejador_personajes->getCantidadPersonajes());
//
//		structFigura* paquete = crearPaqueteFigura(poligono);
//		structPersonaje* paquete1 = crearPaquetePersonaje(personaje);
//		dibujador.dibujarPaqueteFigura(paquete);
//		dibujador.dibujarPaquetePersonaje(paquete1);
//		destruirPaquetePersonaje(paquete1);
//		destruirPaqueteFigura(paquete);
//
//
//
//		int id_jugador = 1;
//		manejador_personajes->moverPersonaje(event, KEYS, id_jugador);
//		dibujador.dibujarFiguras(figuras,cantidad);
//		if(KEYS[SDLK_p]) {
//			mundo->dormir_figuras(figuras);
//			KEYS[SDLK_p] = false;
//			while((KEYS[SDLK_p] == false) && (KEYS[SDLK_ESCAPE] == false) && (KEYS[SDLK_s] == false)){
//				keyboard(event, posicion_mouse_movimiento,posicion_mouse_click,posicion_mouse_scroll);
//			}
//			mundo->despertar_figuras(figuras);
//			KEYS[SDLK_p] = false;
//		}
//
//		if(KEYS[SDLK_s]){
//			size_t z;
//			for(z = 0; z < cantidad; z++){
//				Figura* figura_actual = figuras[z];
//				if (figura_actual != NULL){
//					figura_actual->setPosicionInicial();
//					figura_actual->getBody()->SetLinearVelocity(b2Vec2(0,0));
//					figura_actual->getBody()->SetAngularVelocity(0);
//					figura_actual->getBody()->SetAwake(true);
//				}
//			}
//			KEYS[SDLK_s] = false;
//			dibujador.dibujarFondo(agua);
//			dibujador.dibujarPersonajes(manejador_personajes->getPersonajes(), cantidad_personajes * cantidad_jugadores);
//			dibujador.dibujarFiguras(figuras,cantidad);
//
//
//			structFigura* paquete = crearPaqueteFigura(poligono);
//			structPersonaje* paquete1 = crearPaquetePersonaje(personaje);
//			dibujador.dibujarPaqueteFigura(paquete);
//			dibujador.dibujarPaquetePersonaje(paquete1);
//			destruirPaquetePersonaje(paquete1);
//			destruirPaqueteFigura(paquete);
//
//
//			while((KEYS[SDLK_s] == false) && (KEYS[SDLK_ESCAPE] == false)){
//				keyboard(event, posicion_mouse_movimiento,posicion_mouse_click,posicion_mouse_scroll);
//			}
//			KEYS[SDLK_s] = false;
//		}
//		mundo->devolver_world()->Step(0.05,100,100);
//		mundo->comprobar_nivel_agua();
//		keyboard(event, posicion_mouse_movimiento,posicion_mouse_click,posicion_mouse_scroll);
//	}
//	for(size_t i= 0; i< cantidad ; i++){
//		if (figuras != NULL){
//			if( figuras[i] != NULL ){
//				delete figuras[i];
//			}
//		}
//	}
//	delete agua;
//	delete lector;
//	delete[] figuras;
//	delete mundo;
//	delete cargador;
//	delete manejador_personajes;
//	//delete dibujador; //creo que no hace falta porque no es un puntero y muere al final de la funcion
//	free(posicion_mouse_movimiento);
//	free(posicion_mouse_scroll);
//	free(posicion_mouse_click);
//	close();
//	logFile.close();
//	return retorno;
//}
