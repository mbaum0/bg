/**
 * @file checker.c
 * @author Michael Baumgarten
 * @brief Contains checker related functionality
 */
#include "checker.h"

void updateCheckerSprite(Sprite* sprite, void* data) {
    Checker* checker = (Checker*)data;
    int32_t lastX = Sprite_getX(sprite);
    int32_t lastY = Sprite_getY(sprite);
    int32_t destX = GET_CHECKER_X(checker->location);
    int32_t destY = GET_CHECKER_Y(checker->location, checker->index);
    int32_t newX = lastX;
    int32_t newY = lastY;

    // animate checker movement
    int32_t baseV = 20;
    if (lastX != destX || lastY != destY) {
        // if we're close to the destination, just snap to it
        if (abs(lastX - destX) < baseV && abs(lastY - destY) < baseV) {
            newX = destX;
            newY = destY;
        }
        else {
            if (lastY == destY) {
                // if Y's are the same, just move horizontally
                if (lastX < destX) {
                    newX = lastX + baseV;
                }
                else if (lastX > destX) {
                    newX = lastX - baseV;
                }
            }
            else if (lastX == destX) {
                // if X's are the same, just move vertically
                if (lastY < destY) {
                    newY = lastY + baseV;
                }
                else if (lastY > destY) {
                    newY = lastY - baseV;
                }
            }
            else {
                float vRatio = (float)(destX - lastX) / (float)(destY - lastY);
                int hRate = fabsf(baseV * vRatio);
                int vRate = fabsf(baseV * (1/vRatio));
                if (lastX > destX) {
                    hRate *= -1;
                }
                if (lastY > destY) {
                    vRate *= -1;
                }
                newX = lastX + hRate;
                newY = lastY + vRate;
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
    int32_t x, y;
    int32_t colorOffset = (checker->player == P_Light) ? 0 : CHECKER_SIZE;
    src = (SDL_Rect){ colorOffset, 0, CHECKER_SIZE, CHECKER_SIZE };
    x = GET_CHECKER_X(checker->location);
    y = GET_CHECKER_Y(checker->location, checker->index);
    VM_createSprite(vm, mm->textures.checker, src, x, y, Z_CHECKER, false, updateCheckerSprite, checker, clickChecker, checker);
}
