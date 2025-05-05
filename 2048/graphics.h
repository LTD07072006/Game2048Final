#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <map>

using namespace std;

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Surface* surface;
extern TTF_Font* font;
extern SDL_Color FONT_COLOR;
extern map<int, SDL_Texture*> textures;

void LoadTextures(int value);
void DrawScore(int score, int highscore, TTF_Font* font);
void DrawGameOverMessage(int score);
void DrawBackground(SDL_Renderer* renderer);
void render(int score, int highscore, TTF_Font* font);

#endif // GRAPHICS_H 
