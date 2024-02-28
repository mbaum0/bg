/**
 * @file buttons.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#include "buttons.h"
#include "fsm.h"
#include "sage.h"
#include "util.h"

void clickUndo(ViewManager* vm, Sprite* sprite, void* object, void* context, Sint32 code) {
    (void)vm;
    (void)context;
    (void)sprite;
    (void)object;
    (void)code;
    FSMEvent e = {UNDO_MOVE_EVENT, 0, NULL};
    fsm_enqueue_event(e);
}

void clickConfirm(ViewManager* vm, Sprite* sprite, void* object, void* context, Sint32 code) {
    (void)vm;
    (void)context;
    (void)sprite;
    (void)object;
    (void)code;
    FSMEvent e = {CONFIRMED_MOVE_EVENT, 0, NULL};
    fsm_enqueue_event(e);
}

void clickRoll(ViewManager* vm, Sprite* sprite, void* object, void* context, Sint32 code) {
    (void)vm;
    (void)context;
    (void)sprite;
    (void)object;
    (void)code;
    FSMEvent e = {ROLL_DICE_EVENT, 0, NULL};
    fsm_enqueue_event(e);
}

void clickDub(ViewManager* vm, Sprite* sprite, void* object, void* context, Sint32 code) {
    (void)vm;
    (void)context;
    (void)sprite;
    (void)object;
    (void)code;
    //FSMEvent e = {ROLL_DICE_EVENT, 0, NULL};
    //fsm_enqueue_event(e);
}

void updateConfirmBtn(ViewManager* vm, Sprite* sprite, void* object, void* context) {
    (void)vm;
    (void)context;
    GameButton* btn = (GameButton*)object;
    int x, y;
    y = GAME_BTN_Y;
    x = (btn->location == BTN_LEFT) ? GAME_BTN_LEFT_X : GAME_BTN_RIGHT_X;

    Sprite_setVisible(sprite, btn->visible);
    Sprite_setLocation(sprite, x, y);
}

void updateUndoBtn(ViewManager* vm, Sprite* sprite, void* object, void* context) {
    (void)vm;
    (void)context;
    GameButton* btn = (GameButton*)object;
    int x, y;
    y = GAME_BTN_Y;
    x = GAME_BTN_CENTER_X;

    Sprite_setVisible(sprite, btn->visible);
    Sprite_setLocation(sprite, x, y);
}

void updateRollBtn(ViewManager* vm, Sprite* sprite, void* object, void* context) {
    (void)vm;
    (void)context;
    GameButton* btn = (GameButton*)object;
    int x, y;
    y = GAME_BTN_Y;
    x = (btn->location == BTN_LEFT) ? GAME_BTN_LEFT_X : GAME_BTN_RIGHT_X;

    Sprite_setVisible(sprite, btn->visible);
    Sprite_setLocation(sprite, x, y);
}

void updateDubBtn(ViewManager* vm, Sprite* sprite, void* object, void* context) {
    (void)vm;
    (void)context;
    (void)object;
    (void)sprite;
}

void updateNomovesBtn(ViewManager* vm, Sprite* sprite, void* object, void* context) {
    (void)vm;
    (void)context;

    GameButton* btn = (GameButton*)object;
    int x, y;
    y = GAME_BTN_Y;
    x = (btn->location == BTN_LEFT) ? GAME_BTN_LEFT_X : GAME_BTN_RIGHT_X;

    Sprite_setVisible(sprite, btn->visible);
    Sprite_setLocation(sprite, x, y);
}

void createButtonSprites(GameButton* undo, GameButton* confirm, GameButton* roll, GameButton* dub, GameButton* nomoves) {
    // SDL_Texture* btnTexture = Sage_loadSVGTexture("assets/buttons.svg", BTN_W * 2, BTN_W);
    SDL_Texture* btnTexture = Sage_loadTexture("assets/buttons2x.png");

    Sint32 x = (confirm->location == BTN_LEFT) ? GAME_BTN_LEFT_X : GAME_BTN_RIGHT_X;

    // confirm btn
    SDL_FRect src_rect = {GAME_BTN_SRC_OFFSET_CONF*2, 0, GAME_BTN_SRC_W * 2, GAME_BTN_SRC_W * 2};
    SDL_FRect dst_rect = {x, GAME_BTN_Y, GAME_BTN_W, GAME_BTN_W};
    Sprite* s = Sprite_createEx(btnTexture, src_rect, dst_rect, Z_BUTTONS);
    Sprite_setVisible(s, confirm->visible);
    Sprite_registerClickFn(s, clickConfirm, NULL, NULL, 0);
    Sprite_registerUpdateFn(s, updateConfirmBtn, confirm, NULL);
    s->useViewport = true;
    Sage_registerSprite(s);

    // undo btn
    src_rect.x = (GAME_BTN_SRC_OFFSET_UNDO * 2);
    dst_rect.x = GAME_BTN_CENTER_X;
    s = Sprite_createEx(btnTexture, src_rect, dst_rect, Z_BUTTONS);
    Sprite_setVisible(s, undo->visible);
    Sprite_registerClickFn(s, clickUndo, NULL, NULL, 0);
    Sprite_registerUpdateFn(s, updateUndoBtn, undo, NULL);
    s->useViewport = true;
    Sage_registerSprite(s);

    // roll btn
    src_rect.x = (GAME_BTN_SRC_OFFSET_ROLL * 2);
    dst_rect.x = x;
    s = Sprite_createEx(btnTexture, src_rect, dst_rect, Z_BUTTONS);
    Sprite_setVisible(s, roll->visible);
    Sprite_registerClickFn(s, clickRoll, NULL, NULL, 0);
    Sprite_registerUpdateFn(s, updateRollBtn, roll, NULL);
    s->useViewport = true;
    Sage_registerSprite(s);

    // dub btn
    src_rect.x = (GAME_BTN_SRC_OFFSET_DUB * 2);
    dst_rect.x = GAME_BTN_DUB_X;
    dst_rect.y = GAME_BTN_DUB_Y;
    s = Sprite_createEx(btnTexture, src_rect, dst_rect, Z_BUTTONS);
    Sprite_setVisible(s, dub->visible);
    Sprite_registerClickFn(s, clickDub, NULL, NULL, 0);
    Sprite_registerUpdateFn(s, updateDubBtn, dub, NULL);
    s->useViewport = true;
    Sage_registerSprite(s);

    // no moves btn
    src_rect.x = (GAME_BTN_SRC_OFFSET_NOMOVES * 2);
    dst_rect.x = x;
    s = Sprite_createEx(btnTexture, src_rect, dst_rect, Z_BUTTONS);
    Sprite_setVisible(s, nomoves->visible);
    Sprite_registerUpdateFn(s, updateNomovesBtn, nomoves, NULL);
    s->useViewport = true;
    Sage_registerSprite(s);
}
