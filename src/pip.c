/**
 * @file pip.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#include <SDL3/SDL.h>
#include <stdint.h>
#include "sprite.h"
#include "pip.h"
#include "sage.h"
#include "util.h"


void createPipSprite(SDL_Texture* texture, int index) {
    SDL_FRect pipSrc = { 0, 0, PIP_W, PIP_H };
    SDL_FRect pipDst = pipSrc;
    if (isBetween(index, 0, 5)) {
        pipSrc.x = (index * PIP_W);
        pipSrc.y = 0;
        pipDst.x = PIP_13_X + (index * PIP_W);
        pipDst.y = PIP_13_Y;

    } else if (isBetween(index, 6, 11)){
        pipSrc.x = (index) * PIP_W;
        pipSrc.y = 0;
        pipDst.x = PIP_19_X + ((index-6) * PIP_W);
        pipDst.y = PIP_19_Y;

    } else if (isBetween(index, 12, 17)){
        pipSrc.x = (index-12) * PIP_W;
        pipSrc.y = PIP_H;
        pipDst.x = PIP_12_X + ((index-12) * PIP_W);
        pipDst.y = PIP_12_Y; 
    } else if (isBetween(index, 18, 23)){
        pipSrc.x = (index-12) * PIP_W;
        pipSrc.y = PIP_H;
        pipDst.x = PIP_6_X + ((index-18) * PIP_W);
        pipDst.y = PIP_6_Y; 
    }
    Sprite* pipSprite;
    pipSprite = Sprite_createEx(texture, pipSrc, pipDst, Z_PIPS);
    pipSprite->useViewport = true;
    Sage_registerSprite(pipSprite);
    
}

void Pips_create(void) {
    extern Sage sage;
    int sheetWidth = PIP_W * 12;
    int sheetHeight = PIP_H * 2;
    SDL_Texture* texture = Sage_loadSVGTexture("assets/pips.svg", sheetWidth, sheetHeight);
    for (int32_t i = 0; i < 24; i++){
        createPipSprite(texture, i);
    }
}
