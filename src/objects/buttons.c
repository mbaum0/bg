/**
 * @file buttons.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#include "buttons.h"
#include "fsm.h"
#include "sage.h"
#include "util.h"

void clickUndo(ViewManager* vm, Sprite* sprite, void* object, void* context) {
    (void)vm;
    (void)context;
    (void)sprite;
    (void)object;
    FSMEvent e = {UNDO_MOVE_EVENT, NULL};
    fsm_enqueue_event(e);
}

void clickConfirm(ViewManager* vm, Sprite* sprite, void* object, void* context) {
    (void)vm;
    (void)context;
    (void)sprite;
    (void)object;
    FSMEvent e = {CONFIRMED_MOVE_EVENT, NULL};
    fsm_enqueue_event(e);
}

void updateConfirmBtn(ViewManager* vm, Sprite* sprite, void* object, void* context) {
    (void)vm;
    (void)context;
    GameButton* btn = (GameButton*)object;
    int x, y;
    y = BTN_Y;
    x = (btn->side == 0) ? BTN_LEFT_X : BTN_RIGHT_X;
    x += BTN_GAP; // confirm btn will be on the right side

    Sprite_setVisible(sprite, btn->visible);
    Sprite_setLocation(sprite, x, y);
}

void updateUndoBtn(ViewManager* vm, Sprite* sprite, void* object, void* context) {
    (void)vm;
    (void)context;
    GameButton* btn = (GameButton*)object;
    int x, y;
    y = BTN_Y;
    x = (btn->side == 0) ? BTN_LEFT_X : BTN_RIGHT_X;

    Sprite_setVisible(sprite, btn->visible);
    Sprite_setLocation(sprite, x, y);
}

void createButtonSprites(GameButton* undo, GameButton* confirm) {
    SDL_Texture* btnTexture = Sage_loadSVGTexture("assets/buttons.svg", BTN_W * 2, BTN_W);

    int32_t x = (confirm->side == 0) ? BTN_LEFT_X : BTN_RIGHT_X;

    // confirm btn
    SDL_FRect src_rect = {0, 0, BTN_W, BTN_W};
    SDL_FRect dst_rect = {x + BTN_GAP, BTN_Y, BTN_W, BTN_W};
    Sprite* s = Sprite_createEx(btnTexture, src_rect, dst_rect, Z_BUTTONS);
    Sprite_setVisible(s, confirm->visible);
    Sprite_registerClickFn(s, clickConfirm, NULL, NULL);
    Sprite_registerUpdateFn(s, updateConfirmBtn, confirm, NULL);
    Sage_registerSprite(s);

    // undo btn
    src_rect.x = (BTN_W);
    dst_rect.x = x;
    s = Sprite_createEx(btnTexture, src_rect, dst_rect, Z_BUTTONS);
    Sprite_setVisible(s, undo->visible);
    Sprite_registerClickFn(s, clickUndo, NULL, NULL);
    Sprite_registerUpdateFn(s, updateUndoBtn, undo, NULL);
    Sage_registerSprite(s);
}
