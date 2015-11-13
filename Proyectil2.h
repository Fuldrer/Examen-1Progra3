#ifndef PROJECTIL2_H
#define PROJECTIL2_H

#include <list>
#include "Entidad.h"
#include "Jugador.h"
#include "Proyectil.h"

using namespace std;

class Proyectil2 : public Entidad
{
    public:
        SDL_Renderer* renderer;
        list<Entidad*>* entidades;
        SDL_Texture* texture;
        SDL_Rect rect;
        string state;
        Proyectil2(list<Entidad*>* entidades,SDL_Renderer* renderer, int x, int y, string state);
        virtual ~Proyectil2();
        void dibujar();
        void logica();
    protected:
    private:
};

#endif // PROJECTIL_H
