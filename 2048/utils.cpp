#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "graphics.h"
#include "logic.h"
#include "const.h"
#include <ctime>

void logErrorAndExit(const char* msg, const char* error)
{
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
    SDL_Quit();
}
void quitSDL(SDL_Window* window, SDL_Renderer* renderer, Mix_Music* bgMusic)
{
    // Giải phóng font nếu đã load
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
    Mix_FreeMusic(bgMusic);
    bgMusic = nullptr;
    Mix_CloseAudio();
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
        Mix_CloseAudio();
        return;
    }
    int imgFlags = IMG_INIT_PNG;
    int initted = IMG_Init(imgFlags);
    if ((initted & imgFlags) != imgFlags) {
        SDL_Log("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        Mix_CloseAudio();
        return;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_Log("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
    }

    font = TTF_OpenFont("font/NunitoSans-Italic.ttf", 20);
    if (font == nullptr) {
        SDL_Log("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        Mix_CloseAudio();
        return;
    }

    window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        logErrorAndExit("CreateWindow", SDL_GetError());
    }

    Mix_Music* bgMusic = Mix_LoadMUS("07. Starlight.mp3");
    if (!bgMusic) {
        SDL_Log("Failed to load background music! SDL_mixer Error: %s\n", Mix_GetError());
    }
    else {
        Mix_PlayMusic(bgMusic, -1);
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

