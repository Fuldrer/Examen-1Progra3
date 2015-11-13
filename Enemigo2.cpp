#include "Enemigo2.h"

Enemigo2::Enemigo2(list<Entidad*>* entidades,SDL_Renderer* renderer)
{
    tipo = "Enemigo2";
    this->renderer = renderer;
    this->textures["down"].push_back(IMG_LoadTexture(renderer, "Enemigo2/down1.png"));
    this->textures["down"].push_back(IMG_LoadTexture(renderer, "Enemigo2/down2.png"));
    this->textures["up"].push_back(IMG_LoadTexture(renderer, "Enemigo2/up1.png"));
    this->textures["up"].push_back(IMG_LoadTexture(renderer, "Enemigo2/up2.png"));
    this->textures["left"].push_back(IMG_LoadTexture(renderer, "Enemigo2/left1.png"));
    this->textures["left"].push_back(IMG_LoadTexture(renderer, "Enemigo2/left2.png"));
    this->textures["right"].push_back(IMG_LoadTexture(renderer, "Enemigo2/right1.png"));
    this->textures["right"].push_back(IMG_LoadTexture(renderer, "Enemigo2/right2.png"));

    SDL_QueryTexture(this->textures["down"][0], NULL, NULL, &rect.w, &rect.h);
    x = rand()%100;
    y = rand()%100;
    rect.x=x;
    rect.y=x;

    velocity=0.5;
    animation_velocity=20;

    current_texture=0;

    state="down";

    this->entidades = entidades;

    for(list<Entidad*>::iterator e=entidades->begin();
        e!=entidades->end();
        e++)
    {
        if((*e)->tipo=="Jugador")
        {
            jugador = (Jugador*)*e;
        }
    }
}

Enemigo2::~Enemigo2()
{
    //dtor
}

void Enemigo2::logica()
{
    const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
    if(jugador->x>x)
    {
        state="right";
    }
    if(jugador->x<x)
    {
        state="left";
    }
    if(jugador->y<y)
    {
        state="up";
    }
    if(jugador->y>y)
    {
        state="down";
    }

    if(state=="right")
    {
        x+=velocity;
    }
    if(state=="left")
    {
        x-=velocity;
    }
    if(state=="up")
    {
        y-=velocity;
    }
    if(state=="down")
    {
        y+=velocity;
    }

    if( currentKeyStates[ SDL_SCANCODE_C ] )
    {
        Proyectil2 *c = new Proyectil2(entidades,renderer,x,y,state);
        entidades->push_back(c);
    }
    if(frames%animation_velocity==0)
    {
        current_texture++;
        if(current_texture>=textures[state].size())
            current_texture=0;
    }

    frames++;
}
