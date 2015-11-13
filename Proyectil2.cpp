#include "Proyectil2.h"

Proyectil2::Proyectil2(list<Entidad*>* entidades, SDL_Renderer* renderer, int x, int y, string state)
{
    tipo = "Proyectil2";
    this->entidades = entidades;
    this->renderer = renderer;
    this->state = state;
    texture = IMG_LoadTexture(renderer,"proyectil2.png");
    rect.x=x;
    rect.y=y;
    rect.w=20;
    rect.h=10;
}

Proyectil2::~Proyectil2()
{
    //dtor
}

void Proyectil2::dibujar()
{
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

void Proyectil2::logica()
{
    int velocity = 1;
    if(state=="right")
    {
        rect.x+=velocity;
    }
    if(state=="left")
    {
        rect.x-=velocity;
    }
    if(state=="up")
    {
        rect.y-=velocity;
    }
    if(state=="down")
    {
        rect.y+=velocity;
    }

    for(list<Entidad*>::iterator e=entidades->begin();
        e!=entidades->end();
        e++)
    {
        if((*e)->tipo=="Jugador")
        {
            Jugador* jugador = (Jugador*)(*e);
            if(colision(rect,jugador->rect))
            {
                jugador->delete_flag = true;
                this->delete_flag = true;
                cout<<"Colision con proyectil 3"<<endl;
            }

        }

    }
}
