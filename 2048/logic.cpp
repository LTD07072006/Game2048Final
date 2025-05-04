#include "logic.h"
#include "const.h"
#include "graphics.h"
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstring>
using namespace std;

int board[ROWS][COLUMNS];

bool check_board_full()
{
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            if (!board[i][j]) return false;
        }
    }
    return true;
}

bool check_vaild()
{
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            if (board[i][j] == 0) return true;
            else if (i < ROWS - 1 && board[i][j] == board[i + 1][j]) return true;
            else if (j < COLUMNS - 1 && board[i][j] == board[i][j + 1]) return true;
        }
    }
    return false;
}

bool GameOver() {
    return (check_board_full() && !check_vaild());
}

bool Buttons(SDL_Renderer* renderer, TTF_Font* font, SDL_Event& event, bool& resetGame) {
    SDL_Rect ChoiLai = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 180, 200, 50 };
    SDL_Rect KetThuc = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 240, 200, 50 };

    SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);
    SDL_RenderFillRect(renderer, &ChoiLai);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &ChoiLai);

    SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
    SDL_RenderFillRect(renderer, &KetThuc);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &KetThuc);

    SDL_Color white = { 255, 255, 255 };
    SDL_Surface* replaySurf = TTF_RenderText_Solid(font, "Try Again.", white);
    SDL_Texture* replayText = SDL_CreateTextureFromSurface(renderer, replaySurf);
    SDL_Rect replayTextRect = {
        ChoiLai.x + (ChoiLai.w - replaySurf->w) / 2,
        ChoiLai.y + (ChoiLai.h - replaySurf->h) / 2,
        replaySurf->w,
        replaySurf->h
    };
    SDL_RenderCopy(renderer, replayText, NULL, &replayTextRect);

    SDL_Surface* quitSurf = TTF_RenderText_Solid(font, "Quit Game.", white);
    SDL_Texture* quitText = SDL_CreateTextureFromSurface(renderer, quitSurf);
    SDL_Rect quitTextRect = {
        KetThuc.x + (KetThuc.w - quitSurf->w) / 2,
        KetThuc.y + (KetThuc.h - quitSurf->h) / 2,
        quitSurf->w,
        quitSurf->h
    };
    SDL_RenderCopy(renderer, quitText, NULL, &quitTextRect);

    SDL_FreeSurface(replaySurf); SDL_DestroyTexture(replayText);
    SDL_FreeSurface(quitSurf); SDL_DestroyTexture(quitText);

    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int x = event.button.x;
        int y = event.button.y;

        if (x >= ChoiLai.x && x <= ChoiLai.x + ChoiLai.w && y >= ChoiLai.y && y <= ChoiLai.y + ChoiLai.h) {
            resetGame = true;
            return false;
        }

        if (x >= KetThuc.x && x <= KetThuc.x + KetThuc.w && y >= KetThuc.y && y <= KetThuc.y + KetThuc.h) {
            return true;
        }
    }
    return false;
}

int CinHighScore(const string& filename) {
    ifstream file(filename);
    int highscore = 0;
    if (file.is_open()) {
        file >> highscore;
        file.close();
    }
    return highscore;
}

void CoutHighScore(const string& filename, int highscore) {
    ofstream file(filename);
    if (file.is_open()) {
        file << highscore;
        file.close();
    }
}

void spawn_number() {
    if (check_board_full()) return;

    int empty_count = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            if (board[i][j] == 0) empty_count++;
        }
    }

    if (empty_count == 0) return;

    int target = rand() % empty_count + 1;
    int current = 0;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            if (board[i][j] == 0) {
                current++;
                if (current == target) {
                    board[i][j] = (rand() % 100 < 20) ? 4 : 2;
                    return;
                }
            }
        }
    }
}

void spawn_initial_numbers() {
    spawn_number();
    spawn_number();
}

void play_2048() {
    bool out = false;
    bool keydown = false;
    bool valid_move;

    string filename = "high_score.txt";
    int score = 0, highscore = CinHighScore(filename);
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            board[i][j] = 0;
        }
    }
    spawn_initial_numbers();

    SDL_Event event;
    bool enteredGameOver = false;

    while (!out) {
        render(score, highscore, font);
        bool resetGame = false;

        if (GameOver()) {
            if (!enteredGameOver) {
                enteredGameOver = true;
                SDL_SetWindowTitle(window, "Tro choi ket thuc! Nhan ENTER de choi lai. Nhan Q de ket thuc");

                if (score > highscore) {
                    highscore = score;
                    CoutHighScore(filename, highscore);
                }
            }

            bool quitGameByButton = false;
            SDL_Event gameOverEvent;
            while (SDL_PollEvent(&gameOverEvent)) {
                if (gameOverEvent.type == SDL_QUIT) {
                    out = true;
                    break;
                }
                else if (gameOverEvent.type == SDL_MOUSEBUTTONDOWN) {
                    bool tempQuit = Buttons(renderer, font, gameOverEvent, resetGame);
                    if (tempQuit) {
                        quitGameByButton = true;
                        break;
                    }
                }
                else if (gameOverEvent.type == SDL_KEYDOWN) {
                    switch (gameOverEvent.key.keysym.sym) {
                    case SDLK_q:
                        out = true;
                        break;
                    case SDLK_RETURN:
                        SDL_SetWindowTitle(window, WINDOW_TITLE);
                        for (int i = 0; i < ROWS; i++) {
                            for (int j = 0; j < COLUMNS; j++) {
                                board[i][j] = 0;
                            }
                        }
                        score = 0;
                        spawn_initial_numbers();
                        break;
                    }
                }
            }

            if (quitGameByButton) {
                out = true;
            }
            else if (resetGame) {
                score = 0;
                for (int r = 0; r < ROWS; r++) {
                    for (int c = 0; c < COLUMNS; c++) {
                        board[r][c] = 0;
                    }
                }
                enteredGameOver = false;
                spawn_initial_numbers();
                SDL_SetWindowTitle(window, WINDOW_TITLE);
            }
        }
        else {
            keydown = false;
            while (!keydown) {
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        keydown = true;
                        out = true;
                        break;
                    }
                    else if (event.type == SDL_KEYDOWN) {
                        keydown = true;
                        switch (event.key.keysym.sym) {
                        case SDLK_q:
                            out = true;
                            break;
                        case SDLK_UP:
                            // Kiểm tra có nước đi hợp lệ không
                            valid_move = false;
                            for (int col = 0; col < COLUMNS; col++) {
                                for (int row = 0; row < ROWS - 1; row++) {
                                    if ((board[row][col] == board[row + 1][col] && board[row][col] != 0) ||
                                        (board[row][col] == 0 && board[row + 1][col] != 0)) {
                                        valid_move = true;
                                        break;
                                    }
                                }
                                if (valid_move) break;
                            }
                            if (valid_move) {
                                for (int col = 0; col < COLUMNS; col++) {
                                    int temp[ROWS] = { 0 };
                                    int index = 0;
                                    // Bước 1: Dồn ô khác 0 (từ TRÊN xuống)
                                    for (int row = 0; row < ROWS; row++) {
                                        if (board[row][col] != 0) {
                                            temp[index++] = board[row][col];
                                        }
                                    }
                                    // Bước 2: Gộp ô giống nhau
                                    for (int i = 0; i < ROWS - 1; i++) {
                                        if (temp[i] == temp[i + 1] && temp[i] != 0) {
                                            temp[i] *= 2;
                                            score += temp[i];
                                            temp[i + 1] = 0;
                                        }
                                    }
                                    // Bước 3: Dồn lại sau khi gộp (để không có ô trống ở giữa)
                                    index = 0;
                                    int merged[ROWS] = { 0 };
                                    for (int i = 0; i < ROWS; i++) {
                                        if (temp[i] != 0) {
                                            merged[index++] = temp[i];
                                        }
                                    }
                                    // Bước 4: Cập nhật lại cột (đẩy LÊN đỉnh)
                                    for (int row = 0; row < ROWS; row++) {
                                        board[row][col] = (row < index) ? merged[row] : 0;
                                    }
                                }
                            }
                            spawn_number();
                            break;
                        case SDLK_DOWN:
                            // Kiểm tra có nước đi hợp lệ không
                            valid_move = false;
                            for (int col = 0; col < COLUMNS; col++) {
                                for (int row = 0; row < ROWS - 1; row++) {
                                    if ((board[row][col] == board[row + 1][col] && board[row][col] != 0) ||
                                        (board[row][col] != 0 && board[row + 1][col] == 0)) {
                                        valid_move = true;
                                        break;
                                    }
                                }
                                if (valid_move) break;
                            }
                            if (valid_move) {
                                for (int col = 0; col < COLUMNS; col++) {
                                    int temp[ROWS] = { 0 }, index = 0;
                                    // B1: Dồn ô khác 0 vào temp (từ DƯỚI lên)
                                    for (int row = ROWS - 1; row >= 0; row--) {
                                        if (board[row][col] != 0) {
                                            temp[index++] = board[row][col];
                                        }
                                    }
                                    // B2: Gộp ô giống nhau 
                                    for (int i = 0; i < ROWS - 1; i++) {
                                        if (temp[i] == temp[i + 1] && temp[i] != 0) {
                                            temp[i] *= 2;
                                            score += temp[i];
                                            temp[i + 1] = 0;
                                        }
                                    }
                                    // B3: Dồn lại sau khi gộp
                                    index = 0;
                                    int merged[ROWS] = { 0 };
                                    for (int i = 0; i < ROWS; i++) {
                                        if (temp[i] != 0) {
                                            merged[index++] = temp[i];
                                        }
                                    }
                                    // B4: Cập nhật lại cột (đẩy XUỐNG đáy)
                                    for (int row = 0; row < ROWS; row++) {
                                        board[row][col] = ((ROWS - 1 - row) < index) ? merged[ROWS - 1 - row] : 0;
                                    }
                                }
                            }
                            spawn_number();
                            break;
                        case SDLK_LEFT:
                            // Kiểm tra có nước đi hợp lệ không
                            valid_move = false;
                            for (int row = 0; row < ROWS; row++) {
                                for (int col = 0; col < COLUMNS - 1; col++) {
                                    if ((board[row][col] == board[row][col + 1] && board[row][col] != 0) ||
                                        (board[row][col] == 0 && board[row][col + 1] != 0)) {
                                        valid_move = true;
                                        break;
                                    }
                                }
                                if (valid_move) break;
                            }
                            if (valid_move) {
                                for (int row = 0; row < ROWS; row++) {
                                    int temp[COLUMNS] = { 0 };
                                    int index = 0;
                                    // Bước 1: Dồn các ô khác 0 về bên trái
                                    for (int col = 0; col < COLUMNS; col++) {
                                        if (board[row][col] != 0) {
                                            temp[index++] = board[row][col];
                                        }
                                    }
                                    // Bước 2: Gộp các ô giống nhau
                                    for (int i = 0; i < COLUMNS - 1; i++) {
                                        if (temp[i] == temp[i + 1] && temp[i] != 0) {
                                            temp[i] *= 2;
                                            score += temp[i];
                                            temp[i + 1] = 0;
                                        }
                                    }
                                    // Bước 3: Dồn lại sau khi gộp
                                    index = 0;
                                    for (int i = 0; i < COLUMNS; i++) {
                                        if (temp[i] != 0) {
                                            board[row][index++] = temp[i];
                                        }
                                    }
                                    // Bước 4: Điền các ô trống bên phải
                                    while (index < COLUMNS) {
                                        board[row][index++] = 0;
                                    }
                                }
                            }
                            spawn_number();
                            break;
                        case SDLK_RIGHT:
                            // Kiểm tra có nước đi hợp lệ không
                            valid_move = false;
                            for (int row = 0; row < ROWS; row++) {
                                for (int col = 0; col < COLUMNS - 1; col++) {
                                    if ((board[row][col] == board[row][col + 1] && board[row][col] != 0) ||
                                        (board[row][col + 1] == 0 && board[row][col] != 0)) {
                                        valid_move = true;
                                        break;
                                    }
                                }
                                if (valid_move) break;
                            }
                            if (valid_move) {
                                for (int row = 0; row < ROWS; row++) {
                                    int temp[COLUMNS] = { 0 };
                                    int index = 0;
                                    // B1: Dồn ô khác 0 (từ PHẢI sang trái)
                                    for (int col = COLUMNS - 1; col >= 0; col--) {
                                        if (board[row][col] != 0) {
                                            temp[index++] = board[row][col];
                                        }
                                    }
                                    // B2: Gộp ô giống nhau
                                    for (int i = 0; i < COLUMNS - 1; i++) {
                                        if (temp[i] == temp[i + 1] && temp[i] != 0) {
                                            temp[i] *= 2;
                                            score += temp[i];
                                            temp[i + 1] = 0;
                                        }
                                    }
                                    // B3: Dồn lại sau khi gộp
                                    index = 0;
                                    int merged[COLUMNS] = { 0 };
                                    for (int i = 0; i < COLUMNS; i++) {
                                        if (temp[i] != 0) {
                                            merged[index++] = temp[i];
                                        }
                                    }
                                    // B4: Cập nhật lại hàng (đẩy sang PHẢI)
                                    for (int col = 0; col < COLUMNS; col++) {
                                        board[row][COLUMNS - 1 - col] = (col < index) ? merged[col] : 0;
                                    }
                                }
                            }
                            spawn_number();
                            break;
                        case SDLK_r:
                            for (int i = 0; i < ROWS; i++) {
                                for (int j = 0; j < COLUMNS; j++) {
                                    board[i][j] = 0;
                                }
                            }
                            spawn_initial_numbers();
                            score = 0;
                            break;
                        }
                    }
                    SDL_Delay(16);
                }
                SDL_Delay(50);
            }
        }
    }
    SDL_RenderPresent(renderer);
    SDL_Delay(16);
}
