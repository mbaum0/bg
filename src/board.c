/**
 * @file board.c
 * @author Michael Baumgarten
 * @brief Gameboard and trim objects
 */
#include <SDL3/SDL.h>
#include "sprite.h"
#include "board.h"
#include "sage.h"
#include "util.h"


void Board_create(void){
    extern Sage sage;
    char path[100];
    sprintf(path, "assets/%s/felt.png", sage.scale);
    SDL_Texture* background = Sage_loadTexture(path);
    int feltW, feltH, boardW, boardH;
    SDL_QueryTexture(background, NULL, NULL, &feltW, &feltH);
    SDL_FRect feltRect = {0, 0, feltW, feltH};
    Sprite* back = Sprite_createEx(background, feltRect, feltRect, Z_BACKGROUND);
    Sage_registerSprite(back);

    sprintf(path, "assets/%s/board.png", sage.scale);
    SDL_Texture* boardTexture = Sage_loadTexture(path);
    SDL_QueryTexture(boardTexture, NULL, NULL, &boardW, &boardH);
    SDL_FRect boardSrc = {0, 0, boardW, boardH};
    SDL_FRect boardDst = boardSrc;
    boardDst.x = (feltW - boardW) / 2;
    boardDst.y = (feltH - boardH) / 2;
    Sprite* boardSprite = Sprite_createEx(boardTexture, boardSrc, boardDst, Z_BOARD);
    Sage_registerSprite(boardSprite);
}
