/**
 * @file board.c
 * @author Michael Baumgarten
 * @brief Gameboard and trim objects
 */
#include "board.h"
#include "util.h"

void createPips(Sage* sage){
    SDL_Texture* pip = Sage_loadTexture(sage, "assets/pips_sm.png"); 
    for (int32_t i = 0; i < 6; i++){
        float x = PIP_RIGHT_X_OFFSET_NORMAL + (PIP_WIDTH_NORMAL * i);
        float y = PIP_BOTTOM_Y_OFFSET_NORMAL;
        SDL_FRect pipDst = {x, y, PIP_WIDTH_NORMAL, PIP_HEIGHT_NORMAL};

        SDL_Rect pipSrc = {PIP_SRC_W*(i+6), PIP_SRC_H, PIP_SRC_W, PIP_SRC_H};
        Sprite* pipSprite = Sprite_createEx(pip, pipSrc, pipDst, Z_PIPS, false, true, true);
        Sage_registerSprite(sage, pipSprite);
    }
    for (int32_t i = 0; i < 6; i++){
        float x = PIP_LEFT_X_OFFSET_NORMAL + (PIP_WIDTH_NORMAL * i);
        float y = PIP_BOTTOM_Y_OFFSET_NORMAL;
        SDL_FRect pipDst = {x, y, PIP_WIDTH_NORMAL, PIP_HEIGHT_NORMAL};

        SDL_Rect pipSrc = {PIP_SRC_W*(i), PIP_SRC_H, PIP_SRC_W, PIP_SRC_H};
        Sprite* pipSprite = Sprite_createEx(pip, pipSrc, pipDst, Z_PIPS, false, true, true);
        Sage_registerSprite(sage, pipSprite);
    }
    for (int32_t i = 0; i < 6; i++){
        float x = PIP_LEFT_X_OFFSET_NORMAL + (PIP_WIDTH_NORMAL * i);
        float y = PIP_TOP_Y_OFFSET_NORMAL;
        SDL_FRect pipDst = {x, y, PIP_WIDTH_NORMAL, PIP_HEIGHT_NORMAL};

        SDL_Rect pipSrc = {PIP_SRC_W*(i), 0, PIP_SRC_W, PIP_SRC_H};
        Sprite* pipSprite = Sprite_createEx(pip, pipSrc, pipDst, Z_PIPS, false, true, true);
        Sage_registerSprite(sage, pipSprite);
    }
    for (int32_t i = 0; i < 6; i++){
        float x = PIP_RIGHT_X_OFFSET_NORMAL + (PIP_WIDTH_NORMAL * i);
        float y = PIP_TOP_Y_OFFSET_NORMAL;
        SDL_FRect pipDst = {x, y, PIP_WIDTH_NORMAL, PIP_HEIGHT_NORMAL};

        SDL_Rect pipSrc = {PIP_SRC_W*(i+6), 0, PIP_SRC_W, PIP_SRC_H};
        Sprite* pipSprite = Sprite_createEx(pip, pipSrc, pipDst, Z_PIPS, false, true, true);
        Sage_registerSprite(sage, pipSprite);
    }
}

void Board_create(Sage* sage){
    SDL_Texture* background = Sage_loadTexture(sage, "assets/background.png");
    SDL_Rect bckSrc = {0, 0, BOARD_SRC_W, BOARD_SRC_H};
    SDL_FRect bckDst = {0, 0, sage->width, sage->height};
    Sprite* back = Sprite_createEx(background, bckSrc, bckDst, Z_BACKGROUND, false, false, false);
    Sage_registerSprite(sage, back);

    SDL_Texture* trimTexture = Sage_loadTexture(sage, "assets/trim.png");
    SDL_Rect trimSrc = {0, 0, TRIM_SRC_W, TRIM_SRC_H};
    SDL_FRect trimDst = {TRIM_X_NORMAL, TRIM_Y_NORMAL, TRIM_W_NORMAL, TRIM_H_NORMAL};
    Sprite* trimSprite = Sprite_createEx(trimTexture, trimSrc, trimDst, Z_TRIM, false, true, false);
    Sage_registerSprite(sage, trimSprite);

    SDL_Texture* boardTexture = Sage_loadTexture(sage, "assets/board.png");
    SDL_Rect boardSrc = {0, 0, BOARD_SRC_W, BOARD_SRC_H};
    SDL_FRect boardDst = {BOARD_X_NORMAL, BOARD_Y_NORMAL, BOARD_W_NORMAL, BOARD_H_NORMAL};
    Sprite* boardSprite = Sprite_createEx(boardTexture, boardSrc, boardDst, Z_BOARD, false, true, false);
    Sage_registerSprite(sage, boardSprite);

    createPips(sage);
    //createTrim(sage);
}
