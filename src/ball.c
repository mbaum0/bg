/**
 * @file ball.c
 * @author Michael Baumgarten
 * @brief
 */
#include "ball.h"

void updateBall(Sprite* sprite, void* data) {
  Ball* b = (Ball*)data;

  if (Sprite_getX(sprite) < 0 || Sprite_getX(sprite) > 800 - BALL_SIZE) {
    b->h_vel *= -1;
  }
  if (Sprite_getY(sprite) < 0 || Sprite_getY(sprite) > 600 - BALL_SIZE) {
    b->v_vel *= -1;
  }

  b->x = Sprite_getX(sprite) + b->h_vel;
  b->y = Sprite_getY(sprite) + b->v_vel;
  Sprite_setLocation(sprite, b->x, b->y);
}

Ball* Ball_create(Sage* sage, int32_t h_vel, int32_t v_vel) {
  Ball* b = malloc(sizeof(Ball));
  b->h_vel = h_vel;
  b->v_vel = v_vel;
  b->x = 400;
  b->y = 300;

  SDL_Texture* texture = Sage_loadTexture(sage, "assets/ball.png");
  SDL_Rect src_rect = {0, 0, BALL_SIZE, BALL_SIZE};
  Sprite* s = Sprite_create(texture, src_rect, b->x, b->y, 0, false, updateBall, b, NULL, NULL);
  Sage_registerSprite(sage, s);
  return b;
}

void Ball_destroy(Ball* b) {
  free(b);
}
