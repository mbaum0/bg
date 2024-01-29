/**
 * @file checker.c
 * @author Michael Baumgarten
 * @brief
 */
#include "checker.h"
#include "board.h"
#include "sprite.h"
#include "util.h"
#include "vector.h"
#include "dstats.h"

uint32_t CheckerClickEventType = 0;

int32_t getCheckerX(Checker* checker) {
  int32_t pipIndex = checker->pipIndex;
  int32_t pipOffset;
  int32_t checkerX;

  if (pipIndex == 0){
    if (checker->color == LIGHT){
      return CHECKER_TOP_HOME_X + (checker->pipYOffset * CHECKER_W);
    } else {
      return CHECKER_BOT_HOME_X + (checker->pipYOffset * CHECKER_W);
    }
  }

  if (isBetween(pipIndex, 1, 6)) {
    pipOffset = (6 - pipIndex);
    checkerX = CHECKER_PIP_6_X + (PIP_W * pipOffset);
  }
  else if (isBetween(pipIndex, 7, 12)) {
    pipOffset = (6 - pipIndex) + 6;
    checkerX = CHECKER_PIP_12_X + (PIP_W * pipOffset);
  }
  else if (isBetween(pipIndex, 13, 18)) {
    pipOffset = (pipIndex - 13);
    checkerX = CHECKER_PIP_13_X + (PIP_W * pipOffset);
  }
  else {
    pipOffset = (pipIndex - 19);
    checkerX = CHECKER_PIP_19_X + (PIP_W * pipOffset);
  }
  return checkerX;
}

int32_t getCheckerY(Checker* checker) {
  int32_t offset;
  int32_t pipIndex = checker->pipIndex;

  if (pipIndex == 0){
    if (checker->color == LIGHT){
      return CHECKER_TOP_HOME_Y;
    } else {
      return CHECKER_BOT_HOME_Y;
    }
  }
  if (isBetween(pipIndex, 1, 12)) {
    offset = CHECKER_PIP_12_Y;
    offset -= (checker->pipYOffset * CHECKER_W);
  }
  else {
    offset = CHECKER_PIP_13_Y;
    offset += (checker->pipYOffset * CHECKER_W);
  }
  return offset;
}

void clickChecker(ViewManager* vm, Sprite* sprite, void* object, void* context) {
  (void)vm;
  (void)sprite;
  (void)context;
  Checker* checker = (Checker*)object;
  SDL_Event e = { 0 };
  e.type = CheckerClickEventType;
  e.user.data1 = checker;
  SDL_PushEvent(&e);
}

void updateChecker(ViewManager* vm, Sprite* sprite, void* object, void* context) {
  (void)vm;
  (void)context;
  Checker* c = (Checker*)object;
  float newX = getCheckerX(c);
  float newY = getCheckerY(c);
  float x = Sprite_getX(sprite);
  float y = Sprite_getY(sprite);
  if (!isEqual(x, newX, CHECKER_VELOCITY) || !isEqual(y, newY, CHECKER_VELOCITY)) {
    float xVel = getHorizontalVelocity(CHECKER_VELOCITY, x, y, newX, newY);
    float yVel = getVerticalVelocity(CHECKER_VELOCITY, x, y, newX, newY);
    Sprite_setLocation(sprite, x + xVel, y + yVel);
  }
  else {
    Sprite_setLocation(sprite, newX, newY);
  }
}

void createCheckerSprite(Checker* c) {
  SDL_Texture* texture = Sage_loadSVGTexture("assets/checkers.svg", CHECKER_W * 2, CHECKER_W);
  SDL_FRect s_lightRect = { 0, 0, CHECKER_W, CHECKER_W };
  SDL_FRect s_darkRect = { CHECKER_W, 0, CHECKER_W, CHECKER_W };

  int32_t x = getCheckerX(c);
  int32_t y = getCheckerY(c);

  SDL_FRect dst = { x, y, CHECKER_W, CHECKER_W };
  Sprite* s;
  if (c->color == LIGHT) {
    s = Sprite_createEx(texture, s_lightRect, dst, Z_CHECKERS);
  }
  else {
    s = Sprite_createEx(texture, s_darkRect, dst, Z_CHECKERS);
  }
  s->useViewport = true;

  Sprite_registerUpdateFn(s, updateChecker, c, NULL);
  Sprite_registerClickFn(s, clickChecker, c, NULL);
  Sage_registerSprite(s);
}
