/**
 * @file events.h
 * @author Michael Baumgarten
 * @brief Contains custom event definitions
 */
#pragma once

#include <SDL.h>
#include <stdint.h>
#include <stdbool.h>

extern uint32_t SPRITE_CLICK_EVENT;
extern uint32_t SPRITE_HOVER_EVENT;
extern uint32_t LOCATION_CLICK_EVENT;

/**
 * @brief Event data for a sprite is clicked on
 */
typedef struct SpriteClickEvent {
    int sprite_id;
} SpriteClickEvent;

/**
 * @brief Event data for a sprite that is being hovered over
*/
typedef struct SpriteHoverEvent {
    int sprite_id;
    bool hovered;
} SpriteHoverEvent;

/**
 * @brief Event data for a location being clicked on
 */
typedef struct LocationClickEvent {
    int32_t location;
} LocationClickEvent;

/**
 * @brief Registers custom events with the SDL event system. This function must
 * be called before any custom events can be used.
 */
void init_events(void);
