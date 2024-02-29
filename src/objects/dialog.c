/**
 * @file dialog.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief 
 */
#include "util.h"
#include "dialog.h"
#include "sage.h"
#include <SDL3/SDL.h>

void updateDialog(ViewManager* vm, Sprite* sprite, void* object, void* context) {
    (void)vm;
    (void)context;

    Dialog* d = (Dialog*)object;
    Sprite_setVisible(sprite, d->visible);
}

void createDialogSprite(Dialog* d){
    SDL_Texture* texture = Sage_loadTexture("assets/dialog2x.png");
    SDL_FRect src = {0, 0, DIALOG_SRC_W * 2, DIALOG_SRC_H * 2};
    SDL_FRect dst = {DIALOG_X, DIALOG_Y, DIALOG_W, DIALOG_H};

    Sprite* dialogSprite = Sprite_createEx(texture, src, dst, Z_DIALOG);
    Sprite_registerUpdateFn(dialogSprite, updateDialog, d, NULL);
    Sage_registerSprite(dialogSprite);
}
