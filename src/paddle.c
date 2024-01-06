/**
 * @file paddle.c
 * @author Michael Baumgarten
 * @brief
 */
#include "paddle.h"

void updatePaddle(Sprite* sprite, void* data) {
  Paddle* p = (Paddle*)data;
  p->x += p->vel;
  Sprite_setLocation(sprite, p->x, p->y);
}

void key_cb(uint32_t eventType, SDL_Event* e, void* data) {
  Paddle* p = (Paddle*)data;
  (void)p;
  switch (eventType) {
  case SDL_KEYDOWN:
    if (e->key.repeat == 0) {
      switch (e->key.keysym.sym) {
      case SDLK_LEFT:
        printf("Left key pressed\n");
        p->vel = -10;
        break;
      case SDLK_RIGHT:
        printf("Right key pressed\n");
        p->vel = 10;
        break;
      }
    }
    switch (e->key.keysym.sym) {
    case SDLK_LEFT:
      p->vel = -10;
      break;
    case SDLK_RIGHT:
      p->vel = 10;
      break;
    }
    break;
  case SDL_KEYUP:
    printf("Key up\n");
    p->vel = 0;
    break;
  }
}

Paddle* Paddle_create(Sage* sage) {
  Paddle* p = malloc(sizeof(Paddle));
  p->x = 400;
  p->y = 600 - PADDLE_HEIGHT;
  p->vel = 0;

  SDL_Texture* texture = Sage_loadTexture(sage, "assets/paddle.png");
  SDL_Rect src_rect = {0, 0, PADDLE_WIDTH, PADDLE_HEIGHT};
  Sprite* s = Sprite_create(texture, src_rect, 0, 0, 0, false, updatePaddle, p, NULL, NULL);
  Sage_registerSprite(sage, s);
  Sage_registerEventCallback(sage, SDL_KEYDOWN, key_cb, p);
  Sage_registerEventCallback(sage, SDL_KEYUP, key_cb, p);
  return p;
}
void Paddle_destroy(Paddle* p) {
  free(p);
}
