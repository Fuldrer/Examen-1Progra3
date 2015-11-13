#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h> 	/* srand, rand */
#include <time.h>       /* time */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include<list>

#include "Jugador.h"
#include "Enemigo.h"
#include "Enemigo2.h"
#include "Proyectil.h"
#include "Proyectil2.h"

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event Event;
SDL_Texture *background,*character;
SDL_Rect rect_background,rect_character;
Mix_Music *music = Mix_LoadMUS("musica/Song.wav");

#include <SDL_mouse.h>

using namespace std;

int SCREEN_WIDTH = 1280;
int SCREEN_HEIGHT = 720;

int objectCount = 0;

//Texture wrapper class
class LTexture {
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool loadFromFile(std::string path);

#ifdef _SDL_TTF_H
	//Creates image from font string
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
#endif

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(int x, int y, int w, int h, SDL_Rect* clip = NULL,
			double angle = 0.0, SDL_Point* center = NULL,
			SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};

class LWindow {
public:
	//Intializes internals
	LWindow();

	//Creates window
	bool init();

	//Creates renderer from internal window
	SDL_Renderer* createRenderer();

	//Handles window events
	void handleEvent(SDL_Event& e);

	//Deallocates internals
	void free();

	//Window dimensions
	int getWidth();
	int getHeight();

	//Window focii
	bool hasMouseFocus();
	bool hasKeyboardFocus();
	bool isMinimized();

private:
	//Window data
	SDL_Window* mWindow;

	//Window dimensions
	int mWidth;
	int mHeight;

	//Window focus
	bool mMouseFocus;
	bool mKeyboardFocus;
	bool mFullScreen;
	bool mMinimized;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();


//The window we'll be rendering to
//SDL_Window* gWindow = NULL;

//Our custom window
LWindow gWindow;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Globally used font
TTF_Font *gFont = NULL;

//Scene textures
LTexture gPause;
LTexture gText;


LTexture::LTexture() {
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture() {
	//Deallocate
	free();
}
int jugar()
{
    bool pausa = false;
    bool salida = false;
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return 10;
    }
    //Creates a SDL Window
    if((window = SDL_CreateWindow("Image Loading", 100, 100, 1280/*WIDTH*/, 720/*HEIGHT*/, SDL_WINDOW_RESIZABLE | SDL_RENDERER_PRESENTVSYNC)) == NULL)
    {
        return 20;
    }
    //SDL Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );
    if (renderer == NULL)
    {
        std::cout << SDL_GetError() << std::endl;
        return 30;
    }

    //Init textures
    int w=0,h=0;
    background = IMG_LoadTexture(renderer,"fondo.png");
    SDL_QueryTexture(background, NULL, NULL, &w, &h);
    rect_background.x = 0;
    rect_background.y = 0;
    rect_background.w = w;
    rect_background.h = h;



    list<Entidad*>entidades;
    entidades.push_back(new Jugador(&entidades,renderer));
    entidades.push_back(new Enemigo(&entidades,renderer));
    entidades.push_back(new Enemigo2(&entidades,renderer));


    double last_frame=0;

    //Main Loop
    while(true)
    {
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                return 0;
            }
            if(Event.type == SDL_KEYDOWN)
            {
                if(Event.key.keysym.sym == SDLK_d)
                    rect_character.x++;
            }
        }
        if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
        {
            return false;
        }

        //Solo esta mierda era para musica fuck!!!!!!!!!!!!!
        Mix_Music *music;
        music = Mix_LoadMUS( "musica/Song.wav" );
        if(!Mix_PlayingMusic()){
            cout<<"No esta reproduciendo la musica";
                Mix_PlayMusic(music,1);
        }


        //SDL_Delay(17-(SDL_GetTicks()-last_frame));
        double diferencia = SDL_GetTicks()-last_frame;
        double ajuste = 17 - diferencia;
        if(ajuste>0)
            SDL_Delay(ajuste);
        last_frame=SDL_GetTicks();

        for(list<Entidad*>::iterator e = entidades.begin();
            e!=entidades.end();
            e++)
            (*e)->logica();

        for(list<Entidad*>::iterator e = entidades.begin();
            e!=entidades.end();
            e++)
        {
            if((*e)->delete_flag)
            {
                entidades.remove(*e);
                break;
            }
        }

        SDL_RenderCopy(renderer, background, NULL, &rect_background);

        for(list<Entidad*>::iterator e = entidades.begin();
            e!=entidades.end();
            e++)
            (*e)->dibujar();

        SDL_RenderPresent(renderer);
        if(salida==true)
        {
            SDL_QUIT;
        }
    }

	return 0;
}

bool LTexture::loadFromFile(std::string path) {
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(),
		IMG_GetError());
	} else {
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE,
				SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL) {
			printf("Unable to create texture from %s! SDL Error: %s\n",
					path.c_str(), SDL_GetError());
		} else {
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#ifdef _SDL_TTF_H
bool LTexture::loadFromRenderedText(std::string textureText,
		SDL_Color textColor) {
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(),
			textColor);
	if (textSurface != NULL) {
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL) {
			printf(
					"Unable to create texture from rendered text! SDL Error: %s\n",
					SDL_GetError());
		} else {
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	} else {
		printf("Unable to render text surface! SDL_ttf Error: %s\n",
		TTF_GetError());
	}

	//Return success
	return mTexture != NULL;
}
#endif

void LTexture::free() {
	//Free texture if it exists
	if (mTexture != NULL) {
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
	//Modulate texture rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending) {
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha) {
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, int w, int h, SDL_Rect* clip, double angle,
		SDL_Point* center, SDL_RendererFlip flip) {
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, w, h };

	//Set clip rendering dimensions
	if (clip != NULL) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center,
			flip);
}

int LTexture::getWidth() {
	return mWidth;
}

int LTexture::getHeight() {
	return mHeight;
}

LWindow::LWindow() {
	//Initialize non-existant window
	mWindow = NULL;
	mMouseFocus = false;
	mKeyboardFocus = false;
	mFullScreen = false;
	mMinimized = false;
	mWidth = 0;
	mHeight = 0;
}

bool LWindow::init() {
	//Create window
	mWindow = SDL_CreateWindow("Platform Test", SDL_WINDOWPOS_UNDEFINED,
	SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN); // | SDL_WINDOW_RESIZABLE);
	if (mWindow != NULL) {
		mMouseFocus = true;
		mKeyboardFocus = true;
		mWidth = SCREEN_WIDTH;
		mHeight = SCREEN_HEIGHT;
	}

	return mWindow != NULL;
}

SDL_Renderer* LWindow::createRenderer() {
	return SDL_CreateRenderer(mWindow, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void LWindow::handleEvent(SDL_Event& e) {
	//Window event occured
	if (e.type == SDL_WINDOWEVENT) {
		//Caption update flag
		bool updateCaption = false;

		switch (e.window.event) {
		//Get new dimensions and repaint on window size change
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			mWidth = e.window.data1;
			mHeight = e.window.data2;
			SDL_RenderPresent(gRenderer);
			break;

			//Repaint on exposure
		case SDL_WINDOWEVENT_EXPOSED:
			SDL_RenderPresent(gRenderer);
			break;

			//Mouse entered window
		case SDL_WINDOWEVENT_ENTER:
			mMouseFocus = true;
			updateCaption = true;
			break;

			//Mouse left window
		case SDL_WINDOWEVENT_LEAVE:
			mMouseFocus = false;
			updateCaption = true;
			break;

			//Window has keyboard focus
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			mKeyboardFocus = true;
			updateCaption = true;
			break;

			//Window lost keyboard focus
		case SDL_WINDOWEVENT_FOCUS_LOST:
			mKeyboardFocus = false;
			updateCaption = true;
			break;

			//Window minimized
		case SDL_WINDOWEVENT_MINIMIZED:
			mMinimized = true;
			break;

			//Window maxized
		case SDL_WINDOWEVENT_MAXIMIZED:
			mMinimized = false;
			break;

			//Window restored
		case SDL_WINDOWEVENT_RESTORED:
			mMinimized = false;
			break;
		}

		//Update window caption with new data
		if (updateCaption) {
			std::stringstream caption;
			caption << "Menu de pausa - Examen 1Lab"
					//<< //((mMouseFocus) ? "On" : "Off") << " KeyboardFocus:"
					/*<< ((mKeyboardFocus) ? "On" : "Off")*/;
			SDL_SetWindowTitle(mWindow, caption.str().c_str());
		}
	}
	//Enter exit full screen on return key
	else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F11) {
		if (mFullScreen) {
			SDL_SetWindowFullscreen(mWindow, SDL_FALSE);
			mFullScreen = false;
		} else {
			SDL_SetWindowFullscreen(mWindow, SDL_TRUE);
			mFullScreen = true;
			mMinimized = false;
		}
	}
}

void LWindow::free() {
	if (mWindow != NULL) {
		SDL_DestroyWindow(mWindow);
	}

	mMouseFocus = false;
	mKeyboardFocus = false;
	mWidth = 0;
	mHeight = 0;
}

int LWindow::getWidth() {
	return mWidth;
}

int LWindow::getHeight() {
	return mHeight;
}

bool LWindow::hasMouseFocus() {
	return mMouseFocus;
}

bool LWindow::hasKeyboardFocus() {
	return mKeyboardFocus;
}

bool LWindow::isMinimized() {
	return mMinimized;
}

bool init() {
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init( SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	} else {
		//Set texture filtering to linear
		SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1");

		//Create window
		gWindow.init();

		gRenderer = gWindow.createRenderer();

		//Initialize renderer color
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

		//Initialize PNG loading
		int imgFlags = IMG_INIT_PNG;
		IMG_Init(imgFlags);
		TTF_Init();

	}

	return success;
}

bool loadMedia() {
	//Loading success flag
	bool success = true;

	gFont = TTF_OpenFont("sdone.ttf", 50);
	gPause.loadFromFile("esenario2.jpg");

	return success;
}

void close() {

	TTF_CloseFont(gFont);
	gFont = NULL;
	gPause.free();

	gText.free();

	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	gWindow.free();

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}


bool pause = false;

void PauseMenu() {
	pause = true;
	SDL_Color whiteColor = { 255, 255, 255, 255 };
	SDL_Color redColor = {225,0,0,0};

	int cox = 160, coy = 220;
	int sax = 160, say = 260;
	int lox = 160, loy = 300;
	int rex = 160, rey = 340;
	int sex = 160, sey = 380;
	int qux = 160, quy = 420;

	int index = 1;

	while (pause) {
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				pause = false;
				break;
			}

			//Handle window events
			gWindow.handleEvent(e);


			//If Key Pressed
			if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
				switch (e.key.keysym.sym) {
				case SDLK_w:
					if (index > 1){
						index -= 1;
					}
					break;
				case SDLK_s:
					if (index < 4){
						index += 1;
					}
					break;
				case SDLK_RETURN:
					if (index == 1){
                        pause = false;
						//jugar();
						break;
					}
					else if(index ==4){
                        pause = false;
                        break;
					}
					break;
				case SDLK_ESCAPE:
					pause = false;
					break;
				}
			}
			//If Key Released
			else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
				switch (e.key.keysym.sym) {
				case SDLK_w:
					//
					break;
				case SDLK_s:
					//
					break;
				}
			}
		}


		/*You can Choose to activate the renderclear here by removing the //, but I keep it on because it looks nice*/
		//SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 100);
		//SDL_RenderClear(gRenderer);

		gPause.render(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		std::stringstream indeX;
		//indeX << "index: " << index;

		gText.loadFromRenderedText(indeX.str().c_str(),whiteColor);
		gText.render(gWindow.getWidth()/2, gWindow.getHeight()/2, gText.getWidth(),gText.getHeight());


		gText.loadFromRenderedText("press esc to close this window",whiteColor);
		gText.render(0, 0, gText.getWidth()/2,gText.getHeight()/2);


		if (index == 1){
			gText.loadFromRenderedText("CONTINUAR",redColor);
			gText.render(cox+25, coy,gText.getWidth(),gText.getHeight());
		} else{
			gText.loadFromRenderedText("CONTINUAR",whiteColor);
			gText.render(cox, coy,gText.getWidth(),gText.getHeight());
		}

		if (index == 2){
			gText.loadFromRenderedText("SALVAR",redColor);
			gText.render(sax+25, say,gText.getWidth(),gText.getHeight());
		} else {
			gText.loadFromRenderedText("SALVAR",whiteColor);
			gText.render(sax, say,gText.getWidth(),gText.getHeight());
		}

		if (index == 3){
			gText.loadFromRenderedText("CARGAR",redColor);
			gText.render(lox+25, loy, gText.getWidth(),gText.getHeight());
		}else{
			gText.loadFromRenderedText("CARGAR",whiteColor);
			gText.render(lox, loy, gText.getWidth(),gText.getHeight());
		}

		if (index == 4){
			gText.loadFromRenderedText("SALIR",redColor);
			gText.render(qux+25, quy,gText.getWidth(),gText.getHeight());

		}else{
			gText.loadFromRenderedText("SALIR",whiteColor);
			gText.render(qux, quy,gText.getWidth(),gText.getHeight());
		}

		SDL_RenderPresent(gRenderer);
	}
}



int main(int argc, char* args[]) {
	init();
	loadMedia();

	PauseMenu();

	close();//Free resources and close SDL
	return 0;
}
