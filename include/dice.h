/**
 * @file dice.h
 * @author Michael Baumgarten
 * @brief Contains dice related functionality
 */
#pragma once
#include <stdint.h>
#include <stdlib.h>
#include "util.h"
#include "view.h"
#include "events.h"
#include "media.h"

#define DICE_SIZE 60
#define DICE_GAP 10
#define DIE_0_LEFT_X_OFFSET (PLAY_AREA_LEFT_SIDE_X_OFFSET + (PLAY_AREA_WIDTH / 2) - DICE_SIZE - DICE_GAP)
#define DIE_1_LEFT_X_OFFSET (DIE_0_LEFT_X_OFFSET + DICE_SIZE + DICE_GAP + DICE_GAP)

#define DIE_0_RIGHT_X_OFFSET (PLAY_AREA_RIGHT_SIDE_X_OFFSET + (PLAY_AREA_WIDTH / 2) - (DICE_SIZE / 2))
#define DIE_1_RIGHT_X_OFFSET (DIE_0_RIGHT_X_OFFSET + DICE_SIZE + DICE_GAP)

#define DICE_Y_OFFSET (PLAY_AREA_TOP_OFFSET + (PLAY_AREA_HEIGHT / 2) - (DICE_SIZE / 2))

typedef struct Die Die;

typedef enum {
    D_Left,
    D_Right
} DieSide;

struct Die {
    int32_t value;
    int32_t rollCount;
    DieSide side;
};

/**
 * @brief Create sprites for a die
 */
void Dice_createSprite(Die* die, MediaManager* mm, ViewManager* vm);

/**
 * @brief Roll a die
 */
void Dice_roll(Die* die);
