/**
 * @file buttons.h
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef struct GameButton GameButton;
typedef enum { CONFIRM_BTN, UNDO_BTN } GameButtonType;

struct GameButton {
    GameButtonType kind;
    bool visible;
    int32_t side;
};

void createButtonSprites(GameButton* undo, GameButton* confirm);
