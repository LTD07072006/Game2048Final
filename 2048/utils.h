#ifndef UTILS_H
#define UTILS_H

#include <SDL.h>

void logErrorAndExit(const char* msg, const char* error);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void init();

#endif // UTILS_H 
