/**
 * @file board.c
 * @author Michael Baumgarten
 * @brief Gameboard objects
 */
#include "board.h"
#include "sage.h"
#include "sprite.h"
#include "util.h"
#include <SDL3/SDL.h>

void createBoardSprites(void) {
    SDL_Texture* background = Sage_loadTexture("assets/felt.png");
    SDL_FRect feltSrcRect = {0, 0, FELT_SRC_W, FELT_SRC_H};
    SDL_FRect feltDstRect = {0, 0, FELT_W, FELT_H};
    Sprite* back = Sprite_createEx(background, feltSrcRect, feltDstRect, Z_BACKGROUND);
    Sage_registerSprite(back);

    SDL_Texture* trimTexture = Sage_loadTexture("assets/trim2x.png");
    SDL_FRect trimSrcRect = {0, 0, TRIM_SRC_W*2, TRIM_SRC_H*2};
    SDL_FRect trimDstRect = {TRIM_X, TRIM_Y, TRIM_W, TRIM_H};
    Sprite* trimSprite = Sprite_createEx(trimTexture, trimSrcRect, trimDstRect, Z_TRIM);
    Sage_registerSprite(trimSprite);
    Sage_setViewport(trimDstRect);

    SDL_Texture* boardTexture = Sage_loadTexture("assets/board2x.png");
    SDL_FRect boardSrcRect = {0, 0, BOARD_SRC_W*2, BOARD_SRC_H*2};
    SDL_FRect boardDstRect = {BOARD_X, BOARD_Y, BOARD_W, BOARD_H};
    Sprite* boardSprite = Sprite_createEx(boardTexture, boardSrcRect, boardDstRect, Z_BOARD);
    boardSprite->useViewport = true;
    Sage_registerSprite(boardSprite);
}
