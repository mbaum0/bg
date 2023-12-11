/**
 * @file dice.c
 * @author Michael Baumgarten
 * @brief Contains dice related functionality
 */
#include "dice.h"

void clickDie(void* data) {
    Die* die = (Die*)data;
    DieClickEvent* dce = malloc(sizeof(DieClickEvent));
    dce->value = die->value;
    SDL_Event e;
    e.type = DIE_CLICK_EVENT;
    e.user.data1 = dce;
    SDL_PushEvent(&e);
}

void updateDieSprite(Sprite* sprite, void* data) {
    Die* die = (Die*)data;
    int32_t faceOffset = (die->value - 1);
    if (die->rollCount > 0) {
        die->rollCount--;

        if (die->rollCount % 3 == 0) {
            faceOffset = rand() % 6;
            Sprite_setSourceRect(sprite, (SDL_Rect) { (faceOffset)*DICE_SIZE, 0, DICE_SIZE, DICE_SIZE });
        }
    }
    else {
        Sprite_setSourceRect(sprite, (SDL_Rect) { (faceOffset)*DICE_SIZE, 0, DICE_SIZE, DICE_SIZE });
    }

    int32_t x = (die->turn == P_Light) ? \
        (die->side == D_Left) ? DIE_0_LEFT_X_OFFSET : DIE_1_LEFT_X_OFFSET : \
        (die->side == D_Left) ? DIE_0_RIGHT_X_OFFSET : DIE_1_RIGHT_X_OFFSET;
    int32_t y = DICE_Y_OFFSET;
    Sprite_setLocation(sprite, x, y);

    Sprite_setVisible(sprite, !die->isDisabled);
}

void Dice_createSprite(Die* die, MediaManager* mm, ViewManager* vm) {
    SDL_Rect src;
    int32_t x, y;
    int32_t faceOffset = (die->value - 1) * DICE_SIZE;
    src = (SDL_Rect){ faceOffset, 0, DICE_SIZE, DICE_SIZE };
    if (die->turn == P_Light) {
        if (die->side == D_Left) {
            x = DIE_0_LEFT_X_OFFSET;
        }
        else {
            x = DIE_1_LEFT_X_OFFSET;
        }
    }
    else {
        if (die->side == D_Left) {
            x = DIE_0_RIGHT_X_OFFSET;
        }
        else {
            x = DIE_1_RIGHT_X_OFFSET;
        }
    }
    y = DICE_Y_OFFSET;

    VM_createSprite(vm, mm->textures.dice, src, x, y, Z_DICE, false, updateDieSprite, die, clickDie, die);
}

void Dice_roll(Die* die) {
    die->value = rand() % 6 + 1;
    die->rollCount = 30;

}
