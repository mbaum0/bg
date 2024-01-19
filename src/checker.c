/**
 * @file checker.c
 * @author Michael Baumgarten
 * @brief
 */
#include "checker.h"
#include "board.h"
#include "sprite.h"
#include "util.h"

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
    pipOffset = (pipIndex - 1) % 6;
    checkerX = CHECKER_RIGHT_OFFSET_NORMAL - (PIP_WIDTH_NORMAL * pipOffset);
  }
  return checkerX;
}

float getCheckerYFromPipLocation(int32_t pipIndex) {
  if (isBetween(pipIndex, 1, 12)) {
    return CHECKER_BOTTOM_Y_OFFSET_NORMAL;
  }
  else {
    return CHECKER_TOP_Y_OFFSET_NORMAL;
  }
}

void clickChecker(ViewManager* vm, Sprite* sprite, void* data) {
  (void)vm;
  (void)sprite;
  Checker* c = (Checker*)data;
  int32_t newIndex = c->pipIndex + 1;
  if (newIndex > 24) {
    newIndex = 1;
  }
  c->pipIndex = newIndex;
  log_debug("Clicked checker %d", c->pipIndex);
}

void updateChecker(ViewManager* vm, Sprite* sprite, void* data) {
  (void)vm;
  Checker* c = (Checker*)data;
  float x = getCheckerXFromPipLocation(c->pipIndex);
  float y = getCheckerYFromPipLocation(c->pipIndex);
  Sprite_setLocation(sprite, x, y);
}

Checker* Checker_create(Sage* sage, int32_t pipIndex, Player player) {
  (void)sage;
  Checker* c = malloc(sizeof(Checker));
  c->pipIndex = pipIndex;
  c->player = player;

  SDL_Texture* texture = NULL;
  if (player == P_LIGHT) {
    texture = Sage_loadTexture(sage, "assets/light.png");
  }
  else {
    texture = Sage_loadTexture(sage, "assets/dark.png");
  }
  SDL_Rect src_rect = { 0, 0, CHECKER_SRC_SIZE, CHECKER_SRC_SIZE };

  float checkerX = getCheckerXFromPipLocation(pipIndex);
  float checkerY = getCheckerYFromPipLocation(pipIndex);

  SDL_FRect dst_rect = { checkerX, checkerY, CHECKER_W_NORMAL, CHECKER_H_NORMAL };
  Sprite* s = Sprite_createEx(texture, src_rect, dst_rect, Z_CHECKERS, false, true, true, false);
  Sprite_registerUpdateFn(s, updateChecker, c);
  Sprite_registerClickFn(s, clickChecker, c);
  Sage_registerSprite(sage, s);
  return c;
}

void Checker_destroy(Checker* c) {
  free(c);
}
