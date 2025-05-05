#ifndef UTILS_H
#define UTILS_H

#include <SDL.h>
#include <SDL_mixer.h>

extern Mix_Music* bgMusic;
void logErrorAndExit(const char* msg, const char* error);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer, Mix_Music* bgMusic);
void init();

#endif // UTILS_H 
