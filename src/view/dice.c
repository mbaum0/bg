/**
 * @file dice.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#include "dice.h"
#include "fsm.h"
#include "sage.h"
#include "util.h"
#include "vector.h"

void clickDice(ViewManager* vm, Sprite* sprite, void* object, void* context, Sint32 code) {
    (void)vm;
    (void)sprite;
    (void)context;
    (void)object;
    (void)code;
    FSMEvent e = {DICE_CLICKED_EVENT, 0, NULL};
    fsm_enqueue_event(e);
}

void updateDice(ViewManager* vm, Sprite* sprite, void* object, void* context) {
    (void)vm;
    (void)context;
    GameDie* die = (GameDie*)object;
    SDL_FRect src = sprite->src_rect;
    src.x = (DICE_SRC_W * (die->value - 1) * 2);
    Sprite_setSourceRect(sprite, src);
    SDL_FRect dst = sprite->dst_rect;
    int newX, newY;
    newY = DICE_Y;
    newX = (die->side == 0) ? DICE_LEFT_X : DICE_RIGHT_X;
    if (die->index == 1) {
        newX += DICE_GAP;
    }

    if (die->uses == 1) {
        Sprite_setAlpha(sprite, 128);
    } else if (die->uses == 2) {
        Sprite_setAlpha(sprite, 64);
    } else {
        Sprite_setAlpha(sprite, 255);
    }

    if (!isEqual(dst.x, newX, DICE_VELOCITY) || !isEqual(dst.y, newY, DICE_VELOCITY)) {
        if (die->animation == DICE_SWAP) {
            float nextX, nextY;
            float radius = (DICE_GAP / 2);
            float sideX = (die->side == 0) ? DICE_LEFT_X : DICE_RIGHT_X;
            float centerX = sideX + (DICE_GAP / 2);
            float centerY = DICE_Y;
            getNextCoordinatesCircle(radius, DICE_VELOCITY, centerX, centerY, dst.x, dst.y, &nextX, &nextY);
            Sprite_setLocation(sprite, nextX, nextY);

        } else if (die->animation == DICE_MOVE) {
            die->moving = true;
            float xVel = getHorizontalVelocity(CHECKER_VELOCITY, dst.x, dst.y, newX, newY);
            float yVel = getVerticalVelocity(CHECKER_VELOCITY, dst.x, dst.y, newX, newY);
            Sprite_setLocation(sprite, dst.x + xVel, dst.y + yVel);
        }
    } else {
        if (die->moving) {
            die->moving = false;
        }
        Sprite_setLocation(sprite, newX, newY);
    }
}

void createDiceSprites(GameDie* die1, GameDie* die2) {
    SDL_Texture* diceTexture = Sage_loadTexture("assets/dice2x.png");
    SDL_FRect src_rect = {0, 0, DICE_SRC_W * 2, DICE_SRC_W * 2};
    src_rect.x = (DICE_SRC_W * (die1->value - 1) * 2);

    Sint32 x = (die1->side == 0) ? DICE_LEFT_X : DICE_RIGHT_X;

    SDL_FRect dst_rect = {x, DICE_Y, DICE_W, DICE_W};
    Sprite* s = Sprite_createEx(diceTexture, src_rect, dst_rect, Z_DICE);
    Sprite_registerClickFn(s, clickDice, NULL, NULL, 0);
    Sprite_registerUpdateFn(s, updateDice, die1, NULL);
    s->useViewport = true;
    Sage_registerSprite(s);

    dst_rect.x += DICE_GAP;
    src_rect.x = (DICE_SRC_W * (die2->value - 1) * 2);
    s = Sprite_createEx(diceTexture, src_rect, dst_rect, Z_DICE);
    Sprite_registerClickFn(s, clickDice, NULL, NULL, 0);
    Sprite_registerUpdateFn(s, updateDice, die2, NULL);
    s->useViewport = true;
    Sage_registerSprite(s);
}