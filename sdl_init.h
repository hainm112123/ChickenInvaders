#ifndef SDL_INIT_H_INCLUDED
#define SDL_INIT_H_INCLUDED

#include <iostream>

#include "init.h"

using namespace std;

const string WINDOW_TITLE = "Chicken Invaders";

void logSDLError(ostream &os, const string &msg, bool fatal = false);

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);

void quitSDL(SDL_Window *window, SDL_Renderer *renderer);

void waitUntilKeyPressed();

#endif // SDL_INIT_H_INCLUDED
