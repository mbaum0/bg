/**
 * @file game.h
 * @author Michael Baumgarten
 * @brief Contains all functionality related to starting and running the game
 */
#pragma once
#include "media.h"
#include "view.h"
#include "log.h"
#include <SDL.h>
#include <stdbool.h>
#include "board.h"

#define FPS 60
#define MS_PER_FRAME (1000 / FPS)

typedef struct GameManager GameManager;

/**
 * @brief Allocate and initialize a new GameManager instance.
 * This will also initialize the MediaManager and ViewManager.
 * Must be freed with GM_free.
 * 
 * @return GameManager* 
 */
GameManager* GM_init(void);

/**
 * @brief Free memory allocated by the GameManager
 */
void GM_free(GameManager* gm);

/**
 * @brief Run the game loop. This will block until the game is over.
 */
void GM_run(GameManager* gm);
