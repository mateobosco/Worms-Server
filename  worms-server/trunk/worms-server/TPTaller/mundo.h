#ifndef MUNDO_H
#define MUNDO_H

#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Collision/Shapes/b2ChainShape.h>

#include "figura.h"

using namespace std;

class Mundo {
	public:
		Mundo(b2Vec2 gravedad, Agua* el_agua, b2Vec2 escalas);
		~Mundo();
		bool destruir_cuerpo(b2Body* cuerpo);
		b2Body* devolverBodies();
		void crear_tierra(vector<int> vector_tierra);
		b2World* devolver_world();
		void step(float32 timeStep, int32 velocityIterations, int32 positionIterations);
		void Crear_Chains(b2Vec2* tierra, float32 pixelesX);
		void comprobar_nivel_agua();
		size_t GetCantidadFiguras();
		void despertar_figuras(Figura** figuras);
		void dormir_figuras(Figura** figuras);
		bool checkOverlap(Figura* figura_ins, Escalador *escalador,size_t i);
		int GetCantVerticesTierra();
		void setFiguras(Figura** figuras, int cantidad_figuras);
		void setVectorPersonajes(Personaje** vector_personajes, int cantidad_personajes, int cantidad_jugadores );
		b2Vec2* GetVerticesTierra();
		b2Vec2 GetEscalas();
		Agua* getAgua();
		Personaje** getPersonajes();
		int getCantidadPersonajes();
		Figura** getFiguras();
		int getCantidadFiguras();

	private:
		int cantidad_vertices_tierra;
		int cantidad_personajes;
		int cantidad_jugadores;
		b2Vec2 escalas;
		b2Vec2* vertices_tierra;
		b2Vec2 grav;
		Agua* agua;
		size_t cantidad_figuras;
		b2World* my_world;
		Figura** figuras;
		Personaje** vector_personajes;
};

#endif
