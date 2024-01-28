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

extern uint32_t PipClickEventType;


void handlePipClick(uint32_t eventType, SDL_Event* e, void* data){
    (void)data;
    (void)eventType;
    uint32_t pipIndex = e->user.code;
    log_debug("pip %d clicked", pipIndex);
}

void Board_create(void){
    extern Sage sage;
    //int w, h;
    //SDL_GetWindowSizeInPixels(sage.mm->window, &w, &h);
    SDL_Texture* background = Sage_loadSVGTexture("assets/felt.svg", FELT_W, FELT_H);
    SDL_FRect feltRect = {0, 0, FELT_W, FELT_H};
    Sprite* back = Sprite_createEx(background, feltRect, feltRect, Z_BACKGROUND);
    Sage_registerSprite(back);

    SDL_Texture* boardTexture = Sage_loadSVGTexture("assets/board.svg", BOARD_H, BOARD_W);
    SDL_FRect boardSrcRect = {0, 0, BOARD_W, BOARD_H};
    SDL_FRect boardDstRect = {BOARD_X, BOARD_Y, BOARD_W, BOARD_H};
    Sprite* boardSprite = Sprite_createEx(boardTexture, boardSrcRect, boardDstRect, Z_BOARD);
    Sage_registerSprite(boardSprite);
    Sage_setViewport(boardDstRect);

    PipClickEventType = SDL_RegisterEvents(1);
    Sage_registerEventCallback(PipClickEventType, handlePipClick, NULL);
}
