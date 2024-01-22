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

void createPips(void){
    SDL_Texture* pip = Sage_loadTexture("assets/pips_sm.png"); 
    for (int32_t i = 0; i < 6; i++){
        float x = PIP_RIGHT_X_OFFSET_NORMAL + (PIP_WIDTH_NORMAL * i);
        float y = PIP_BOTTOM_Y_OFFSET_NORMAL;
        SDL_FRect pipDst = {x, y, PIP_WIDTH_NORMAL, PIP_HEIGHT_NORMAL};

        SDL_FRect pipSrc = {PIP_SRC_W*(i+6), PIP_SRC_H, PIP_SRC_W, PIP_SRC_H};
        Sprite* pipSprite = Sprite_createEx(pip, pipSrc, pipDst, Z_PIPS, false, true, true, true);
        Sage_registerSprite(pipSprite);
    }
    for (int32_t i = 0; i < 6; i++){
        float x = PIP_LEFT_X_OFFSET_NORMAL + (PIP_WIDTH_NORMAL * i);
        float y = PIP_BOTTOM_Y_OFFSET_NORMAL;
        SDL_FRect pipDst = {x, y, PIP_WIDTH_NORMAL, PIP_HEIGHT_NORMAL};

        SDL_FRect pipSrc = {PIP_SRC_W*(i), PIP_SRC_H, PIP_SRC_W, PIP_SRC_H};
        Sprite* pipSprite = Sprite_createEx(pip, pipSrc, pipDst, Z_PIPS, false, true, true, true);
        Sage_registerSprite(pipSprite);
    }
    for (int32_t i = 0; i < 6; i++){
        float x = PIP_LEFT_X_OFFSET_NORMAL + (PIP_WIDTH_NORMAL * i);
        float y = PIP_TOP_Y_OFFSET_NORMAL;
        SDL_FRect pipDst = {x, y, PIP_WIDTH_NORMAL, PIP_HEIGHT_NORMAL};

        SDL_FRect pipSrc = {PIP_SRC_W*(i), 0, PIP_SRC_W, PIP_SRC_H};
        Sprite* pipSprite = Sprite_createEx(pip, pipSrc, pipDst, Z_PIPS, false, true, true, false);
        Sage_registerSprite(pipSprite);
    }
    for (int32_t i = 0; i < 6; i++){
        float x = PIP_RIGHT_X_OFFSET_NORMAL + (PIP_WIDTH_NORMAL * i);
        float y = PIP_TOP_Y_OFFSET_NORMAL;
        SDL_FRect pipDst = {x, y, PIP_WIDTH_NORMAL, PIP_HEIGHT_NORMAL};

        SDL_FRect pipSrc = {PIP_SRC_W*(i+6), 0, PIP_SRC_W, PIP_SRC_H};
        Sprite* pipSprite = Sprite_createEx(pip, pipSrc, pipDst, Z_PIPS, false, true, true, false);
        Sage_registerSprite(pipSprite);
    }
}

void Board_create(void){
    extern Sage sage;
    SDL_Texture* background = Sage_loadTexture("assets/background.png");
    SDL_FRect bckSrc = {0, 0, BOARD_SRC_W, BOARD_SRC_H};
    SDL_FRect bckDst = {0, 0, sage.width, sage.height};
    Sprite* back = Sprite_createEx(background, bckSrc, bckDst, Z_BACKGROUND, false, false, false, false);
    Sage_registerSprite(back);

    SDL_Texture* trimTexture = Sage_loadTexture("assets/trim.png");
    SDL_FRect trimSrc = {0, 0, TRIM_SRC_W, TRIM_SRC_H};
    SDL_FRect trimDst = {TRIM_X_NORMAL, TRIM_Y_NORMAL, TRIM_W_NORMAL, TRIM_H_NORMAL};
    Sprite* trimSprite = Sprite_createEx(trimTexture, trimSrc, trimDst, Z_TRIM, false, true, false, false);
    Sage_registerSprite(trimSprite);

    SDL_Texture* boardTexture = Sage_loadTexture("assets/board.png");
    SDL_FRect boardSrc = {0, 0, BOARD_SRC_W, BOARD_SRC_H};
    SDL_FRect boardDst = {BOARD_X_NORMAL, BOARD_Y_NORMAL, BOARD_W_NORMAL, BOARD_H_NORMAL};
    Sprite* boardSprite = Sprite_createEx(boardTexture, boardSrc, boardDst, Z_BOARD, false, true, false, false);
    Sage_registerSprite(boardSprite);

    createPips();
}
