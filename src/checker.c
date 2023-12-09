/**
 * @file checker.c
 * @author Michael Baumgarten
 * @brief Contains checker related functionality
 */
#include "checker.h"

void updateCheckerSprite(Sprite* sprite, void* data) {
    Checker* checker = (Checker*)data;
    uint32_t x = GET_CHECKER_X(checker->location);
    uint32_t y = GET_CHECKER_Y(checker->location, checker->index);
    Sprite_setLocation(sprite, x, y);
}

void clickChecker(void* data) {
    Checker* checker = (Checker*)data;
    LocationClickEvent* lce = malloc(sizeof(LocationClickEvent));
    lce->location = checker->location;
    SDL_Event e;
    e.type = LOCATION_CLICK_EVENT;
    e.user.data1 = lce;
    SDL_PushEvent(&e);
}

void Checker_createSprite(Checker* checker, MediaManager* mm, ViewManager* vm){
    SDL_Rect src;
    uint32_t x, y;
    uint32_t colorOffset = (checker->player == P_Light) ? 0 : CHECKER_SIZE;
    src = (SDL_Rect){ colorOffset, 0, CHECKER_SIZE, CHECKER_SIZE };
    x = GET_CHECKER_X(checker->location);
    y = GET_CHECKER_Y(checker->location, checker->index);
    VM_createSprite(vm, mm->textures.checker, src, x, y, Z_CHECKER, false, updateCheckerSprite, checker, clickChecker, checker);
}
