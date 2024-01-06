/**
 * @file paddle.h
 * @author Michael Baumgarten
 * @brief 
 */
#pragma once
#include <stdint.h>
#include "sprite.h"
#include "sage.h"

#define PADDLE_WIDTH 80
#define PADDLE_HEIGHT 10

typedef struct Paddle Paddle;

struct Paddle {
    int32_t x;
    int32_t y;
    int32_t vel;
};

Paddle* Paddle_create(Sage* sage);
void Paddle_destroy(Paddle* p);
