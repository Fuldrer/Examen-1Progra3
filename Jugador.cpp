#include "Jugador.h"

Jugador::Jugador(list<Entidad*>* entidades,SDL_Renderer* renderer)
{
    tipo = "Jugador";
    this->renderer = renderer;
    this->textures["down"].push_back(IMG_LoadTexture(renderer, "Personaje/down1.png"));
    this->textures["down"].push_back(IMG_LoadTexture(renderer, "Personaje/down2.png"));
    this->textures["up"].push_back(IMG_LoadTexture(renderer, "Personaje/up1.png"));
    this->textures["up"].push_back(IMG_LoadTexture(renderer, "Personaje/up2.png"));
    this->textures["left"].push_back(IMG_LoadTexture(renderer, "Personaje/left1.png"));
    this->textures["left"].push_back(IMG_LoadTexture(renderer, "Personaje/left2.png"));
    this->textures["right"].push_back(IMG_LoadTexture(renderer, "Personaje/right1.png"));
    this->textures["right"].push_back(IMG_LoadTexture(renderer, "Personaje/right2.png"));

    SDL_QueryTexture(this->textures["down"][0], NULL, NULL, &rect.w, &rect.h);
    x = 200;
    y = 100;
    rect.x = x;
    rect.y = x;

    velocity=1.5;
    animation_velocity=20;

    current_texture=0;

    state="down";

    this->entidades = entidades;
}

Jugador::~Jugador()
{
    //dtor
}

void Jugador::logica()
{
    const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );

    if( currentKeyStates[ SDL_SCANCODE_RIGHT ] )
    {
        x+=velocity;
        state="right";
    }
    if( currentKeyStates[ SDL_SCANCODE_LEFT ] )
    {
        x-=velocity;
        state="left";
    }
    if( currentKeyStates[ SDL_SCANCODE_UP ] )
    {
        y-=velocity;
        state="up";
    }
    if( currentKeyStates[ SDL_SCANCODE_DOWN ] )
    {
        y+=velocity;
        state="down";
    }

    if( currentKeyStates[ SDL_SCANCODE_Z ] )
    {
        Proyectil *p = new Proyectil(entidades,renderer,x,y,state);
        entidades->push_back(p);
    }

    if(frames%animation_velocity==0)
    {
        current_texture++;
        if(current_texture>=textures[state].size())
            current_texture=0;
    }

    frames++;
}
