/**
 * @file sand.h
 * @author Michael Baumgarten
 * @brief A particle of sand
 */
#pragma once
#include "sage.h"

typedef struct Sand Sand;

struct Sand {
    int32_t x;
    int32_t y;
    float acc;
    float vel;
};

Sand* Sand_create(Sage* sage, int32_t x, int32_t y);
void Sand_destroy(Sand* s);
void Sand_init(Sage* sage);
