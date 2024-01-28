/**
 * @file game.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#include "stdlib.h"
#include "game.h"
#include "dice.h"
#include "pip.h"
#include "board.h"

extern uint32_t PipClickEventType;
extern uint32_t CheckerClickEventType;

int32_t getNumCheckersOnPip(GameBoard* gb, int32_t pipIndex) {
    int32_t count = 0;
    Checker* checker;
    for (int32_t i = 0; i < 15; i++) {
        checker = &gb->lightCheckers[i];
        if (checker->pipIndex == pipIndex) {
            count += 1;
        }
        checker = &gb->darkCheckers[i];
        if (checker->pipIndex == pipIndex) {
            count += 1;
        }
    }
    return count;
}

void moveChecker(GameBoard* gb, int32_t pipIndex, int32_t amount) {
    // get the top checker on the pip
    Checker* c = NULL;
    int32_t topIndex = -1;

    for (int32_t i = 0; i < 15; i++) {
        Checker* checker = &gb->lightCheckers[i];
        if (checker->pipIndex == pipIndex) {
            if (checker->pipOffset > topIndex) {
                topIndex = checker->pipOffset;
                c = checker;
            }
        }
        checker = &gb->darkCheckers[i];
        if (checker->pipIndex == pipIndex) {
            if (checker->pipOffset > topIndex) {
                topIndex = checker->pipOffset;
                c = &gb->darkCheckers[i];
            }
        }
    }
    if (c == NULL) {
        log_error("no checker found on pip %d", pipIndex);
        return;
    }

    // move the checker
    amount = amount * (c->color) ? 1 : -1;
    int32_t newIndex = c->pipIndex + amount;
    newIndex %= 25;
    c->pipOffset = getNumCheckersOnPip(gb, newIndex);
    c->pipIndex = newIndex;
    log_debug("moved checker from pip %d to pip %d", pipIndex, c->pipIndex);
}

void handlePipClick(uint32_t eventType, SDL_Event* e, void* data) {
    (void)eventType;
    GameBoard* gb = (GameBoard*)data;
    uint32_t pipIndex = e->user.code;
    log_debug("pip %d clicked", pipIndex);
    moveChecker(gb, pipIndex, 1);
}

void handleCheckerClick(uint32_t eventType, SDL_Event* e, void* data) {
    (void)eventType;
    GameBoard* gb = (GameBoard*)data;
    Checker* checker = (Checker*)e->user.data1;
    log_debug("checker on pip %d clicked", checker->pipIndex);
    moveChecker(gb, checker->pipIndex, 1);
}


void initCheckerSetup(GameBoard* gb) {
    int32_t lightSetup[] = { 1, 1, 12, 12, 12, 12, 12, 17, 17, 17, 19, 19, 19, 19, 19 };
    int32_t darkSetup[] = { 24, 24, 13, 13, 13, 13, 13, 8, 8, 8, 6, 6, 6, 6, 6 };
    int32_t pipOffsets[] = { 0, 1, 0, 1, 2, 3, 4, 0, 1, 2, 0, 1, 2, 3, 4 };

    for (int32_t i = 0; i < 15; i++) {
        gb->lightCheckers[i].pipIndex = lightSetup[i];
        gb->lightCheckers[i].pipOffset = pipOffsets[i];
        gb->lightCheckers[i].color = LIGHT;
        gb->pipCounts[lightSetup[i]]++;
    }

    for (int32_t i = 0; i < 15; i++) {
        gb->darkCheckers[i].pipIndex = darkSetup[i];
        gb->darkCheckers[i].pipOffset = pipOffsets[i];
        gb->lightCheckers[i].color = DARK;
        gb->pipCounts[darkSetup[i] + 15]++;
    }
}

void initEventCallbacks(GameBoard* gb) {
    PipClickEventType = SDL_RegisterEvents(1);
    CheckerClickEventType = SDL_RegisterEvents(1);
    Sage_registerEventCallback(PipClickEventType, handlePipClick, gb);
    Sage_registerEventCallback(CheckerClickEventType, handleCheckerClick, gb);
}

GameBoard* GameBoard_create(void) {
    GameBoard* gb = calloc(1, sizeof(GameBoard));
    initCheckerSetup(gb);
    createBoardSprites();
    createPipSprites();
    createDiceSprites();
    for (int32_t i = 0; i < 15; i++) {
        createCheckerSprite(&gb->lightCheckers[i]);
        createCheckerSprite(&gb->darkCheckers[i]);
    }
    initEventCallbacks(gb);
    return gb;
}
