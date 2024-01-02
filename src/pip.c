/**
 * @file pip.c
 * @author Michael Baumgarten
 * @brief Contains pip related functionality
 */
#include "pip.h"

void clickPip(void* data) {
    int32_t pipIdx = (intptr_t)data;
    LocationClickEvent* lce = malloc(sizeof(LocationClickEvent));
    lce->location = pipIdx;
    SDL_Event e;
    e.type = LOCATION_CLICK_EVENT;
    e.user.data1 = lce;
    SDL_PushEvent(&e);
}

void Pip_createSprite(int32_t pip, MediaManager* mm, ViewManager* vm) {
    SDL_Rect src;
    int32_t x, y;
    bool flip = (pip < 12) ? true : false;
    int32_t color = (pip % 2 == 0) ? 0 : PIP_WIDTH;
    src = (SDL_Rect){ color, 0, PIP_WIDTH, PIP_HEIGHT };
    x = GET_PIP_X(pip);
    y = GET_PIP_Y(pip);
    VM_createSprite(vm, mm->textures.pip, src, x, y, Z_PIP, flip, NULL, NULL, clickPip, (void*)(intptr_t)(pip + 1));
}