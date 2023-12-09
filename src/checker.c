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

    if (lastX == destX && lastY == destY) {
        return;
    }

    int32_t velocity = 30;

    // if the checker is close enough, just set it to the destination
    if (abs(destX - lastX) < velocity && abs(destY - lastY) < velocity) {
        newX = destX;
        newY = destY;
    }
    else {
        float hypotenuse = sqrt(pow(destX - lastX, 2) + pow(destY - lastY, 2));
        float sinOrigin = abs(destX - lastX) / hypotenuse;
        float cosOrigin = abs(destY - lastY) / hypotenuse;
        int32_t xVelocity = sinOrigin * velocity;
        int32_t yVelocity = cosOrigin * velocity;

        xVelocity *= (lastX < destX) ? 1 : -1;
        yVelocity *= (lastY < destY) ? 1 : -1;

        newX += xVelocity;
        newY += yVelocity;
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
