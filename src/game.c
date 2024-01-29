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
#include "util.h"

extern uint32_t PipClickEventType;
extern uint32_t CheckerClickEventType;
extern uint32_t DiceClickEventType;

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

    if (pipIndex == 0){
        return;
    }

    for (int32_t i = 0; i < 15; i++) {
        Checker* checker = &gb->lightCheckers[i];
        if (checker->pipIndex == pipIndex) {
            if (checker->pipYOffset > topIndex) {
                topIndex = checker->pipYOffset;
                c = checker;
            }
        }
        checker = &gb->darkCheckers[i];
        if (checker->pipIndex == pipIndex) {
            if (checker->pipYOffset > topIndex) {
                topIndex = checker->pipYOffset;
                c = &gb->darkCheckers[i];
            }
        }
    }
    if (c == NULL) {
        log_error("no checker found on pip %d", pipIndex);
        return;
    }

    // move the checker
    int32_t direction = (c->color == LIGHT) ? 1 : -1;
    amount *= direction;
    int32_t newIndex = c->pipIndex + amount;
    if (newIndex <= 0 || newIndex >= 25){
        newIndex = 0;
    }
    c->pipYOffset = gb->pipCounts[newIndex];
    c->pipIndex = newIndex;
    gb->pipCounts[newIndex]++;
    gb->pipCounts[pipIndex]--;
    log_debug("moved checker from pip %d to pip %d", pipIndex, c->pipIndex);
}

void handlePipClick(uint32_t eventType, SDL_Event* e, void* data) {
    (void)eventType;
    GameBoard* gb = (GameBoard*)data;
    uint32_t pipIndex = e->user.code;
    log_debug("pip %d clicked", pipIndex);
    moveChecker(gb, pipIndex, gb->die1);
}

void handleCheckerClick(uint32_t eventType, SDL_Event* e, void* data) {
    (void)eventType;
    GameBoard* gb = (GameBoard*)data;
    Checker* checker = (Checker*)e->user.data1;
    log_debug("checker on pip %d clicked", checker->pipIndex);
    moveChecker(gb, checker->pipIndex, gb->die1);
}

void handleDiceClick(uint32_t eventType, SDL_Event* e, void* data){
    (void)eventType;
    (void)e;
    GameBoard* gb = (GameBoard*)data;
    log_debug("got dice click event");
    gb->die1 = generateRandomNumber(1, 6);
    gb->die2 = generateRandomNumber(1, 6);
}


void initCheckerSetup(GameBoard* gb) {
    int32_t lightSetup[] = { 1, 1, 12, 12, 12, 12, 12, 17, 17, 17, 19, 19, 19, 19, 19 };
    int32_t darkSetup[] = { 24, 24, 13, 13, 13, 13, 13, 8, 8, 8, 6, 6, 6, 6, 6 };
    //int32_t pipYOffsets[] = { 0, 1, 0, 1, 2, 3, 4, 0, 1, 2, 0, 1, 2, 3, 4 };

    int32_t pipIndex;
    for (int32_t i = 0; i < 15; i++) {
        pipIndex = lightSetup[i];
        gb->lightCheckers[i].pipIndex = pipIndex;
        gb->lightCheckers[i].pipYOffset = gb->pipCounts[pipIndex];
        gb->lightCheckers[i].color = LIGHT;
        gb->pipCounts[pipIndex]++;
    }

    for (int32_t i = 0; i < 15; i++) {
        pipIndex = darkSetup[i];
        gb->darkCheckers[i].pipIndex = pipIndex;
        gb->darkCheckers[i].pipYOffset = gb->pipCounts[pipIndex];
        gb->darkCheckers[i].color = DARK;
        gb->pipCounts[pipIndex]++;
    }
}

void initEventCallbacks(GameBoard* gb) {
    PipClickEventType = SDL_RegisterEvents(1);
    CheckerClickEventType = SDL_RegisterEvents(1);
    DiceClickEventType = SDL_RegisterEvents(1);
    Sage_registerEventCallback(PipClickEventType, handlePipClick, gb);
    Sage_registerEventCallback(CheckerClickEventType, handleCheckerClick, gb);
    Sage_registerEventCallback(DiceClickEventType, handleDiceClick, gb);
}

GameBoard* GameBoard_create(void) {
    GameBoard* gb = calloc(1, sizeof(GameBoard));
    gb->die1 = 1;
    gb->die2 = 2;
    initCheckerSetup(gb);
    createBoardSprites();
    createPipSprites();
    createDiceSprites(&gb->die1, &gb->die2);
    for (int32_t i = 0; i < 15; i++) {
        createCheckerSprite(&gb->lightCheckers[i]);
        createCheckerSprite(&gb->darkCheckers[i]);
    }
    initEventCallbacks(gb);
    return gb;
}
