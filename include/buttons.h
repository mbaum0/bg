/**
 * @file buttons.h
 * @author Michael Baumgarten
 * @brief Game button functions
 */
#pragma once
#include "view.h"
#include <media.h>
#include <stdbool.h>
#include "events.h"
#include "util.h"
#include "dice.h"

#define ROLL_BUTTON_WIDTH 150
#define BUTTON_HEIGHT DICE_SIZE
#define BUTTON_WIDTH DICE_SIZE

typedef struct Button Button;

struct Button {
    Player turn;
    bool isDisabled;
    bool isClicked;
    ButtonType type;
};

/**
 * @brief Create button sprites
 */
void Button_createSprite(Button* button, MediaManager* mm, ViewManager* vm);
