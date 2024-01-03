/**
 * @file sprite.h
 * @author Michael Baumgarten
 * @brief Sprite implementation for the game engine
 */
#pragma once
#include <SDL.h>
#include <stdint.h>
#include <stdbool.h>

 /**
  * @brief A Sprite is a 2D image that is drawn on the screen. It can be manipulated using
  * the various Sprite_* functions.
  */
typedef struct Sprite Sprite;

typedef struct SpriteArray SpriteArray;

/**
 * @brief Sprite update functions are called every frame to update the Sprite's state.
 */
typedef void (*SpriteUpdate_fn)(Sprite* sprite, void* data);

/**
 * @brief Sprite click functions are called when the Sprite is clicked.
 */
typedef void (*SpriteClick_fn)(void* data);

struct SpriteArray {
    Sprite** sprites;
    int32_t size;
};

struct Sprite {
    int32_t id;
    SDL_Texture* texture;
    SDL_Rect src_rect;
    SDL_Rect dst_rect;
    bool visible;
    bool hovered;
    SpriteUpdate_fn update_fn;
    void* update_data;
    SpriteClick_fn click_fn;
    void* click_data;
    int32_t z;
    bool flip;
    uint32_t frame; // for storing arbitrary data;
};

/**
 * @brief Set the location of the sprite. Only used in SpriteUpdate_fn callbacks
 */
void Sprite_setLocation(Sprite* sprite, int32_t x, int32_t y);

/**
 * @brief Get the x coordinate of the sprite
 */
int32_t Sprite_getX(Sprite* sprite);

/**
 * @brief Get the y coordinate of the sprite
 */
int32_t Sprite_getY(Sprite* sprite);

/**
 * @brief Set the visibility of the sprite. Only used in SpriteUpdate_fn callbacks
 */
void Sprite_setVisible(Sprite* sprite, bool visible);

/**
 * @brief Set the source rectangle of the sprite. Only used in SpriteUpdate_fn callbacks
 */
void Sprite_setSourceRect(Sprite* sprite, SDL_Rect src);

/**
 * @brief Set the frame of the sprite. Only used in SpriteUpdate_fn callbacks
*/
void Sprite_setFrame(Sprite* sprite, uint32_t frame);

/**
 * @brief Get the frame of the sprite
*/
uint32_t Sprite_getFrame(Sprite* sprite);
