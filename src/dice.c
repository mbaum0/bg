/**
 * @file dice.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#include "dice.h"
#include "sage.h"
#include "util.h"
#include "vector.h"

uint32_t DiceClickEventType = 0;

void clickDice(ViewManager* vm, Sprite* sprite, void* object, void* context) {
  (void)vm;
  (void)sprite;
  (void)context;
  (void)object;
  SDL_Event e = { 0 };
  e.type = DiceClickEventType;
  SDL_PushEvent(&e);
}

void updateDice(ViewManager* vm, Sprite* sprite, void* object, void* context) {
  (void)vm;
  (void)context;
  GameDie* die = (GameDie*)object;
  SDL_FRect src = sprite->src_rect;
  src.x = (DICE_W * (die->value - 1));
  Sprite_setSourceRect(sprite, src);
  SDL_FRect dst = sprite->dst_rect;
  int newX, newY;
  newY = dst.y;
  newX = (die->side == 0) ? DICE_LEFT_X : DICE_RIGHT_X;
  if (die->index == 1) { newX += DICE_GAP; }

  if (!isEqual(dst.x, newX, CHECKER_VELOCITY) || !isEqual(dst.y, newY, CHECKER_VELOCITY)){
    float xVel = getHorizontalVelocity(CHECKER_VELOCITY, dst.x, dst.y, newX, newY);
    float yVel = getVerticalVelocity(CHECKER_VELOCITY, dst.x, dst.y, newX, newY);
    Sprite_setLocation(sprite, dst.x + xVel, dst.y + yVel);
  } else {
    Sprite_setLocation(sprite, newX, newY);
  }
}

void createDiceSprites(GameDie* die1, GameDie* die2) {
  SDL_Texture* diceTexture = Sage_loadSVGTexture("assets/dice.svg", DICE_W * 6, DICE_W);
  SDL_FRect src_rect = { 0, 0, DICE_W, DICE_W };
  src_rect.x = (DICE_W * (die1->value - 1));

  int32_t x = (die1->side == 0) ? DICE_LEFT_X : DICE_RIGHT_X;

  SDL_FRect dst_rect = { x, DICE_Y, DICE_W, DICE_W };
  Sprite* s = Sprite_createEx(diceTexture, src_rect, dst_rect, Z_DICE);
  Sprite_registerClickFn(s, clickDice, NULL, NULL);
  Sprite_registerUpdateFn(s, updateDice, die1, NULL);
  Sage_registerSprite(s);

  dst_rect.x += DICE_GAP;
  src_rect.x = (DICE_W * (die2->value - 1));
  s = Sprite_createEx(diceTexture, src_rect, dst_rect, Z_DICE);
  Sprite_registerClickFn(s, clickDice, NULL, NULL);
  Sprite_registerUpdateFn(s, updateDice, die2, NULL);
  Sage_registerSprite(s);
}

