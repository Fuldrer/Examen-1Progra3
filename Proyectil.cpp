#include "Proyectil.h"

Proyectil::Proyectil(list<Entidad*>* entidades, SDL_Renderer* renderer, int x, int y, string state)
{
    tipo = "Proyectil";
    this->entidades = entidades;
    this->renderer = renderer;
    this->state = state;
    texture = IMG_LoadTexture(renderer,"proyectil.png");
    rect.x=x;
    rect.y=y;
    rect.w=20;
    rect.h=10;
}

Proyectil::~Proyectil()
{
    //dtor
}

void Proyectil::dibujar()
{
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

void Proyectil::logica()
{
    int velocity = 1;
    if(state=="right")
    {
        rect.y+=velocity;
    }
    if(state=="left")
    {
        rect.y+=velocity;
    }
    if(state=="up")
    {
        rect.y+=velocity;
    }
    if(state=="down")
    {
        rect.y+=velocity;
    }

    for(list<Entidad*>::iterator e=entidades->begin();
        e!=entidades->end();
        e++)
    {
        if((*e)->tipo=="Enemigo")
        {
            Enemigo* enemigo = (Enemigo*)(*e);
            if(colision(rect,enemigo->rect))
            {
                enemigo->delete_flag = true;
                this->delete_flag = true;
                cout<<"Colision con proyectil"<<endl;
            }

        }
        else if((*e)->tipo=="Enemigo2")
        {
            Enemigo2* enemigo2 = (Enemigo2*)(*e);
            if(colision(rect,enemigo2->rect))
            {
                enemigo2->delete_flag = true;
                this->delete_flag = true;
                cout<<"Colision con proyectil 2"<<endl;
            }

        }

    }
}





