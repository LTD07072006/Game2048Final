#ifndef LOGIC_H
#define LOGIC_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "const.h"

extern int board[ROWS][COLUMNS];

bool check_board_full();
bool check_vaild();
bool GameOver();
bool Buttons(SDL_Renderer* renderer, TTF_Font* font, SDL_Event& event, bool& resetGame);
int CinHighScore(const std::string& filename);
void CoutHighScore(const std::string& filename, int highscore);
void spawn_number();
void spawn_initial_numbers();
void play_2048();

#endif // LOGIC_H
