/**
 * @file checker.c
 * @author Michael Baumgarten
 * @brief
 */
#include "checker.h"
#include "sprite.h"
#include "util.h"

int32_t getCheckerXFromPipLocation(int32_t pipIndex, Board board) {
  int32_t checkerX, pipOffset;
  if (isBetween(pipIndex, 1, 6)) {
    pipOffset = (pipIndex - 1) % 6;
    checkerX = (board.x + board.w) - (board.w * CHECKER_BOARD_X_RIGHT_OFFSET_PCT) - (pipOffset * board.pipW);
  }
  else if (isBetween(pipIndex, 7, 12)) {
    pipOffset = 5 - ((pipIndex - 1) % 6);
    checkerX = board.x + (board.w * CHECKER_BOARD_X_LEFT_OFFSET_PCT) + (pipOffset * board.pipW);
  }
  else if (isBetween(pipIndex, 13, 18)) {
    pipOffset = (pipIndex - 13) % 6;
    checkerX = board.x + (board.w * CHECKER_BOARD_X_LEFT_OFFSET_PCT) + (pipOffset * board.pipW);
  }
  else {
    pipOffset = (pipIndex % 6);
    checkerX = (board.x + board.w) - (board.w * CHECKER_BOARD_X_RIGHT_OFFSET_PCT) - (pipOffset * board.pipW);
  }
  return checkerX;
}

int32_t getCheckerYFromPipLocation(int32_t pipIndex, Board board) {
  int32_t checkerY;
  int32_t checkerSize = board.w * CHECKER_BOARD_WIDTH_PCT;
  if (isBetween(pipIndex, 1, 12)) {
    checkerY = board.y + board.h - checkerSize;
  }
  else {
    checkerY = board.y;
  }
  return checkerY;
}

void clickChecker(ViewManager* vm, Sprite* sprite, void* data) {
  (void)vm;
  (void)sprite;
  Checker* c = (Checker*)data;
  log_debug("Clicked checker %d", c->pipIndex);
  c->pipIndex++;
}

void updateChecker(ViewManager* vm, Sprite* sprite, void* data) {
  (void)vm;
  (void)data;
  (void)sprite;
  // Checker* c = (Checker*)data;
  // SDL_Rect dst_rect = sprite->dst_rect;
  // dst_rect.x = getCheckerXFromPipLocation(c->pipIndex, vm->board);
  // dst_rect.y = getCheckerYFromPipLocation(c->pipIndex, vm->board);
}

Checker* Checker_create(Sage* sage, int32_t pipIndex, Player player, Board board) {
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
  int32_t checkerSize = board.w * CHECKER_BOARD_WIDTH_PCT;

  int32_t checkerX = getCheckerXFromPipLocation(pipIndex, board);
  int32_t checkerY = getCheckerYFromPipLocation(pipIndex, board);

  SDL_Rect dst_rect = { checkerX, checkerY, checkerSize, checkerSize };
  Sprite* s = Sprite_createEx(texture, src_rect, dst_rect, 5, false);
  Sprite_registerUpdateFn(s, updateChecker, c);
  Sprite_registerClickFn(s, clickChecker, c);
  Sage_registerSprite(sage, s);
  return c;
}

void Checker_destroy(Checker* c) {
  free(c);
}
