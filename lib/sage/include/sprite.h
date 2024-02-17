/**
 * @file sprite.h
 * @author Michael Baumgarten
 * @brief Sprite implementation for the game engine
 */
#pragma once
#include <SDL3/SDL.h>
#include <stdbool.h>


/**
 * @brief A Sprite is a 2D image that is drawn on the screen. It can be manipulated using
 * the various Sprite_* functions.
 */
typedef struct Sprite Sprite;

struct Sprite {
    Sint32 id;
    SDL_Texture* texture;
    SDL_FRect src_rect;
    SDL_FRect dst_rect;
    bool visible;
    bool hovered;
    void* update_fn;
    void* update_object;
    void* update_context;
    void* click_fn;
    void* click_object;
    void* click_context;
    Sint32 click_code;
    bool useViewport;
    Sint32 z;
    uint8_t alpha;
    SDL_Color outline;
};

/**
 * @brief Create a new Sprite instance
 *
 * @param texture The SDL_Texture to use for this sprite
 * @param src The source rectangle
 * @param x The x coordinate
 * @param y The y coordinate
 * @param z The z coordinate
 * @return Sprite* The new Sprite instance
 */
Sprite* Sprite_create(SDL_Texture* texture, SDL_FRect src, Sint32 x, Sint32 y, Sint32 z);

Sprite* Sprite_createEx(SDL_Texture* texture, SDL_FRect src, SDL_FRect dst, Sint32 z);

/**
 * @brief Set the location of the sprite. Only used in SpriteUpdate_fn callbacks
 */
void Sprite_setLocation(Sprite* sprite, float x, float y);

/**
 * @brief Get the x coordinate of the sprite
 */
float Sprite_getX(Sprite* sprite);

/**
 * @brief Get the y coordinate of the sprite
 */
float Sprite_getY(Sprite* sprite);

/**
 * @brief Set the visibility of the sprite. Only used in SpriteUpdate_fn callbacks
 */
void Sprite_setVisible(Sprite* sprite, bool visible);

/**
 * @brief Set the source rectangle of the sprite. Only used in SpriteUpdate_fn callbacks
 */
void Sprite_setSourceRect(Sprite* sprite, SDL_FRect src);

/**
 * @brief Set the frame of the sprite. Only used in SpriteUpdate_fn callbacks
 */
void Sprite_setFrame(Sprite* sprite, Uint32 frame);

/**
 * @brief Get the frame of the sprite
 */
Uint32 Sprite_getFrame(Sprite* sprite);

void Sprite_setAlpha(Sprite* sprite, uint8_t alpha);

void Sprite_setOutline(Sprite* sprite, SDL_Color color);
