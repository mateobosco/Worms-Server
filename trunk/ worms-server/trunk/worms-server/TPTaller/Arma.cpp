#include "Arma.h"

Arma::Arma(){
	this->fuerza = 0;
}

Arma::~Arma(){

}

void Arma::disparar(){

}

b2Vec2* Arma::definirImpulso(b2Vec2 destino){
	b2Vec2 *impulso = new b2Vec2();
	impulso->x = (destino.x - this->body->GetPosition().x) * this->fuerza;
	impulso->y = (destino.y - this->body->GetPosition().y) * this->fuerza;
	return impulso;
}

//Retorna 0 si chocó con una figura.
//Retorna 1 si chocó con un personaje.
//Retorna 2 no hay choque.
int Arma::checkImpacto(Mundo *mundo){
	Figura **figuras = mundo->getFiguras();
	Figura *figura_aux;
	Personaje **personajes = mundo->getPersonajes();
	Personaje *personaje_aux;

	b2Shape *forma_ins, *forma_world;
	int index_ins, index_world;
	b2Transform transf_ins, transf_world;

	forma_ins = this->body->GetFixtureList()->GetShape();
	index_ins = 0;
	transf_ins = this->body->GetTransform();
	bool overlap;
	for(unsigned int i = 0; i < mundo->GetCantidadFiguras(); i++){
		figura_aux = figuras[i];
		forma_world = figura_aux->getBody()->GetFixtureList()->GetShape();
		index_world = 0;
		transf_world = figura_aux->getBody()->GetTransform();
		overlap = b2TestOverlap(forma_ins, index_ins, forma_world, index_world, transf_ins, transf_world);
		if(overlap){
			return 0;
		}
	}
	for(int i = 0; i < mundo->getCantidadPersonajes(); i++){
		personaje_aux = personajes[i];
		forma_world = personaje_aux->getBody()->GetFixtureList()->GetShape();
		index_world = 0;
		transf_world = personaje_aux->getBody()->GetTransform();
		overlap = b2TestOverlap(forma_ins, index_ins, forma_world, index_world, transf_ins, transf_world);
		if(overlap){
			personaje_aux->quitarEnergia(this->danio);
			return 1;
		}
	}
	return 2;
}

bool Arma::setFuerza(){
	if(this->fuerza >= MAX_FUERZA){
		return false;
	} else{
		this->fuerza += (MAX_FUERZA / 1000);//todo poner algo inferior
		return true;
	}
}

void Arma::setAngulo(int un_angulo){
	this->angulo = un_angulo;
}
