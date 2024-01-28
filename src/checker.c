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

int32_t getCheckerXFromPipLocation(int32_t pipIndex) {
  (void)pipIndex;
  int32_t pipOffset;
  int32_t checkerX;
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

int32_t getCheckersOnPip(int32_t pipIndex, GameBoard* board) {
  int32_t count = 0;
  for (int32_t i = 0; i < 15; i++) {
    if (board->lightCheckers[i].pipIndex == pipIndex) {
      count++;
    }
    if (board->darkCheckers[i].pipIndex == pipIndex) {
      count++;
    }
  }
  return count;
}

int32_t getCheckerY(Checker* checker) {
  int32_t offset;
  int32_t pipIndex = checker->pipIndex;
  if (isBetween(pipIndex, 1, 12)) {
    offset = CHECKER_PIP_12_Y;
    offset -= (checker->pipOffset * CHECKER_W);
  }
  else {
    offset = CHECKER_PIP_13_Y;
    offset += (checker->pipOffset * CHECKER_W);
  }
  return offset;
}

void moveFromPip(int32_t pipIndex, GameBoard* board) {
  // get the top checker on the pip
  Checker* c = NULL;
  int32_t topIndex = -1;

  for (int32_t i = 0; i < 15; i++) {
    Checker* checker = &board->lightCheckers[i];
    if (checker->pipIndex == pipIndex) {
      if (checker->pipOffset > topIndex) {
        topIndex = checker->pipOffset;
        c = checker;
      }
    }
    checker = &board->darkCheckers[i];
    if (checker->pipIndex == pipIndex) {
      if (checker->pipOffset > topIndex) {
        topIndex = checker->pipOffset;
        c = &board->darkCheckers[i];
      }
    }
  }
  if (c == NULL) {
    log_error("no checker found on pip %d", pipIndex);
    return;
  }

  // move the checker
  int32_t newIndex = c->pipIndex + 1;
  if (newIndex > 24) {
    newIndex = 1;
  }
  c->pipOffset = getCheckersOnPip(newIndex, board);
  c->pipIndex = newIndex;
  log_debug("moved checker from pip %d to pip %d", pipIndex, c->pipIndex);
}

void clickChecker(ViewManager* vm, Sprite* sprite, void* object, void* context) {
  (void)vm;
  (void)sprite;
  Checker* checker = (Checker*)object;
  GameBoard* board = (GameBoard*)context;
  moveFromPip(checker->pipIndex, board);
}

void updateChecker(ViewManager* vm, Sprite* sprite, void* object, void* context) {
  (void)vm;
  (void)context;
  (void)sprite;
  (void)object;
  Checker* c = (Checker*)object;
  float newX = getCheckerXFromPipLocation(c->pipIndex);
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

void GameBoard_destroy(GameBoard* board) {
  free(board);
}

void createCheckers(GameBoard* board) {
  SDL_Texture* texture = Sage_loadSVGTexture("assets/checkers.svg", CHECKER_W*2, CHECKER_W);
  SDL_FRect s_lightRect = { 0, 0, CHECKER_W, CHECKER_W };
  SDL_FRect s_darkRect = { CHECKER_W, 0, CHECKER_W, CHECKER_W };

  int32_t lightSetup[] = { 1, 1, 12, 12, 12, 12, 12, 17, 17, 17, 19, 19, 19, 19, 19 };
  int32_t darkSetup[] = { 24, 24, 13, 13, 13, 13, 13, 8, 8, 8, 6, 6, 6, 6, 6 };
  int32_t pipOffsets[] = { 0, 1, 0, 1, 2, 3, 4, 0, 1, 2, 0, 1, 2, 3, 4 };

  for (int32_t i = 0; i < 15; i++) {
    board->lightCheckers[i].pipIndex = lightSetup[i];
    board->lightCheckers[i].pipOffset = pipOffsets[i];
    board->pipCounts[lightSetup[i]]++;
  }

  for (int32_t i = 0; i < 15; i++) {
    board->darkCheckers[i].pipIndex = darkSetup[i];
    board->darkCheckers[i].pipOffset = pipOffsets[i];
    board->pipCounts[darkSetup[i] + 15]++;
  }

  // create the checker sprites
  for (int32_t i = 0; i < 30; i++) {
    if (i < 15) {
      Checker* c = &board->lightCheckers[i];
      int32_t x = getCheckerXFromPipLocation(c->pipIndex);
      int32_t y = getCheckerY(c);
      SDL_FRect dst_rect = { x, y, CHECKER_W, CHECKER_W };
      Sprite* s = Sprite_createEx(texture, s_lightRect, dst_rect, Z_CHECKERS);
      s->useViewport = true;
      Sprite_registerUpdateFn(s, updateChecker, c, board);
      Sprite_registerClickFn(s, clickChecker, c, board);
      Sage_registerSprite(s);
    }
    else {
      Checker* c = &board->darkCheckers[i - 15];
      int32_t x = getCheckerXFromPipLocation(c->pipIndex);
      int32_t y = getCheckerY(c);
      SDL_FRect dst_rect = { x, y, CHECKER_W, CHECKER_W };
      Sprite* s = Sprite_createEx(texture, s_darkRect, dst_rect, Z_CHECKERS);
      s->useViewport = true;
      Sprite_registerUpdateFn(s, updateChecker, c, board);
      Sprite_registerClickFn(s, clickChecker, c, board);
      Sage_registerSprite(s);
    }
  }
}

// void createDice(GameBoard* board) {
//   board->die1 = 1;
//   board->die2 = 1;
//   SDL_Texture* diceTexture = Sage_loadTexture("assets/dice.png");
//   SDL_FRect src_rect = { 0, 0, DIE_SRC_W, DIE_SRC_H };

//   for (int32_t i = 0; i < 2; i++) {
//     float x = DIE_R1_X_NORMAL + (i * DIE_W_NORMAL);
//     float y = DIE_Y_NORMAL;
//     SDL_FRect dst_rect = { x, y, DIE_W_NORMAL, DIE_H_NORMAL };
//     Sprite* s = Sprite_createEx(diceTexture, src_rect, dst_rect, Z_DICE);
//     Sage_registerSprite(s);
//   }
// }


GameBoard* GameBoard_create(void) {
  GameBoard* board = calloc(1, sizeof(GameBoard));
  createCheckers(board);
  //createDice(board);

  return board;
}
