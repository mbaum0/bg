/**
 * @file game.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#include <stdlib.h>
#include "game.h"
#include "dice.h"
#include "pip.h"
#include "board.h"
#include "util.h"
#include "fsm.h"

extern FiniteStateMachine FSM;

// void rollDiceIfPossible(GameBoard* gb);
// void swapDiceIfPossible(GameBoard* gb);
// void updateGameState(GameBoard* gb, GameState state);
// void moveCheckerIfPossible(GameBoard* gb, Checker* c);
// bool isValidMove(GameBoard* gb, Checker* checker, int32_t amount);
// Color getPipOwner(GameBoard* gb, int32_t pipIndex);
// bool playerHasAllCheckersHome(GameBoard* gb, Color player);
// bool playerHasMoves(GameBoard* gb, Color player, bool bothDice);
// uint32_t handleTimerEndPlayerTurnIfNoMoves(uint32_t interval, void* ctx);
// Checker* getTopCheckerOnPip(GameBoard* gb, int32_t pipIndex);
// int32_t getNumCheckersOnPip(GameBoard* gb, int32_t pipIndex);
// bool playerHasCheckersOnBar(GameBoard* gb, Color player);
// int32_t getNextPipIndex(Checker* c, int32_t amount);
// void saveCheckerState(GameBoard* gb);
// void loadCheckerState(GameBoard* gb);
// void confirmIfPossible(GameBoard* gb);
// void undoIfPossible(GameBoard* gb);
// void setDiceUsed(GameBoard* gb, int32_t diceUsed);
// void checkForWinner(GameBoard* gb);

// void updateGameState(GameBoard* gb, GameState state) {
//     switch (state)
//     {
//     case LIGHT_DICE_ROLL:
//         log_debug("Entered state: LIGHT_DICE_ROLL");
//         gb->die1.side = 0;
//         gb->die2.side = 0;
//         gb->die1.animation = DICE_MOVE;
//         gb->die2.animation = DICE_MOVE;
//         setDiceUsed(gb, 0);
//         gb->confirm.visible = false;
//         gb->undo.visible = false;
//         break;
//     case LIGHT_MOVE_ONE:
//         log_debug("Entered state: LIGHT_MOVE_ONE");
//         gb->die1.animation = DICE_SWAP;
//         gb->die2.animation = DICE_SWAP;
//         setDiceUsed(gb, 0);
//         gb->confirm.visible = false;
//         gb->undo.visible = false;
//         saveCheckerState(gb);
//         SDL_AddTimer(1000, handleTimerEndPlayerTurnIfNoMoves, gb);
//         break;
//     case LIGHT_MOVE_TWO:
//         log_debug("Entered state: LIGHT_MOVE_TWO");
//         setDiceUsed(gb, 1);
//         SDL_AddTimer(1000, handleTimerEndPlayerTurnIfNoMoves, gb);
//         break;
//     case LIGHT_CONFIRM:
//         log_debug("Entered state: LIGHT_CONFIRM");
//         setDiceUsed(gb, 2);
//         gb->confirm.visible = true;
//         gb->confirm.side = 0;
//         gb->undo.visible = true;
//         gb->undo.side = 0;
//         break;
//     case DARK_DICE_ROLL:
//         log_debug("Entered state: DARK_DICE_ROLL");
//         gb->die1.side = 1;
//         gb->die2.side = 1;
//         gb->die1.animation = DICE_MOVE;
//         gb->die2.animation = DICE_MOVE;
//         setDiceUsed(gb, 0);
//         gb->confirm.visible = false;
//         gb->undo.visible = false;
//         break;
//     case DARK_MOVE_ONE:
//         log_debug("Entered state: DARK_MOVE_ONE");
//         gb->die1.animation = DICE_SWAP;
//         gb->die2.animation = DICE_SWAP;
//         gb->confirm.visible = false;
//         gb->undo.visible = false;
//         setDiceUsed(gb, 0);
//         saveCheckerState(gb);
//         SDL_AddTimer(1000, handleTimerEndPlayerTurnIfNoMoves, gb);
//         break;
//     case DARK_MOVE_TWO:
//         log_debug("Entered state: DARK_MOVE_TWO");
//         setDiceUsed(gb, 1);
//         SDL_AddTimer(1000, handleTimerEndPlayerTurnIfNoMoves, gb);
//         break;
//     case DARK_CONFIRM:
//         log_debug("Entered state: DARK_CONFIRM");
//         setDiceUsed(gb, 2);
//         gb->confirm.visible = true;
//         gb->confirm.side = 1;
//         gb->undo.visible = true;
//         gb->undo.side = 1;
//         break;
//     case GAME_OVER:
//         log_debug("Entered state: GAME_OVER");
//         log_debug("Player %d wins!", gb->winner);
//         return;
//     default:
//         break;
//     }
//     gb->state = state;
//     checkForWinner(gb);
// }

// void setDiceUsed(GameBoard* gb, int32_t diceUsed) {
//     if (diceUsed == 0) {
//         gb->die1.used = false;
//         gb->die2.used = false;
//     }
//     else if (diceUsed == 1) {
//         if (gb->die1.index == 0) {
//             gb->die1.used = true;
//             gb->die2.used = false;
//         }
//         else {
//             gb->die1.used = false;
//             gb->die2.used = true;
//         }
//     }
//     else {
//         gb->die1.used = true;
//         gb->die2.used = true;
//     }
// }

// void saveCheckerState(GameBoard* gb) {
//     memcpy(gb->lightCheckersSave, gb->lightCheckers, sizeof(gb->lightCheckersSave));
//     memcpy(gb->darkCheckersSave, gb->darkCheckers, sizeof(gb->darkCheckersSave));
// }

// void loadCheckerState(GameBoard* gb) {
//     memcpy(gb->lightCheckers, gb->lightCheckersSave, sizeof(gb->lightCheckers));
//     memcpy(gb->darkCheckers, gb->darkCheckersSave, sizeof(gb->darkCheckers));
// }

bool isPipOpponentBlot(GameBoard* gb, int32_t pipIndex) {
    Color opponent = OPPONENT_COLOR(gb->activePlayer);

    int32_t numCheckers = getNumCheckersOnPip(gb, pipIndex);
    if (numCheckers == 1) {
        Checker* c = getTopCheckerOnPip(gb, pipIndex);
        return (c->color == opponent);
    }
    return false;
}

// Color getPipOwner(GameBoard* gb, int32_t pipIndex) {
//     if (pipIndex == DARK_HOME) {
//         return DARK;
//     }
//     if (pipIndex == LIGHT_HOME) {
//         return LIGHT;
//     }
//     Checker* c = getTopCheckerOnPip(gb, pipIndex);
//     if (c == NULL) {
//         return NONE;
//     }
//     int32_t numCheckers = getNumCheckersOnPip(gb, pipIndex);
//     if (numCheckers == 1) {
//         return BLOT;
//     }
//     if (c->color == DARK) {
//         return DARK;
//     }
//     if (c->color == LIGHT) {
//         return LIGHT;
//     }
//     return NONE;
// }

bool playerHasCheckersOnBar(GameBoard* gb) {
    if (gb->activePlayer == LIGHT) {
        return getNumCheckersOnPip(gb, LIGHT_BAR) > 0;
    }
    else {
        return getNumCheckersOnPip(gb, DARK_BAR) > 0;
    }
}

int32_t getNextDieValue(GameBoard* gb) {
    GameDie die1 = FIRST_DIE(gb);
    GameDie die2 = SECOND_DIE(gb);
    if (DOUBLES_ROLLED(gb)) {
        // if we have doubles, uses max at 2
        if (die1.uses < 2) {
            return die1.value;
        }
        if (die2.uses < 2) {
            return die2.value;
        }
    }
    // no doubles
    if (die1.uses == 0) {
        return die1.value;
    }
    if (die2.uses == 0) {
        return die2.value;
    }
    return -1;
}

bool playerHasClosedBoard(GameBoard* gb, Color player) {
    Checker checker;
    for (int32_t i = 0; i < 15; i++) {
        if (player == LIGHT) {
            checker = gb->lightCheckers[i];
        }
        else {
            checker = gb->darkCheckers[i];
        }

        if (player == LIGHT && !isBetween(checker.pipIndex, LIGHT_HOME_START, LIGHT_HOME_END)) {
            return false;
        }
        if (player == DARK && !isBetween(checker.pipIndex, DARK_HOME_START, DARK_HOME_END)) {
            return false;
        }
    }
    return true;
}

// void checkForWinner(GameBoard* gb){
//     int32_t numLightHome = 0;
//     int32_t numDarkHome = 0;
//     for (int32_t i = 0; i < 15; i++){
//         if (gb->lightCheckers[i].pipIndex == LIGHT_HOME){
//             numLightHome++;
//         }
//         if (gb->darkCheckers[i].pipIndex == DARK_HOME){
//             numDarkHome++;
//         }
//     }
//     if (numLightHome == 15){
//         gb->winner = LIGHT;
//         updateGameState(gb, GAME_OVER);
//         return;
//     }
//     if (numDarkHome == 15){
//         gb->winner = DARK;
//         updateGameState(gb, GAME_OVER);
//         return;
//     }
// }

// /**
//  * @brief Returns true if the player has any valid moves
//  *
//  * @param gb
//  * @return true
//  * @return false
//  */
// bool playerHasMoves(GameBoard* gb, Color player, bool bothDice) {
//     Checker* c;
//     int32_t firstDie = FIRST_DIE_VALUE(gb);
//     int32_t secondDie = SECOND_DIE_VALUE(gb);
//     for (int32_t i = 0; i < 15; i++) {
//         if (player == DARK) {
//             c = &gb->darkCheckers[i];
//         }
//         else {
//             c = &gb->lightCheckers[i];
//         }
//         if (bothDice) {
//             if (isValidMove(gb, c, firstDie)) {
//                 return true;
//             }
//         }

//         if (isValidMove(gb, c, secondDie)) {
//             return true;
//         }
//     }
//     return false;
// }

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

Checker* getTopCheckerOnPip(GameBoard* gb, int32_t pipIndex) {
    Checker* topChecker = NULL;
    int32_t topIndex = -1;
    for (int32_t i = 0; i < 15; i++) {
        Checker* c = &gb->lightCheckers[i];
        if (c->pipIndex == pipIndex && c->pipOffset > topIndex) {
            topChecker = c;
        }
        c = &gb->darkCheckers[i];
        if (c->pipIndex == pipIndex && c->pipOffset > topIndex) {
            topChecker = c;
        }
    }
    return topChecker;
}

int32_t getNextPip(Checker* c, int32_t amount) {
    int32_t oldPip = c->pipIndex;
    int32_t direction = (c->color == LIGHT) ? 1 : -1;
    int32_t newPip;
    if (oldPip == LIGHT_BAR) {
        newPip = DARK_HOME + amount;
    }
    else if (oldPip == DARK_BAR) {
        newPip = LIGHT_HOME - amount;
    }
    else {
        newPip = oldPip + (direction * amount);
    }
    newPip = CAP_PIP_INDEX(newPip);
    return newPip;
}

// // returns true if a move is valid
bool isValidMove(GameBoard* gb, Checker* c, int32_t amount) {
    Color player = c->color;
    int32_t oldPip = c->pipIndex;
    int32_t newPip = getNextPip(c, amount);

    // not valid if moving home piece that is already home
    if (oldPip == LIGHT_HOME || oldPip == DARK_HOME) {
        return false;
    }

    // not valid if any checkers are barred and moving a non-barred checker
    if (playerHasCheckersOnBar(gb)) {
        if (player == LIGHT && oldPip != LIGHT_BAR) {
            return false;
        }
        if (player == DARK && oldPip != DARK_BAR) {
            return false;
        }
    }

    // not valid if moving bearing off but not all checkers are home
    if ((newPip == DARK_HOME || newPip == LIGHT_HOME) && !playerHasClosedBoard(gb, player)) {
        return false;
    }

    int32_t numCheckers = getNumCheckersOnPip(gb, newPip);
    // not valid move if moving checker to opponent owned-pip
    if (numCheckers > 1) {
        Checker* topChecker = getTopCheckerOnPip(gb, newPip);
        if (topChecker->color == OPPONENT_COLOR(c)) {
            return false;
        }
    }

    return true;
}

void barChecker(GameBoard* gb, Checker* c) {
    if (c->color == DARK) {
        c->pipOffset = getNumCheckersOnPip(gb, DARK_BAR);
        c->pipIndex = DARK_BAR;
    }
    else {
        c->pipOffset = getNumCheckersOnPip(gb, LIGHT_BAR);
        c->pipIndex = LIGHT_BAR;
    }
}

// void handlePipClick(uint32_t eventType, SDL_Event* e, void* data) {
//     (void)eventType;
//     GameBoard* gb = (GameBoard*)data;
//     uint32_t pipIndex = e->user.code;
//     log_debug("pip %d clicked", pipIndex);
//     Checker* checker = getTopCheckerOnPip(gb, pipIndex);
//     if (checker != NULL) {
//         moveCheckerIfPossible(gb, checker);
//     }

// }

// void confirmIfPossible(GameBoard* gb) {
//     if (gb->state == LIGHT_CONFIRM) {
//         updateGameState(gb, DARK_DICE_ROLL);
//     }
//     else if (gb->state == DARK_CONFIRM) {
//         updateGameState(gb, LIGHT_DICE_ROLL);
//     }
// }

// void undoIfPossible(GameBoard* gb) {
//     if (gb->state == LIGHT_MOVE_TWO || gb->state == LIGHT_CONFIRM) {
//         loadCheckerState(gb);
//         updateGameState(gb, LIGHT_MOVE_ONE);
//     }
//     else if (gb->state == DARK_MOVE_TWO || gb->state == DARK_CONFIRM) {
//         loadCheckerState(gb);
//         updateGameState(gb, DARK_MOVE_ONE);
//     }
// }

// void handleButtonClick(uint32_t eventType, SDL_Event* e, void* data) {
//     (void)eventType;
//     GameBoard* gb = (GameBoard*)data;
//     GameButtonType btnType = e->user.code;
//     if (btnType == CONFIRM_BTN) {
//         log_debug("confirm btn clicked");
//         confirmIfPossible(gb);
//     }
//     else if (btnType == UNDO_BTN) {
//         log_debug("undo btn clicked");
//         undoIfPossible(gb);
//     }
// }

// void handleCheckerClick(uint32_t eventType, SDL_Event* e, void* data) {
//     (void)eventType;
//     GameBoard* gb = (GameBoard*)data;
//     Checker* checker = (Checker*)e->user.data1;
//     log_debug("checker on pip %d clicked", checker->pipIndex);
//     moveCheckerIfPossible(gb, checker);
// }


// void handleDiceClick(uint32_t eventType, SDL_Event* e, void* data) {
//     (void)eventType;
//     (void)e;
//     GameBoard* gb = (GameBoard*)data;
//     log_debug("got dice click event");
//     swapDiceIfPossible(gb);
//     rollDiceIfPossible(gb);
// }

// void handleKeyPress(uint32_t eventType, SDL_Event* e, void* data) {
//     (void)eventType;
//     GameBoard* gb = (GameBoard*)data;
//     if (e->key.keysym.sym == SDLK_SPACE) {
//         confirmIfPossible(gb);
//     }
//     else if (e->key.keysym.sym == SDLK_u) {
//         undoIfPossible(gb);
//     }
// }

// void rollDiceIfPossible(GameBoard* gb) {
//     if (gb->state == LIGHT_DICE_ROLL) {
//         gb->die1.value = generateRandomNumber(1, 6);
//         gb->die2.value = generateRandomNumber(1, 6);
//         updateGameState(gb, LIGHT_MOVE_ONE);
//     }
//     else if (gb->state == DARK_DICE_ROLL) {
//         gb->die1.value = generateRandomNumber(1, 6);
//         gb->die2.value = generateRandomNumber(1, 6);
//         updateGameState(gb, DARK_MOVE_ONE);
//     }
// }

void rollDice(GameBoard* gb) {
    gb->die1.value = generateRandomNumber(1, 6);
    gb->die2.value = generateRandomNumber(1, 6);
}

void swapDiceIfAllowed(GameBoard* gb) {
    if (gb->die1.uses == 0 && gb->die2.uses == 0) {
        int32_t index1 = gb->die1.index;
        gb->die1.index = gb->die2.index;
        gb->die2.index = index1;
    }
}

int32_t incrementMoveCount(GameBoard* gb) {
    if (DOUBLES_ROLLED(gb)) {
        if (gb->die1.uses < 2) {
            gb->die1.uses++;
            return (4 - gb->die1.uses);
        }
        if (gb->die2.uses < 2) {
            gb->die2.uses++;
            return (2 - gb->die2.uses);
        }
    }

    if (gb->die1.uses == 0) {
        gb->die1.uses++;
        return 1;
    }
    if (gb->die2.uses == 0) {
        gb->die2.uses++;
        return 0;
    }
    return 0;
}

void updateBoardForPlayerMove(GameBoard* gb) {
    gb->die1.uses = 0;
    gb->die2.uses = 0;
    gb->die1.animation = DICE_SWAP;
    gb->die2.animation = DICE_SWAP;
}

void updateBoardForDiceRoll(GameBoard* gb) {
    gb->die1.animation = DICE_MOVE;
    gb->die2.animation = DICE_MOVE;
    gb->confirm.visible = false;
    gb->undo.visible = false;
    if (gb->activePlayer == LIGHT) {
        gb->die1.side = 0;
        gb->die2.side = 0;
    }
    else {
        gb->die1.side = 1;
        gb->die2.side = 1;
    }
}

void updateBoardForConfirm(GameBoard* gb) {
    gb->confirm.visible = true;
    gb->undo.visible = true;
    if (gb->activePlayer == LIGHT) {
        gb->confirm.side = 0;
        gb->undo.side = 0;
    }
    else {
        gb->confirm.side = 1;
        gb->undo.side = 1;
    }
}


// uint32_t handleTimerEndPlayerTurnIfNoMoves(uint32_t interval, void* ctx) {
//     (void)interval;
//     GameBoard* gb = (GameBoard*)ctx;
//     Color player = NONE;
//     bool hasBothDiceLeft = false;

//     switch (gb->state)
//     {
//     case LIGHT_MOVE_ONE:
//         player = LIGHT;
//         hasBothDiceLeft = true;
//         break;
//     case LIGHT_MOVE_TWO:
//         player = LIGHT;
//         hasBothDiceLeft = false;
//         break;
//     case DARK_MOVE_ONE:
//         player = DARK;
//         hasBothDiceLeft = true;
//         break;
//     case DARK_MOVE_TWO:
//         player = DARK;
//         hasBothDiceLeft = false;
//         break;
//     default:
//         return 0;
//         break;
//     }

//     if (!playerHasMoves(gb, player, hasBothDiceLeft)) {
//         if (player == LIGHT) {
//             log_debug("LIGHT has no moves!");
//             if (hasBothDiceLeft) {
//                 updateGameState(gb, DARK_DICE_ROLL);
//             }
//             else {
//                 updateGameState(gb, LIGHT_CONFIRM);
//             }

//         }
//         else {
//             log_debug("DARK has no moves!");
//             if (hasBothDiceLeft) {
//                 updateGameState(gb, LIGHT_DICE_ROLL);
//             }
//             else {
//                 updateGameState(gb, DARK_CONFIRM);
//             }
//         }
//     }
//     return 0;
// }

void moveChecker(GameBoard* gb, Checker* c, int32_t amount) {
    int32_t pipIndex = c->pipIndex;
    int32_t nextPip = getNextPip(c, amount);

    // if we hit our opponents blot, bar the checker
    if (isPipOpponentBlot(gb, nextPip)) {
        Checker* blot = getTopCheckerOnPip(gb, nextPip);
        barChecker(gb, blot);
    }

    c->pipOffset = getNumCheckersOnPip(gb, nextPip);
    c->pipIndex = nextPip;
    log_debug("moved checker from pip %d to pip %d", pipIndex, nextPip);
}

void initCheckerSetup(void) {
    int32_t lightSetup[] = { 1, 1, 12, 12, 12, 12, 12, 17, 17, 17, 19, 19, 19, 19, 19 };
    //int32_t darkSetup[] = { 24, 24, 13, 13, 13, 13, 13, 8, 8, 8, 6, 6, 6, 6, 6 };
    int32_t darkSetup[] = { 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2 };

    int32_t pipIndex;
    for (int32_t i = 0; i < 15; i++) {
        pipIndex = lightSetup[i];
        FSM.gb.lightCheckers[i].pipOffset = getNumCheckersOnPip(&FSM.gb, pipIndex);
        FSM.gb.lightCheckers[i].pipIndex = pipIndex;
        FSM.gb.lightCheckers[i].color = LIGHT;
    }

    for (int32_t i = 0; i < 15; i++) {
        pipIndex = darkSetup[i];
        FSM.gb.darkCheckers[i].pipOffset = getNumCheckersOnPip(&FSM.gb, pipIndex);
        FSM.gb.darkCheckers[i].pipIndex = pipIndex;
        FSM.gb.darkCheckers[i].color = DARK;
    }
}

// void initEventCallbacks(GameBoard* gb) {
//     PipClickEventType = SDL_RegisterEvents(1);
//     CheckerClickEventType = SDL_RegisterEvents(1);
//     DiceClickEventType = SDL_RegisterEvents(1);
//     ButtonClickEventType = SDL_RegisterEvents(1);
//     Sage_registerEventCallback(PipClickEventType, handlePipClick, gb);
//     Sage_registerEventCallback(CheckerClickEventType, handleCheckerClick, gb);
//     Sage_registerEventCallback(DiceClickEventType, handleDiceClick, gb);
//     Sage_registerEventCallback(SDL_EVENT_KEY_DOWN, handleKeyPress, gb);
//     Sage_registerEventCallback(ButtonClickEventType, handleButtonClick, gb);
// }

void gameboard_init(void) {
    FSM.gb.die1 = (GameDie){ 1, 0, 1, DICE_NONE, false };
    FSM.gb.die2 = (GameDie){ 2, 1, 1, DICE_NONE, false };
    FSM.gb.confirm = (GameButton){ CONFIRM_BTN, false, 1 };
    FSM.gb.undo = (GameButton){ UNDO_BTN, false, 1 };
    FSM.gb.activePlayer = LIGHT;
    initCheckerSetup();
    createBoardSprites();
    createPipSprites();
    createDiceSprites(&FSM.gb.die1, &FSM.gb.die2);
    createButtonSprites(&FSM.gb.undo, &FSM.gb.confirm);
    for (int32_t i = 0; i < 15; i++) {
        createCheckerSprite(&FSM.gb.lightCheckers[i]);
        createCheckerSprite(&FSM.gb.darkCheckers[i]);
    }
}
