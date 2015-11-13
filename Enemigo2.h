#ifndef ENEMIGO2_H
#define ENEMIGO2_H

#include "Personaje.h"
#include "Jugador.h"
#include "Proyectil2.h"
#include <stdlib.h>
#include <list>

class Jugador;

class Enemigo2 : public Personaje
{
    public:
        Jugador* jugador;
        Enemigo2(list<Entidad*>* entidades,SDL_Renderer* renderer);
        virtual ~Enemigo2();
        void logica();
    protected:
    private:
};

#endif // ENEMIGO2_H
