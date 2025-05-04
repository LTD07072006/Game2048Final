#include "graphics.h"
#include "const.h"
#include "logic.h"
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <map>

using namespace std;

map<int, SDL_Texture*> textures;

void LoadTextures(int value)
{
    char* basePath = SDL_GetBasePath();
    string name = string(basePath) + to_string(value) + ".png";
    SDL_free(basePath);

    cout << "Dang tai anh tu: " << name << endl;
    SDL_Surface* surface = IMG_Load(name.c_str());
    if (surface) {
        textures[value] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    else {
        cout << "Khong the load anh. Loi :" << IMG_GetError() << endl;
    }
}

void DrawScore(int score, int highscore, TTF_Font* font)
{
    string ScoreString = "SCORE: " + to_string(score);
    SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, ScoreString.c_str(), FONT_COLOR);
    if (!scoreSurface) {
        SDL_Log("Failed to render score text surface: %s", TTF_GetError());
        return;
    }

    // tạo score nói riêng
    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    int scoreWidth = scoreSurface->w;
    int scoreHeight = scoreSurface->h;
    SDL_FreeSurface(scoreSurface);
    SDL_Rect ScoreRect = { 10, 10, scoreWidth, scoreHeight };
    SDL_RenderCopy(renderer, scoreTexture, NULL, &ScoreRect);
    SDL_DestroyTexture(scoreTexture);

    // tạo highscore 
    string HighScoreString = "HIGH SCORE: " + to_string(highscore);
    SDL_Surface* HighScoreSurface = TTF_RenderText_Solid(font, HighScoreString.c_str(), FONT_COLOR);
    if (!HighScoreSurface) {
        SDL_Log("Failed to render high score text surface: %s", TTF_GetError());
        return;
    }
    SDL_Texture* highTexture = SDL_CreateTextureFromSurface(renderer, HighScoreSurface);
    int highWidth = HighScoreSurface->w;
    int highHeight = HighScoreSurface->h;

    SDL_FreeSurface(HighScoreSurface);
    SDL_Rect HighScoreRect = { SCREEN_WIDTH - highWidth - 10 , 10 , highWidth, highHeight };
    SDL_RenderCopy(renderer, highTexture, NULL, &HighScoreRect);
    SDL_DestroyTexture(highTexture);
}

void DrawGameOverMessage(int score)
{
    std::string msg = "Diem cua ban la: " + to_string(score) + ". Nhan ENTER de choi lai.Nhan Q de ket thuc";

    // tránh trùng với biến toàn cục
    SDL_Surface* msgSurface = TTF_RenderText_Solid(font, msg.c_str(), FONT_COLOR);
    if (!msgSurface) {
        SDL_Log("Failed to render game over message: %s", TTF_GetError());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, msgSurface);
    if (!texture) {
        SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
        SDL_FreeSurface(msgSurface);
        return;
    }

    // Hiển thị ở góc dưới bên trái
    int textWidth = msgSurface->w;
    int textHeight = msgSurface->h;
    SDL_FreeSurface(msgSurface);

    int x = SCREEN_WIDTH / 50;  // Lề trái
    int y = SCREEN_HEIGHT - textHeight - 10;  // Lề dưới

    SDL_Rect dstRect = { x, y, textWidth, textHeight };
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    SDL_DestroyTexture(texture);
}
void DrawBackground(SDL_Renderer* renderer) {

    SDL_SetRenderDrawColor(renderer, 237, 224, 200, 255); // màu nền (background)
    SDL_RenderClear(renderer); // tô màu toàn bộ window
}

void render(int score, int highscore, TTF_Font* font)
{
    DrawBackground(renderer);
    
    const int MAX_BOARD_SIZE = 500;
    int TILE_SIZE_DYNAMIC = (MAX_BOARD_SIZE - (COLUMNS + 1) * TILE_MARGIN) / COLUMNS;

    // Cập nhật lại chiều rộng và chiều cao của bảng
    int board_width = COLUMNS * TILE_SIZE_DYNAMIC + (COLUMNS + 1) * TILE_MARGIN;
    int board_height = ROWS * TILE_SIZE_DYNAMIC + (ROWS + 1) * TILE_MARGIN;

    // Tính vị trí vẽ bảng để căn giữa màn hình
    int offsetX = (SCREEN_WIDTH - board_width) / 2;
    int offsetY = (SCREEN_HEIGHT - board_height) / 2 - 80;

    // Vẽ từng ô
    SDL_Rect tile;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            tile.x = offsetX + TILE_MARGIN + j * (TILE_SIZE_DYNAMIC + TILE_MARGIN);
            tile.y = offsetY + TILE_MARGIN + i * (TILE_SIZE_DYNAMIC + TILE_MARGIN);
            tile.w = TILE_SIZE_DYNAMIC;
            tile.h = TILE_SIZE_DYNAMIC;
            int val = board[i][j];
            if (textures.count(val)) {
                SDL_RenderCopy(renderer, textures[val], NULL, &tile);
            }
        }
    }
    DrawScore(score, highscore, font);
    if (GameOver()) {
        DrawGameOverMessage(score);
        bool dummy = false;
        SDL_Event dummyEvent;
        Buttons(renderer, font, dummyEvent, dummy);
    }
    SDL_RenderPresent(renderer);
    SDL_Delay(50);

}
