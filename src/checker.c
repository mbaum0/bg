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
        int32_t xVelocity = getHorizontalVelocity(velocity, lastX, lastY, destX, destY);
        int32_t yVelocity = getVerticalVelocity(velocity, lastX, lastY, destX, destY);

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

void updateCheckerIndexText(Snippet* snippet, void* data) {
    Checker* checker = (Checker*)data;
    char text[2];
    sprintf(text, "%d", checker->index + 1);
    int32_t textX = GET_CHECKER_X(checker->location) + (CHECKER_SIZE / 2);
    int32_t textY = GET_CHECKER_Y(checker->location, checker->index) + (CHECKER_SIZE / 2);
    Snippet_setLocation(snippet, textX, textY);
    Snippet_setText(snippet, text);
}

void Checker_createSprite(Checker* checker, MediaManager* mm, ViewManager* vm) {
    SDL_Rect src;
    int32_t x, y;
    int32_t colorOffset = (checker->player == P_Light) ? 0 : CHECKER_SIZE;
    src = (SDL_Rect){ colorOffset, 0, CHECKER_SIZE, CHECKER_SIZE };
    x = GET_CHECKER_X(checker->location);
    y = GET_CHECKER_Y(checker->location, checker->index);
    VM_createSprite(vm, mm->textures.checker, src, x, y, Z_CHECKER, false, updateCheckerSprite, checker, clickChecker, checker);

    SDL_Color color;
    if (checker->player == P_Light) {
        color = (SDL_Color){ 0, 0, 0, 255 };
    }
    else {
        color = (SDL_Color){ 255, 255, 255, 255 };
    }
    char text[2];
    sprintf(text, "%d", checker->index + 1);
    int32_t textX = x + CHECKER_SIZE / 2;
    int32_t textY = y + CHECKER_SIZE / 2;
    VM_createSnippet(vm, mm->fonts.standard, color, text, textX, textY, Z_CHECKERTEXT, true, updateCheckerIndexText, checker);
}
