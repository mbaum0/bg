/**
 * @file dialog.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#include "dialog.h"
#include "sage.h"
#include "util.h"
#include "vector.h"
#include <SDL3/SDL.h>

void updateDialog(ViewManager* vm, Sprite* sprite, void* object, void* context) {
    (void)vm;
    (void)context;

    Dialog* d = (Dialog*)object;
    Sprite_setVisible(sprite, d->visible);

    float y = Sprite_getY(sprite);
    float x = Sprite_getX(sprite);

    float yDst = (sprite->visible ? DIALOG_Y : DIALOG_START_Y);

    if (!isEqual(y, yDst, CHECKER_VELOCITY)) {
        float yVel = getVerticalVelocity(CHECKER_VELOCITY, x, y, x, yDst);
        Sprite_setLocation(sprite, x, y + yVel);
    } else {
        Sprite_setLocation(sprite, x, yDst);
    }
}

void updateOverlay(ViewManager* vm, Sprite* sprite, void* object, void* context) {
    (void)vm;
    (void)context;
    Dialog* d = (Dialog*)object;
    Sprite_setVisible(sprite, d->visible);
    Sprite_setAlpha(sprite, d->alpha);

    if (d->alpha < 255) {
        d->alpha += 5;
    } else if (d->alpha > 255) {
        d->alpha = 255;
    }
}

void createDialogSprite(Dialog* d) {
    SDL_Texture* texture = Sage_loadTexture("assets/dialog2x.png");
    SDL_FRect src = {0, 0, DIALOG_SRC_W * 2, DIALOG_SRC_H * 2};
    SDL_FRect dst = {DIALOG_X, DIALOG_Y, DIALOG_W, DIALOG_H};

    Sprite* dialogSprite = Sprite_createEx(texture, src, dst, Z_DIALOG);
    Sprite_registerUpdateFn(dialogSprite, updateDialog, d, NULL);
    Sage_registerSprite(dialogSprite);

    SDL_Texture* overlayTexture = Sage_loadTexture("assets/overlay_shade2x.png");
    src = (SDL_FRect){0, 0, OVERLAY_SHADE_SRC_W * 2, OVERLAY_SHADE_SRC_H * 2};
    dst = (SDL_FRect){OVERLAY_SHADE_X, OVERLAY_SHADE_Y, OVERLAY_SHADE_W, OVERLAY_SHADE_H};
    Sprite* overlaySprite = Sprite_createEx(overlayTexture, src, dst, Z_DIALOG_OVERLAY);
    Sprite_registerUpdateFn(overlaySprite, updateOverlay, d, NULL);
    Sage_registerSprite(overlaySprite);
}
