/**
 * @file events.c
 * @author Michael Baumgarten
 * @brief Event definitions
 */

#include "events.h"

uint32_t SPRITE_CLICK_EVENT = 0;
uint32_t SPRITE_HOVER_EVENT = 0;

void init_events(void) {
    SPRITE_CLICK_EVENT = SDL_RegisterEvents(1);
    SPRITE_HOVER_EVENT = SDL_RegisterEvents(1);
}
