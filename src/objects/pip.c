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

extern Sage sage;

void clickPip(ViewManager* vm, Sprite* sprite, void* object, void* context, Sint32 code) {
    (void)vm;
    (void)sprite;
    (void)context;
    (void)object;
    FSMEvent e = {PIP_CLICKED_EVENT, code, NULL};
    fsm_enqueue_event(e);
}

void updatePip(ViewManager* vm, Sprite* sprite, void* object, void* context) {
    (void)vm;
    (void)context;
    Pip* p = (Pip*)object;
    SDL_FRect srcRect = sprite->src_rect;
    if (p->index < 12) {
        srcRect.x = (p->color == PIP_BLUE) ? 0 : PIP_SRC_W * 4;
    } else {
        srcRect.x = (p->color == PIP_BLUE) ? PIP_SRC_W * 2 : PIP_SRC_W * 6;
    }
    Sprite_setSourceRect(sprite, srcRect);
    Sprite_setAlpha(sprite, p->alpha);
}

void registerPipSprite(SDL_Texture* texture, Pip* pip) {
    SDL_FRect pipSrc = {0, 0, PIP_SRC_W * 2, PIP_SRC_H * 2};
    SDL_FRect pipDst = {0, 0, PIP_W, PIP_H};
    Sint32 offsetIndex;

    if (isBetween(pip->index, 1, 6)) {
        offsetIndex = 6 - pip->index;
        pipSrc.x = 0;
        pipDst.x = PIP_6_X + (offsetIndex * PIP_W);
        pipDst.y = PIP_6_Y;

    } else if (isBetween(pip->index, 7, 12)) {
        offsetIndex = (5 - (pip->index - 7));
        pipSrc.x = 0;
        pipDst.x = PIP_12_X + (offsetIndex * PIP_W);
        pipDst.y = PIP_12_Y;

    } else if (isBetween(pip->index, 13, 18)) {
        offsetIndex = (pip->index - 13);
        pipSrc.x = PIP_SRC_W * 2;
        pipDst.x = PIP_13_X + (offsetIndex * PIP_W);
        pipDst.y = PIP_13_Y;

    } else if (isBetween(pip->index, 19, 24)) {
        offsetIndex = (pip->index - 19);
        pipSrc.x = PIP_SRC_W * 2;
        pipDst.x = PIP_19_X + (offsetIndex * PIP_W);
        pipDst.y = PIP_19_Y;
    }
    Sprite* pipSprite;
    pipSprite = Sprite_createEx(texture, pipSrc, pipDst, Z_PIPS);
    pipSprite->useViewport = true;

    Sprite_registerClickFn(pipSprite, clickPip, pip, NULL, pip->index);
    Sprite_registerUpdateFn(pipSprite, updatePip, pip, NULL);
    Sage_registerSprite(pipSprite);
}

void createPipSprite(Pip* pip) {
    SDL_Texture* texture = Sage_loadTexture("assets/piphl2x.png");
    registerPipSprite(texture, pip);
}
