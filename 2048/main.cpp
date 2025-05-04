#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "const.h"
#include "logic.h"
#include "graphics.h"
#include "utils.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Surface* surface = nullptr;
TTF_Font* font = nullptr;
SDL_Color FONT_COLOR = { 0, 0, 0, 255 };

int main(int argc, char* argv[])
{
    init();
    play_2048();
    quitSDL(window, renderer);
    return 0;
}