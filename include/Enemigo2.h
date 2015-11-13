#ifndef ENEMIGO2_H
#define ENEMIGO2_H

//#include "Personaje.h"
#include "Jugador.h"
#include <stdlib.h>
#include <list>

class Jugador;

class Enemigo2 : public Enemigo
{
    public:
        Jugador* jugador;
        Enemigo(list<Entidad*>* entidades,SDL_Renderer* renderer);
        virtual ~Enemigo();
        void logica();
    protected:
    private:
};

#endif // ENEMIGO_H
