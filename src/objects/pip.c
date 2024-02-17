/**
 * @file pip.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#include "pip.h"
#include "fsm.h"
#include "sage.h"
#include "sprite.h"
#include "util.h"
#include <SDL3/SDL.h>
#include <stdint.h>

extern Sage sage;

void clickPip(ViewManager* vm, Sprite* sprite, void* object, void* context, int32_t code) {
    (void)vm;
    (void)sprite;
    (void)context;
    (void)object;

    FSMEvent e = {PIP_CLICKED_EVENT, code, NULL};
    fsm_enqueue_event(e);
}

void createPipSprite(SDL_Texture* texture, int32_t index) {
    SDL_FRect pipSrc = {0, 0, 1, 1};
    SDL_FRect pipDst = {0, 0, PIP_W, PIP_H};
    if (isBetween(index, 0, 5)) {
        pipDst.x = PIP_13_X + (index * PIP_W);
        pipDst.y = PIP_13_Y;

    } else if (isBetween(index, 6, 11)) {
        pipDst.x = PIP_19_X + ((index - 6) * PIP_W);
        pipDst.y = PIP_19_Y;

    } else if (isBetween(index, 12, 17)) {
        pipDst.x = PIP_12_X + ((index - 12) * PIP_W);
        pipDst.y = PIP_12_Y;
    } else if (isBetween(index, 18, 23)) {
        pipDst.x = PIP_6_X + ((index - 18) * PIP_W);
        pipDst.y = PIP_6_Y;
    }
    Sprite* pipSprite;
    pipSprite = Sprite_createEx(texture, pipSrc, pipDst, Z_PIPS);
    pipSprite->useViewport = true;

    int32_t realIndex = (index < 12) ? (index + 13) : (24 - index);
    Sprite_registerClickFn(pipSprite, clickPip, NULL, NULL, realIndex);
    Sage_registerSprite(pipSprite);
}

void createPipSprites(void) {
    SDL_Texture* texture = Sage_loadSVGTexture("assets/empty.svg", 1, 1);
    for (int32_t i = 0; i < 24; i++) {
        createPipSprite(texture, i);
    }
}
