/**
 * @file buttons.h
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef struct GameButton GameButton;
typedef enum { CONFIRM_BTN, UNDO_BTN, ROLL_BTN } GameButtonType;
typedef enum { BTN_LEFT, BTN_CENTER, BTN_RIGHT } GameButtonLocation;

struct GameButton {
    GameButtonType kind;
    bool visible;
    GameButtonLocation location;
};

void createButtonSprites(GameButton* undo, GameButton* confirm, GameButton* roll);
