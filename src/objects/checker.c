/**
 * @file checker.c
 * @author Michael Baumgarten
 * @brief
 */
#include "checker.h"
#include "board.h"
#include "dstats.h"
#include "fsm.h"
#include "game.h"
#include "sprite.h"
#include "util.h"
#include "vector.h"

Sint32 getCheckerX(Checker* checker) {
    Sint32 pipIndex = checker->pipIndex;
    Sint32 pipOffset;
    Sint32 checkerX;

    if (checker->player == PLAYER_ONE) {
        if (pipIndex == PLAYER_ONE_HOME) {
            return CHECKER_BOT_HOME_X + (checker->pipOffset * CHECKER_GAP);
        }
        if (pipIndex == PLAYER_ONE_BAR) {
            return CHECKER_BAR_X;
        }
    } else if (checker->player == PLAYER_TWO) {
        if (pipIndex == PLAYER_TWO_HOME) {
            return CHECKER_TOP_HOME_X + (checker->pipOffset * CHECKER_GAP);
        }
        if (pipIndex == PLAYER_TWO_BAR) {
            return CHECKER_BAR_X;
        }
    }

    if (isBetween(pipIndex, 1, 6)) {
        pipOffset = (6 - pipIndex);
        checkerX = CHECKER_PIP_6_X + (PIP_W * pipOffset);
    } else if (isBetween(pipIndex, 7, 12)) {
        pipOffset = (6 - pipIndex) + 6;
        checkerX = CHECKER_PIP_12_X + (PIP_W * pipOffset);
    } else if (isBetween(pipIndex, 13, 18)) {
        pipOffset = (pipIndex - 13);
        checkerX = CHECKER_PIP_13_X + (PIP_W * pipOffset);
    } else {
        pipOffset = (pipIndex - 19);
        checkerX = CHECKER_PIP_19_X + (PIP_W * pipOffset);
    }
    return checkerX;
}

Sint32 getCheckerY(Checker* checker) {
    Sint32 offset;
    Sint32 pipIndex = checker->pipIndex;

    if (checker->player == PLAYER_ONE) {
        if (pipIndex == PLAYER_ONE_HOME) {
            return CHECKER_BOT_HOME_Y;
        }
        if (pipIndex == PLAYER_ONE_BAR) {
            return CHECKER_BAR_BOT_Y;
        }
    } else if (checker->player == PLAYER_TWO) {
        if (pipIndex == PLAYER_TWO_HOME) {
            return CHECKER_TOP_HOME_Y;
        }
        if (pipIndex == PLAYER_TWO_BAR) {
            return CHECKER_BAR_TOP_Y;
        }
    }

    Uint32 checkerGap = CHECKER_GAP;
    if (checker->numNeighbors > 5) {
        checkerGap = (CHECKER_GAP * 5) / checker->numNeighbors;
    }
    if (isBetween(pipIndex, 1, 12)) {
        offset = CHECKER_PIP_12_Y;
        offset -= (checker->pipOffset * checkerGap);
    } else {
        offset = CHECKER_PIP_13_Y;
        offset += (checker->pipOffset * checkerGap);
    }
    return offset;
}

void clickChecker(ViewManager* vm, Sprite* sprite, void* object, void* context, Sint32 code) {
    (void)vm;
    (void)sprite;
    (void)context;
    (void)code;
    Checker* checker = (Checker*)object;
    Sint32 pipIndex = checker->pipIndex;
    FSMEvent e = {PIP_CLICKED_EVENT, pipIndex, NULL};
    fsm_enqueue_event(e);
}

void updateChecker(ViewManager* vm, Sprite* sprite, void* object, void* context) {
    (void)vm;
    (void)context;
    Checker* c = (Checker*)object;
    float newX = getCheckerX(c);
    float newY = getCheckerY(c);
    float x = Sprite_getX(sprite);
    float y = Sprite_getY(sprite);

    if (!isEqual(x, newX, CHECKER_VELOCITY) || !isEqual(y, newY, CHECKER_VELOCITY)) {
        float xVel = getHorizontalVelocity(CHECKER_VELOCITY, x, y, newX, newY);
        float yVel = getVerticalVelocity(CHECKER_VELOCITY, x, y, newX, newY);
        Sprite_setZ(sprite, Z_MOVING_CHECKERS);
        Sprite_setLocation(sprite, x + xVel, y + yVel);
    } else {
        if (IS_CHECKER_BARRED(c)) {
            Sprite_setZ(sprite, Z_BAR_CHECKERS);
        } else {
            // add the pip offset so the overlap properly if compressed
            Sprite_setZ(sprite, Z_CHECKERS + c->pipOffset);
        }
        Sprite_setLocation(sprite, newX, newY);
    }
}

void createCheckerSprite(Checker* c) {
    SDL_Texture* texture = Sage_loadTexture("assets/checkers2x.png");
    SDL_FRect s_lightRect = {0, 0, CHECKER_SRC_W * 2, CHECKER_SRC_W * 2};
    SDL_FRect s_darkRect = {CHECKER_SRC_W * 2, 0, CHECKER_SRC_W * 2, CHECKER_SRC_W * 2};

    Sint32 x = getCheckerX(c);
    Sint32 y = getCheckerY(c);

    SDL_FRect dst = {x, y, CHECKER_W, CHECKER_W};
    Sprite* s;
    if (c->player == PLAYER_ONE) { // player one defaults to light
        s = Sprite_createEx(texture, s_lightRect, dst, Z_CHECKERS);
    } else {
        s = Sprite_createEx(texture, s_darkRect, dst, Z_CHECKERS);
    }
    s->useViewport = true;

    Sprite_registerUpdateFn(s, updateChecker, c, NULL);
    Sprite_registerClickFn(s, clickChecker, c, NULL, 0);
    Sage_registerSprite(s);
}
