/**
 * @file dialog.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#include "dialog.h"
#include "fsm.h"
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

    float yDst = (d->visible ? DIALOG_Y : DIALOG_START_Y);

    if (!isEqual(y, yDst, CHECKER_VELOCITY)) {
        float yVel = getVerticalVelocity(CHECKER_VELOCITY, x, y, x, yDst);
        Sprite_setLocation(sprite, x, y + yVel);
        d->x = x;
        d->y = y + yVel;
        d->moving = true;
    } else {
        Sprite_setLocation(sprite, x, yDst);
        d->x = x;
        d->y = yDst;
        d->moving = false;
    }
}

void clickNextRound(ViewManager* vm, Sprite* sprite, void* object, void* context, Sint32 code) {
    (void)code;
    (void)vm;
    (void)context;
    (void)sprite;
    (void)object;
    FSMEvent e = {NEXT_ROUND_BUTTON_CLICKED_EVENT, 0, NULL};
    fsm_enqueue_event(e);
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

void updateDialogBtnNewGame(ViewManager* vm, Sprite* sprite, void* object, void* context) {
    (void)vm;
    (void)context;
    Dialog* d = (Dialog*)object;
    Sprite_setLocation(sprite, d->x + DIALOG_BTN_LEFT_X, d->y + DIALOG_BTN_Y);
    Sprite_setVisible(sprite, d->visible);
}

void updateDialogBtnNextRound(ViewManager* vm, Sprite* sprite, void* object, void* context) {
    (void)vm;
    (void)context;
    Dialog* d = (Dialog*)object;
    Sprite_setLocation(sprite, d->x + DIALOG_BTN_RIGHT_X, d->y + DIALOG_BTN_Y);
    Sprite_setVisible(sprite, d->visible);
}

void titleUpdate(ViewManager* vm, Snippet* snippet, void* data) {
    (void)vm;
    (void)data;
    Dialog* d = (Dialog*)data;

    char title[30];
    if (d->playerWon) {
        sprintf(title, "You Won");
    } else {
        sprintf(title, "You Lost");
    }
    Snippet_setLocation(snippet, d->x + DIALOG_TITLE_X, d->y + DIALOG_TITLE_Y);
    Snippet_setVisible(snippet, d->visible);
    Snippet_setText(snippet, title);
}

void matchToUpdate(ViewManager* vm, Snippet* snippet, void* data) {
    (void)vm;
    (void)data;
    Dialog* d = (Dialog*)data;

    char matchTo[30];
    sprintf(matchTo, "Match to %d points", d->maxGames);
    Snippet_setLocation(snippet, d->x + DIALOG_MATCHTO_X, d->y + DIALOG_MATCHTO_Y);
    Snippet_setVisible(snippet, d->visible);
    Snippet_setText(snippet, matchTo);
}

void wonUpdate(ViewManager* vm, Snippet* snippet, void* data) {
    (void)vm;
    (void)data;
    Dialog* d = (Dialog*)data;

    char won[50];
    sprintf(won, "Games Won:\t%d", d->roundsWon);
    Snippet_setLocation(snippet, d->x + DIALOG_WON_X, d->y + DIALOG_WON_Y);
    Snippet_setVisible(snippet, d->visible);
    Snippet_setText(snippet, won);
}

void lostUpdate(ViewManager* vm, Snippet* snippet, void* data) {
    (void)vm;
    (void)data;
    Dialog* d = (Dialog*)data;

    char lost[50];
    sprintf(lost, "Games Lost:\t%d", d->roundsLost);
    Snippet_setLocation(snippet, d->x + DIALOG_LOST_X, d->y + DIALOG_LOST_Y);
    Snippet_setVisible(snippet, d->visible);
    Snippet_setText(snippet, lost);
}

void createDialogText(Dialog* d) {
    char title[30];
    char matchTo[30];
    char won[50];
    char lost[50];

    sprintf(title, "");
    sprintf(matchTo, "");
    sprintf(won, "");
    sprintf(lost, "");

    SageFont* sf = Sage_loadBitmapFont("assets/NotoSans-Medium_0.png", "assets/NotoSans-Medium.fnt",
                                       SCORE_FONT_SRC_SIZE, SCORE_FONT_SIZE);

    SDL_Color fontColor = {255, 255, 255, 255};
    Snippet* titleSnippet = Snippet_create(sf, fontColor, DIALOG_TITLE_X, DIALOG_TITLE_Y, Z_DEBUG, false);
    Snippet* matchToSnippet = Snippet_create(sf, fontColor, DIALOG_MATCHTO_X, DIALOG_MATCHTO_Y, Z_DEBUG, false);
    Snippet* wonSnippet = Snippet_create(sf, fontColor, DIALOG_WON_X, DIALOG_WON_Y, Z_DEBUG, false);
    Snippet* lostSnippet = Snippet_create(sf, fontColor, DIALOG_LOST_X, DIALOG_LOST_Y, Z_DEBUG, false);

    Sage_registerSnippet(titleSnippet);
    Sage_registerSnippet(matchToSnippet);
    Sage_registerSnippet(wonSnippet);
    Sage_registerSnippet(lostSnippet);

    Snippet_setText(titleSnippet, title);
    Snippet_setText(matchToSnippet, matchTo);
    Snippet_setText(wonSnippet, won);
    Snippet_setText(lostSnippet, lost);

    Snippet_registerUpdateFn(titleSnippet, titleUpdate, d);
    Snippet_registerUpdateFn(matchToSnippet, matchToUpdate, d);
    Snippet_registerUpdateFn(wonSnippet, wonUpdate, d);
    Snippet_registerUpdateFn(lostSnippet, lostUpdate, d);
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

    SDL_Texture* buttonTexture = Sage_loadTexture("assets/dialogbtns2x.png");
    src = (SDL_FRect){0, 0, DIALOG_BTN_SRC_W * 2, DIALOG_BTN_SRC_H * 2};
    dst = (SDL_FRect){DIALOG_BTN_LEFT_X, DIALOG_BTN_Y, DIALOG_BTN_W, DIALOG_BTN_H};
    Sprite* ngBtnSprite = Sprite_createEx(buttonTexture, src, dst, Z_DIALOG + 1);
    Sprite_registerUpdateFn(ngBtnSprite, updateDialogBtnNewGame, d, NULL);
    Sage_registerSprite(ngBtnSprite);

    src = (SDL_FRect){DIALOG_BTN_SRC_W * 2, 0, DIALOG_BTN_SRC_W * 2, DIALOG_BTN_SRC_H * 2};
    dst = (SDL_FRect){DIALOG_BTN_LEFT_X, DIALOG_BTN_Y, DIALOG_BTN_W, DIALOG_BTN_H};
    Sprite* nmBtnSprite = Sprite_createEx(buttonTexture, src, dst, Z_DIALOG + 1);
    Sprite_registerUpdateFn(nmBtnSprite, updateDialogBtnNextRound, d, NULL);
    Sprite_registerClickFn(nmBtnSprite, clickNextRound, NULL, NULL, 0);
    Sage_registerSprite(nmBtnSprite);

    createDialogText(d);
}
