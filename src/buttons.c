/**
 * @file buttons.c
 * @author Michael Baumgarten
 * @brief Game button functions
 */
#include "buttons.h"

void clickButton(void* data) {
    Button* button = (Button*)data;
    ButtonClickEvent* bce = malloc(sizeof(ButtonClickEvent));
    bce->type = button->type;
    SDL_Event e;
    e.type = BUTTON_CLICK_EVENT;
    e.user.data1 = bce;
    SDL_PushEvent(&e);
}

void updateButtonSprite(Sprite* sprite, void* data){
    Button* button = (Button*)data;
    Sprite_setVisible(sprite, !button->isDisabled);
    int32_t y = DICE_Y_OFFSET;
    int32_t x = 0;
    if (button->type == B_Confirm){
        x = (button->turn == P_Light) ? DIE_0_LEFT_X_OFFSET : DIE_0_RIGHT_X_OFFSET;
    } else if (button->type == B_Undo){
        x = (button->turn == P_Light) ? DIE_1_LEFT_X_OFFSET : DIE_1_RIGHT_X_OFFSET;
    } else if (button->type == B_Roll){
        x = (button->turn == P_Light) ? DIE_0_LEFT_X_OFFSET : DIE_0_RIGHT_X_OFFSET;
    }
    Sprite_setLocation(sprite, x, y);
}

void Button_createSprite(Button* button, MediaManager *mm, ViewManager *vm){
    SDL_Rect src;
    int32_t x, y;
    if (button->type == B_Confirm) {
        src = (SDL_Rect){ 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT };
        x = (button->turn == P_Light) ? DIE_0_LEFT_X_OFFSET : DIE_0_RIGHT_X_OFFSET;
        y = DICE_Y_OFFSET;

        VM_createSprite(vm, mm->textures.confirmBtn, src, x, y, Z_BUTTON, false, updateButtonSprite, button, clickButton, button);
    } else if (button->type == B_Undo){
        src = (SDL_Rect){ 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT };
        x = (button->turn == P_Light) ? DIE_1_LEFT_X_OFFSET : DIE_1_RIGHT_X_OFFSET;
        y = DICE_Y_OFFSET;

        VM_createSprite(vm, mm->textures.undoBtn, src, x, y, Z_BUTTON, false, updateButtonSprite, button, clickButton, button);
    } else if (button->type == B_Roll){
        src = (SDL_Rect){ 0, 0, ROLL_BUTTON_WIDTH, BUTTON_HEIGHT };
        x = (button->turn == P_Light) ? DIE_0_LEFT_X_OFFSET : DIE_0_RIGHT_X_OFFSET;
        y = DICE_Y_OFFSET;

        VM_createSprite(vm, mm->textures.rollBtn, src, x, y, Z_BUTTON, false, updateButtonSprite, button, clickButton, button);
    }
}
