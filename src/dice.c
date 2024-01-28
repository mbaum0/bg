/**
 * @file dice.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief 
 */
#include "dice.h"
#include "sage.h"
#include "util.h"

void createDiceSprites(void) {
  SDL_Texture* diceTexture = Sage_loadSVGTexture("assets/dice.svg", DICE_W*6, DICE_W);
  SDL_FRect src_rect = { 0, 0, DICE_W, DICE_W };

  SDL_FRect dst_rect = { DICE_RIGHT_X, DICE_Y, DICE_W, DICE_W };
  Sprite* s = Sprite_createEx(diceTexture, src_rect, dst_rect, Z_DICE);
  Sage_registerSprite(s);

  dst_rect.x += DICE_GAP;
  s = Sprite_createEx(diceTexture, src_rect, dst_rect, Z_DICE);
  Sage_registerSprite(s);
}

