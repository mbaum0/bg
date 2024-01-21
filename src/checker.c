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
    pipOffset = 5 - ((pipIndex - 1) % 6);
    checkerX = CHECKER_RIGHT_OFFSET_NORMAL - (PIP_WIDTH_NORMAL * pipOffset);
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

void createCheckers(Sage* sage, GameBoard* board){
  SDL_Texture* lightTexture = Sage_loadTexture(sage, "assets/light.png");
  SDL_Texture* darkTexture = Sage_loadTexture(sage, "assets/dark.png");
  SDL_Rect src_rect = { 0, 0, CHECKER_SRC_SIZE, CHECKER_SRC_SIZE };

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
      float x = getCheckerXFromPipLocation(c->pipIndex);
      float y = getCheckerY(c);
      SDL_FRect dst_rect = { x, y, CHECKER_W_NORMAL, CHECKER_H_NORMAL };
      Sprite* s = Sprite_createEx(lightTexture, src_rect, dst_rect, Z_CHECKERS, false, true, true, false);
      Sprite_registerUpdateFn(s, updateChecker, c, board);
      Sprite_registerClickFn(s, clickChecker, c, board);
      Sage_registerSprite(sage, s);
    }
    else {
      Checker* c = &board->darkCheckers[i - 15];
      float x = getCheckerXFromPipLocation(c->pipIndex);
      float y = getCheckerY(c);
      SDL_FRect dst_rect = { x, y, CHECKER_W_NORMAL, CHECKER_H_NORMAL };
      Sprite* s = Sprite_createEx(darkTexture, src_rect, dst_rect, Z_CHECKERS, false, true, true, false);
      Sprite_registerUpdateFn(s, updateChecker, c, board);
      Sprite_registerClickFn(s, clickChecker, c, board);
      Sage_registerSprite(sage, s);
    }
  }
}

void createDice(Sage* sage, GameBoard* board){
  board->die1 = 1;
  board->die2 = 1;
  SDL_Texture* diceTexture = Sage_loadTexture(sage, "assets/dice.png");
  SDL_Rect src_rect = { 0, 0, DIE_SRC_W, DIE_SRC_H };

  for (int32_t i = 0; i < 2; i++) {
    float x = DIE_R1_X_NORMAL + (i * DIE_W_NORMAL);
    float y = DIE_Y_NORMAL;
    SDL_FRect dst_rect = { x, y, DIE_W_NORMAL, DIE_H_NORMAL };
    Sprite* s = Sprite_createEx(diceTexture, src_rect, dst_rect, Z_DICE, false, true, true, false);
    Sage_registerSprite(sage, s);
  }
}


GameBoard* GameBoard_create(Sage* sage) {
  GameBoard* board = calloc(1, sizeof(GameBoard));
  createCheckers(sage, board);
  createDice(sage, board);

  return board;
}
