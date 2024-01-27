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
    SDL_Texture* background = Sage_loadTexture("assets/board_sm.png");
    SDL_FRect bckSrc = {0, 0, FELT_SRC_W, FELT_SRC_H};
    SDL_FRect bckDst = {0, 0, sage.width, sage.height};
    Sprite* back = Sprite_createEx(background, bckSrc, bckDst, Z_BACKGROUND);
    Sage_registerSprite(back);
}

//     SDL_Texture* boardTexture = Sage_loadTexture("assets/board_sm.png");
//     SDL_FRect boardSrc = {0, 0, BOARD_SRC_W, BOARD_SRC_H};
//     SDL_FRect boardDst = {0, 0, BOARD_SRC_W, BOARD_SRC_H};
//     boardDst.x = (FELT_SRC_W - BOARD_SRC_W) / 2;
//     boardDst.y = (FELT_SRC_H - BOARD_SRC_H) / 2;
//     Sprite* boardSprite = Sprite_createEx(boardTexture, boardSrc, boardDst, Z_BOARD);
//     Sage_registerSprite(boardSprite);
// }
