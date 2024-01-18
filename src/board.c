/**
 * @file board.c
 * @author Michael Baumgarten
 * @brief Gameboard and trim objects
 */
#include "board.h"

#define BOARD_SRC_W 1461
#define BOARD_SRC_H 1000
#define TRIM_SRC_W 1584
#define TRIM_SRC_H 1104
#define PIP_SRC_W 90
#define PIP_SRC_H 451
#define PIP_RATIO 4.956

#define PIP_BOARD_X_OFFSET_PCT .0382
#define PIP_BOARD_WIDTH_PCT .0623

#define TRIM_BOARD_X_OFFSET_PCT .0392
#define TRIM_BOARD_Y_OFFSET_PCT .0472
#define TRIM_BOARD_WIDTH_PCT 1.0842
#define TRIM_BOARD_HEIGHT_PCT 1.104

#define BOARD_W_PERCENT .8
#define BOARD_H_PERCENT .55


void createPips(Sage* sage, SDL_Rect board){
    SDL_Texture* pip = Sage_loadTexture(sage, "assets/pips.png"); 
    for (int32_t i = 0; i < 6; i++){
        SDL_Rect pipSrc = {PIP_SRC_W*i, 0, PIP_SRC_W, PIP_SRC_H};
        int32_t pipWidth = board.w * PIP_BOARD_WIDTH_PCT;
        int32_t pipHeight = pipWidth * PIP_RATIO;
        int32_t pipX = board.x + board.w * PIP_BOARD_X_OFFSET_PCT + (i * pipWidth) + pipWidth;
        int32_t pipY = board.y;

        SDL_Rect pipDst = {pipX, pipY, pipWidth, pipHeight};
        Sprite* pipSprite = Sprite_createEx(pip, pipSrc, pipDst, 3, false);
        Sage_registerSprite(sage, pipSprite);
    }
    for (int32_t i = 0; i < 6; i++){
        SDL_Rect pipSrc = {PIP_SRC_W*(i+6), 0, PIP_SRC_W, PIP_SRC_H};
        int32_t pipWidth = board.w * PIP_BOARD_WIDTH_PCT;
        int32_t pipHeight = pipWidth * PIP_RATIO;
        int32_t pipX = (board.w + board.x) - (board.w * PIP_BOARD_X_OFFSET_PCT) + (i * pipWidth) - (pipWidth * 7);
        int32_t pipY = board.y;

        SDL_Rect pipDst = {pipX, pipY, pipWidth, pipHeight};
        Sprite* pipSprite = Sprite_createEx(pip, pipSrc, pipDst, 3, false);
        Sage_registerSprite(sage, pipSprite);
    }
    for (int32_t i = 0; i < 6; i++){
        SDL_Rect pipSrc = {PIP_SRC_W*i, PIP_SRC_H, PIP_SRC_W, PIP_SRC_H};
        int32_t pipWidth = board.w * PIP_BOARD_WIDTH_PCT;
        int32_t pipHeight = pipWidth * PIP_RATIO;
        int32_t pipX = board.x + board.w * PIP_BOARD_X_OFFSET_PCT + (i * pipWidth) + pipWidth;
        int32_t pipY = board.y + board.h - pipHeight;

        SDL_Rect pipDst = {pipX, pipY, pipWidth, pipHeight};
        Sprite* pipSprite = Sprite_createEx(pip, pipSrc, pipDst, 3, false);
        Sage_registerSprite(sage, pipSprite);
    }

    for (int32_t i = 0; i < 6; i++){
        SDL_Rect pipSrc = {PIP_SRC_W*(i+6), PIP_SRC_H, PIP_SRC_W, PIP_SRC_H};
        int32_t pipWidth = board.w * PIP_BOARD_WIDTH_PCT;
        int32_t pipHeight = pipWidth * PIP_RATIO;
        int32_t pipX = (board.w + board.x) - (board.w * PIP_BOARD_X_OFFSET_PCT) + (i * pipWidth) - (pipWidth * 7);
        int32_t pipY = board.y + board.h - pipHeight;

        SDL_Rect pipDst = {pipX, pipY, pipWidth, pipHeight};
        Sprite* pipSprite = Sprite_createEx(pip, pipSrc, pipDst, 2, false);
        Sage_registerSprite(sage, pipSprite);
    }
}

void createTrim(Sage* sage, SDL_Rect board){
    SDL_Texture* trim = Sage_loadTexture(sage, "assets/trim.png");
    int32_t trimWidth = board.w * TRIM_BOARD_WIDTH_PCT;
    int32_t trimHeight = board.h * TRIM_BOARD_HEIGHT_PCT;
    int32_t trimX = board.x - (board.w * TRIM_BOARD_X_OFFSET_PCT);
    int32_t trimY = board.y - (board.h * TRIM_BOARD_Y_OFFSET_PCT);
    SDL_Rect trimSrc = {0, 0, TRIM_SRC_W, TRIM_SRC_H};
    SDL_Rect trimDst = {trimX, trimY, trimWidth, trimHeight};
    Sprite* trimSprite = Sprite_createEx(trim, trimSrc, trimDst, 3, false);
    Sage_registerSprite(sage, trimSprite);
}

void Board_create(Sage* sage, int32_t gameWidth, int32_t gameHeight){
    SDL_Texture* background = Sage_loadTexture(sage, "assets/background.png");
    SDL_Rect bckSrc = {0, 0, BOARD_SRC_W, BOARD_SRC_H};
    SDL_Rect bckDst = {0, 0, gameWidth, gameHeight};
    Sprite* back = Sprite_createEx(background, bckSrc, bckDst, 0, false);
    Sage_registerSprite(sage, back);

    SDL_Texture* board = Sage_loadTexture(sage, "assets/board.png");
    SDL_Rect boardSrc = {0, 0, BOARD_SRC_W, BOARD_SRC_H}; 
    int32_t boardHeight = gameHeight * BOARD_H_PERCENT;
    int32_t boardWidth = gameWidth * BOARD_W_PERCENT;
    int32_t boardX = (gameWidth - boardWidth) / 2;
    int32_t boardY = (gameHeight - boardHeight) / 2;
    SDL_Rect boardDst = {boardX, boardY, boardWidth, boardHeight};
    Sprite* boardSprite = Sprite_createEx(board, boardSrc, boardDst, 1, false);
    Sage_registerSprite(sage, boardSprite);

    createPips(sage, boardDst);
    createTrim(sage, boardDst);
}
