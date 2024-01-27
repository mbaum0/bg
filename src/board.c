/**
 * @file board.c
 * @author Michael Baumgarten
 * @brief Gameboard objects
 */
#include <SDL3/SDL.h>
#include "sprite.h"
#include "board.h"
#include "sage.h"
#include "util.h"


void Board_create(void){
    extern Sage sage;
    int w, h;
    SDL_GetWindowSizeInPixels(sage.mm->window, &w, &h);
    SDL_Texture* background = Sage_loadSVGTexture("assets/felt.svg", w, 0);
    int feltW, feltH, boardW, boardH;
    SDL_QueryTexture(background, NULL, NULL, &feltW, &feltH);
    SDL_FRect feltRect = {0, 0, feltW, feltH};
    Sprite* back = Sprite_createEx(background, feltRect, feltRect, Z_BACKGROUND);
    Sage_registerSprite(back);

    SDL_Texture* boardTexture = Sage_loadSVGTexture("assets/board.svg", w, 0);
    SDL_QueryTexture(boardTexture, NULL, NULL, &boardW, &boardH);
    SDL_FRect boardSrc = {0, 0, boardW, boardH};
    SDL_FRect boardDst = boardSrc;
    boardDst.x = 0;
    boardDst.y = (h - boardH) / 2;
    Sprite* boardSprite = Sprite_createEx(boardTexture, boardSrc, boardDst, Z_BOARD);
    Sage_registerSprite(boardSprite);
}
