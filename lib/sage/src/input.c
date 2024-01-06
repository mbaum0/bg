/**
 * @file input.c
 * @author Michael Baumgarten
 * @brief
 */
#include "input.h"

bool processInput(void) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      log_debug("Received SDL_QUIT event");
      return true;
    }
  }
  return false;
}
