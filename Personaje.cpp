#include "Personaje.h"

Personaje::Personaje()
{
    tipo = "Personaje";
}

Personaje::~Personaje()
{
    //dtor
}

void Personaje::dibujar()
{
    rect.x = x;
    rect.y = y;
    SDL_RenderCopy(renderer, textures[state][current_texture], NULL, &rect);
}


