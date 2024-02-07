/**
 * @file board.h
 * @author Michael Baumgarten
 * @brief board and trim objects
 */
#pragma once
#include <stdint.h>

#define FIRST_DIE_VALUE(gb) ((gb->die1.index == 0) ? gb->die1.value : gb->die2.value)
#define SECOND_DIE_VALUE(gb) ((gb->die2.index == 1) ? gb->die2.value : gb->die1.value)

void createBoardSprites(void);
