/**
 * @file ball.h
 * @author Michael Baumgarten
 * @brief 
 */
#pragma once
#include <stdint.h>
#include "sprite.h"
#include "sage.h"

#define BALL_SIZE 15
typedef struct Ball Ball;

struct Ball {
    int32_t x;
    int32_t y;
    int32_t h_vel;
    int32_t v_vel;
};

Ball* Ball_create(Sage* sage, int32_t h_vel, int32_t v_vel);

void Ball_destroy(Ball* b);
