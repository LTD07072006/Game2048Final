#include <SDL_image.h>
#include <SDL_ttf.h>
#include "graphics.h"
#include "logic.h"
#include "const.h"
#include <ctime>

void logErrorAndExit(const char* msg, const char* error)
{
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
    SDL_Quit();
}
void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
    // Giải phóng font nếu đã load
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }

    TTF_Quit();  // Đóng SDL_ttf
    IMG_Quit();
    // Clean SDL...
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void init()
{
    srand(static_cast<unsigned int>(time(NULL)));
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logErrorAndExit("SDL_Init", SDL_GetError());
    }
    if (TTF_Init() < 0) {
        SDL_Log("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        TTF_Quit();
        SDL_Quit();
        return;
    }
    int imgFlags = IMG_INIT_PNG;
    int initted = IMG_Init(imgFlags);
    if ((initted & imgFlags) != imgFlags) {
        SDL_Log("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return;
    }

    font = TTF_OpenFont("font/NunitoSans-Italic.ttf", 20);
    if (font == nullptr) {
        SDL_Log("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return;
    }

    window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        logErrorAndExit("CreateWindow", SDL_GetError());
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        logErrorAndExit("CreateRenderer", SDL_GetError());
    }


    int values[] = { 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048 };
    int size = sizeof(values) / sizeof(values[0]);
    for (int i = 0; i < size; i++) {
        int val = values[i];
        LoadTextures(val);
    }
}

