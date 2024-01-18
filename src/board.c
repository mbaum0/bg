/**
 * @file board.c
 * @author Michael Baumgarten
 * @brief Gameboard and trim objects
 */
#include "board.h"

#define BOARD_W_PERCENT .9
#define BOARD_H_PERCENT .55

void Board_create(Sage* sage, int32_t gameWidth, int32_t gameHeight){
    SDL_Texture* background = Sage_loadTexture(sage, "assets/background.png");
    SDL_Rect bckSrc = {0, 0, 2571, 1805};
    SDL_Rect bckDst = {0, 0, gameWidth, gameHeight};
    Sprite* back = Sprite_createEx(background, bckSrc, bckDst, 0, false);
    Sage_registerSprite(sage, back);

    SDL_Texture* trim = Sage_loadTexture(sage, "assets/trim.png");
    int32_t trimHeight = gameHeight * BOARD_H_PERCENT;
    int32_t trimWidth = gameWidth * BOARD_W_PERCENT;
    int32_t trimX = (gameWidth - trimWidth) / 2;
    int32_t trimY = (gameHeight - trimHeight) / 2;
    SDL_Rect trimSrc = {0, 0, 1584, 1104};
    SDL_Rect trimDst = {trimX, trimY, trimWidth, trimHeight};
    Sprite* trimSprite = Sprite_createEx(trim, trimSrc, trimDst, 2, false);
    Sage_registerSprite(sage, trimSprite);

    SDL_Texture* board = Sage_loadTexture(sage, "assets/board.png");
    SDL_Rect boardSrc = {0, 0, 1461, 1000}; 
    int32_t boardHeight = trimHeight * .92;
    int32_t boardWidth = trimWidth * .93;
    int32_t boardX = trimX + (trimWidth - boardWidth) / 2;
    int32_t boardY = trimY + (trimHeight - boardHeight) / 2;
    SDL_Rect boardDst = {boardX, boardY, boardWidth, boardHeight};
    Sprite* boardSprite = Sprite_createEx(board, boardSrc, boardDst, 1, false);
    Sage_registerSprite(sage, boardSprite);
}
