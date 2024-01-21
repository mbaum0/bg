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

float getCheckerXFromPipLocation(int32_t pipIndex) {
  float checkerX;
  int32_t pipOffset;
  if (isBetween(pipIndex, 1, 6)) {
    pipOffset = (pipIndex - 1) % 6;
    checkerX = CHECKER_RIGHT_OFFSET_NORMAL - (PIP_WIDTH_NORMAL * pipOffset);
  }
  else if (isBetween(pipIndex, 7, 12)) {
    pipOffset = 5 - ((pipIndex - 1) % 6);
    checkerX = CHECKER_LEFT_X_OFFSET_NORMAL + (PIP_WIDTH_NORMAL * pipOffset);
  }
  else if (isBetween(pipIndex, 13, 18)) {
    pipOffset = (pipIndex - 13) % 6;
    checkerX = CHECKER_LEFT_X_OFFSET_NORMAL + (PIP_WIDTH_NORMAL * pipOffset);
  }
  else {
    pipOffset = 5 -((pipIndex - 1) % 6);
    checkerX = CHECKER_RIGHT_OFFSET_NORMAL - (PIP_WIDTH_NORMAL * pipOffset);
  }
  return checkerX;
}

int32_t getCheckersOnPip(int32_t pipIndex, Checkers* checkers){
  int32_t count = 0;
  for (int32_t i = 0; i < 15; i++) {
    if (checkers->lightCheckers[i].pipIndex == pipIndex) {
      count++;
    }
    if (checkers->darkCheckers[i].pipIndex == pipIndex) {
      count++;
    }
  }
  return count;
}

float getCheckerY(Checker* checker) {
  float offset;
  int32_t pipIndex = checker->pipIndex;
  if (isBetween(pipIndex, 1, 12)) {
    offset = CHECKER_BOTTOM_Y_OFFSET_NORMAL;
    offset -= (checker->pipOffset * CHECKER_H_OFFSET_NORMAL);
  }
  else {
    offset = CHECKER_TOP_Y_OFFSET_NORMAL;
    offset += (checker->pipOffset * CHECKER_H_OFFSET_NORMAL);
  }
  return offset;
}

void clickChecker(ViewManager* vm, Sprite* sprite, void* object, void* context) {
  (void)vm;
  (void)sprite;
  Checker* c = (Checker*)object;
  Checkers* checkers = (Checkers*)context;
  int32_t newIndex = c->pipIndex + 1;
  if (newIndex > 24) {
    newIndex = 1;
  }
  c->pipOffset = getCheckersOnPip(newIndex, checkers);
  c->pipIndex = newIndex;
  log_debug("clicked checker. new index: %d, new offset: %d", c->pipIndex, c->pipOffset);
}

void updateChecker(ViewManager* vm, Sprite* sprite, void* object, void* context) {
  (void)vm;
  (void)context;
  Checker* c = (Checker*)object;
  float newX = getCheckerXFromPipLocation(c->pipIndex);
  float newY = getCheckerY(c);
  float x = Sprite_getX(sprite);
  float y = Sprite_getY(sprite);
  if (!isEqual(x, newX, CHECKER_VELOCITY) || !isEqual(y, newY, CHECKER_VELOCITY)) {
    float xVel = getHorizontalVelocity(CHECKER_VELOCITY, x, y, newX, newY);
    float yVel = getVerticalVelocity(CHECKER_VELOCITY, x, y, newX, newY);
    Sprite_setLocation(sprite, x + xVel, y + yVel);
  } else {
    Sprite_setLocation(sprite, newX, newY);
  }
}

// Checker* Checker_create(Sage* sage, int32_t pipIndex, Player player) {
//   (void)sage;
//   Checker* c = malloc(sizeof(Checker));
//   c->pipIndex = pipIndex;
//   c->player = player;
//   c->xVelocity = 0;
//   c->yVelocity = 0;

//   SDL_Texture* texture = NULL;
//   if (player == P_LIGHT) {
//     texture = Sage_loadTexture(sage, "assets/light.png");
//   }
//   else {
//     texture = Sage_loadTexture(sage, "assets/dark.png");
//   }
//   SDL_Rect src_rect = { 0, 0, CHECKER_SRC_SIZE, CHECKER_SRC_SIZE };

//   float checkerX = getCheckerXFromPipLocation(pipIndex);
//   float checkerY = getCheckerYFromPipLocation(pipIndex);

//   SDL_FRect dst_rect = { checkerX, checkerY, CHECKER_W_NORMAL, CHECKER_H_NORMAL };
//   Sprite* s = Sprite_createEx(texture, src_rect, dst_rect, Z_CHECKERS, false, true, true, false);
//   Sprite_registerUpdateFn(s, updateChecker, c, NULL);
//   Sprite_registerClickFn(s, clickChecker, c);
//   Sage_registerSprite(sage, s);
//   return c;
// }

void Checker_destroy(Checker* c) {
  free(c);
}

Checkers* Checkers_create(Sage* sage){
  Checkers* checkers = calloc(1, sizeof(Checkers));
  SDL_Texture* lightTexture = Sage_loadTexture(sage, "assets/light.png");
  SDL_Texture* darkTexture = Sage_loadTexture(sage, "assets/dark.png");
  SDL_Rect src_rect = { 0, 0, CHECKER_SRC_SIZE, CHECKER_SRC_SIZE };

  int32_t lightSetup[] = { 1, 1, 12, 12, 12, 12, 12, 17, 17, 17, 19, 19, 19, 19, 19 };
  int32_t darkSetup[] = { 24, 24, 13, 13, 13, 13, 13, 8, 8, 8, 6, 6, 6, 6, 6 };
  int32_t pipOffsets[] = { 0, 1, 0, 1, 2, 3, 4, 0, 1, 2, 0, 1, 2, 3, 4 };

  for (int32_t i = 0; i < 15; i++){
    checkers->lightCheckers[i].pipIndex = lightSetup[i];
    checkers->lightCheckers[i].player = P_LIGHT;
    checkers->lightCheckers[i].pipOffset = pipOffsets[i];
  }

  for (int32_t i = 0; i < 15; i++){
    checkers->darkCheckers[i].pipIndex = darkSetup[i];
    checkers->darkCheckers[i].player = P_DARK;
    checkers->darkCheckers[i].pipOffset = pipOffsets[i];
  }

  for (int32_t i = 0; i < 30; i++){
    if (i < 15) {
      Checker* c = &checkers->lightCheckers[i];
      c->player = P_LIGHT;
      float x = getCheckerXFromPipLocation(c->pipIndex);
      float y = getCheckerY(c);
      SDL_FRect dst_rect = { x, y, CHECKER_W_NORMAL, CHECKER_H_NORMAL };
      Sprite* s = Sprite_createEx(lightTexture, src_rect, dst_rect, Z_CHECKERS, false, true, true, false);
      Sprite_registerUpdateFn(s, updateChecker, c, checkers);
      Sprite_registerClickFn(s, clickChecker, c, checkers);
      Sage_registerSprite(sage, s);
    }
    else {
      Checker* c = &checkers->darkCheckers[i - 15];
      c->player = P_DARK;
      float x = getCheckerXFromPipLocation(c->pipIndex);
      float y = getCheckerY(c);
      SDL_FRect dst_rect = { x, y, CHECKER_W_NORMAL, CHECKER_H_NORMAL };
      Sprite* s = Sprite_createEx(darkTexture, src_rect, dst_rect, Z_CHECKERS, false, true, true, false);
      Sprite_registerUpdateFn(s, updateChecker, c, checkers);
      Sprite_registerClickFn(s, clickChecker, c, checkers);
      Sage_registerSprite(sage, s);
    }
  }
  return checkers;
}
