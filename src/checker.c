/**
 * @file checker.c
 * @author Michael Baumgarten
 * @brief Contains checker related functionality
 */
#include "checker.h"

void updateCheckerSprite(Sprite* sprite, void* data) {
    Checker* checker = (Checker*)data;
    uint32_t lastX = Sprite_getX(sprite);
    uint32_t lastY = Sprite_getY(sprite);
    uint32_t destX = GET_CHECKER_X(checker->location);
    uint32_t destY = GET_CHECKER_Y(checker->location, checker->index);
    uint32_t newX = lastX;
    uint32_t newY = lastY;

    // animate checker movement
    uint32_t baseV = 10;
    if (lastX != destX || lastY != destY) {
        // if we're close to the destination, just snap to it
        if (abs(lastX - destX) < baseV && abs(lastY - destY) < baseV) {
            newX = destX;
            newY = destY;
        }
        else {
            float vRatio = (float)(destX - lastX) / (float)(destY - lastY);
            int hRate = baseV * vRatio;
            int vRate = baseV;
            if (lastX < destX) {
                newX = lastX + hRate;
            }
            else if (lastX > destX) {
                newX = lastX - hRate;
            }
            if (lastY < destY) {
                newY = lastY + vRate;
            }
            else if (lastY > destY) {
                newY = lastY - vRate;
            }
        }
    }


    Sprite_setLocation(sprite, newX, newY);
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

void Checker_createSprite(Checker* checker, MediaManager* mm, ViewManager* vm) {
    SDL_Rect src;
    uint32_t x, y;
    uint32_t colorOffset = (checker->player == P_Light) ? 0 : CHECKER_SIZE;
    src = (SDL_Rect){ colorOffset, 0, CHECKER_SIZE, CHECKER_SIZE };
    x = GET_CHECKER_X(checker->location);
    y = GET_CHECKER_Y(checker->location, checker->index);
    VM_createSprite(vm, mm->textures.checker, src, x, y, Z_CHECKER, false, updateCheckerSprite, checker, clickChecker, checker);
}
