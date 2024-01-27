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

void Pips_create(void){
    extern Sage sage;
    char path[100];
    sprintf(path, "assets/%s/pips.png", sage.scale);
    SDL_Texture* pipTexture = Sage_loadTexture(path);
    int textureW, textureH;
    float pipW, pipH;
    SDL_QueryTexture(pipTexture, NULL, NULL, &textureW, &textureH);
    pipW = (float)textureW / 12;
    pipH = (float)textureH / 2;

    SDL_FRect pipSrc = {0, 0, pipW, pipH};
    SDL_FRect pipDst = pipSrc;
    float sideOffset = pipW / 4;
    float bottomOffset = pipW * 17.4;
    float topOffset = pipH * 0.9;
    float gapOffset = pipW * 0.4;
    Sprite* pipSprite;
    for (int32_t i = 0; i < 12; i++){
        pipSrc.x = (i * pipW);
        pipSrc.y = 0;
        pipDst.x = (i * pipW) + sideOffset;
        if (i >= 6) {
            pipDst.x += gapOffset;
        }
        
        pipDst.y = topOffset;
        pipSprite = Sprite_createEx(pipTexture, pipSrc, pipDst, Z_PIPS);
        Sage_registerSprite(pipSprite);

        pipSrc.y = pipH;
        pipDst.y = bottomOffset;
        pipSprite = Sprite_createEx(pipTexture, pipSrc, pipDst, Z_PIPS);
        Sage_registerSprite(pipSprite);
    }
}
