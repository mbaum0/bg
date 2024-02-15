/**
 * @file game.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#include "game.h"
#include "board.h"
#include "dice.h"
#include "fsm.h"
#include "pip.h"
#include "util.h"
#include <stdlib.h>

extern FiniteStateMachine FSM;

void saveCheckerState(GameBoard* gb) {
    memcpy(gb->lightCheckersSave, gb->lightCheckers, sizeof(gb->lightCheckersSave));
    memcpy(gb->darkCheckersSave, gb->darkCheckers, sizeof(gb->darkCheckersSave));
}

void loadCheckerState(GameBoard* gb) {
    memcpy(gb->lightCheckers, gb->lightCheckersSave, sizeof(gb->lightCheckers));
    memcpy(gb->darkCheckers, gb->darkCheckersSave, sizeof(gb->darkCheckers));
}

bool isPipOpponentBlot(GameBoard* gb, int32_t pipIndex) {
    Color opponent = OPPONENT_COLOR(gb->activePlayer);

    int32_t numCheckers = getNumCheckersOnPip(gb, pipIndex);
    if (numCheckers == 1) {
        Checker* c = getTopCheckerOnPip(gb, pipIndex);
        return (c->color == opponent);
    }
    return false;
}

bool playerHasCheckersOnBar(GameBoard* gb) {
    if (gb->activePlayer == LIGHT) {
        return getNumCheckersOnPip(gb, LIGHT_BAR) > 0;
    } else {
        return getNumCheckersOnPip(gb, DARK_BAR) > 0;
    }
}

int32_t getNextDieValue(GameBoard* gb) {
    GameDie die1 = *FIRST_DIE(gb);
    GameDie die2 = *SECOND_DIE(gb);
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
        } else {
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
            topIndex = c->pipOffset;
        }
        c = &gb->darkCheckers[i];
        if (c->pipIndex == pipIndex && c->pipOffset > topIndex) {
            topChecker = c;
            topIndex = c->pipOffset;
        }
    }
    return topChecker;
}

int32_t getNextPip(int32_t pipIndex, Color player, int32_t amount) {
    int32_t oldPip = pipIndex;
    int32_t direction = (player == LIGHT) ? 1 : -1;
    int32_t newPip;
    if (oldPip == LIGHT_BAR) {
        newPip = DARK_HOME + amount;
    } else if (oldPip == DARK_BAR) {
        newPip = LIGHT_HOME - amount;
    } else {
        newPip = oldPip + (direction * amount);
    }
    newPip = CAP_PIP_INDEX(newPip);
    return newPip;
}

// // returns true if a move is valid
bool isValidMove(GameBoard* gb, GameMove gm) {
    Color player = gm.player;
    int32_t oldPip = gm.srcPip;
    int32_t newPip = getNextPip(oldPip, player, gm.amount);

    if (player != gb->activePlayer) {
        return false;
    }

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
        if (topChecker->color == OPPONENT_COLOR(gm.player)) {
            return false;
        }
    }

    return true;
}

void barChecker(GameBoard* gb, Checker* c) {
    if (c->color == DARK) {
        c->pipOffset = getNumCheckersOnPip(gb, DARK_BAR);
        c->pipIndex = DARK_BAR;
    } else {
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
    GameDie* die1 = FIRST_DIE(gb);
    GameDie* die2 = SECOND_DIE(gb);
    if (DOUBLES_ROLLED(gb)) {
        if (die1->uses < 2) {
            die1->uses++;
            return (4 - die1->uses);
        }
        if (die2->uses < 2) {
            die2->uses++;
            return (2 - die2->uses);
        }
    }

    if (die1->uses == 0) {
        die1->uses++;
        return 1;
    }
    if (die2->uses == 0) {
        die2->uses++;
        return 0;
    }
    return 0;
}

void updateBoardForPlayerMove(GameBoard* gb) {
    gb->die1.uses = 0;
    gb->die2.uses = 0;
    gb->die1.animation = DICE_SWAP;
    gb->die2.animation = DICE_SWAP;
    gb->confirm.visible = false;
    gb->undo.visible = false;
    if (gb->activePlayer == LIGHT) {
        gb->die1.side = 0;
        gb->die2.side = 0;
    } else {
        gb->die1.side = 1;
        gb->die2.side = 1;
    }
}

void updateBoardForDiceRoll(GameBoard* gb) {
    gb->die1.uses = 0;
    gb->die2.uses = 0;
    gb->die1.animation = DICE_MOVE;
    gb->die2.animation = DICE_MOVE;
    gb->confirm.visible = false;
    gb->undo.visible = false;
    if (gb->activePlayer == LIGHT) {
        gb->die1.side = 0;
        gb->die2.side = 0;
    } else {
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
    } else {
        gb->confirm.side = 1;
        gb->undo.side = 1;
    }
}

bool playerHasMoves(GameBoard* gb) {
    Checker* c;
    GameDie* firstDie = FIRST_DIE(gb);
    GameDie* secondDie = SECOND_DIE(gb);
    GameMove gm;
    int32_t usesPerDie = (DOUBLES_ROLLED(gb) ? 2 : 1);
    bool hasBothDiceLeft = (firstDie->uses < usesPerDie);
    for (int32_t i = 0; i < 15; i++) {
        if (gb->activePlayer == DARK) {
            c = &gb->darkCheckers[i];
        } else {
            c = &gb->lightCheckers[i];
        }
        gm.srcPip = c->pipIndex;
        gm.player = gb->activePlayer;
        if (hasBothDiceLeft) {
            gm.amount = firstDie->value;
            if (isValidMove(gb, gm)) {
                return true;
            }
        }
        gm.amount = secondDie->value;
        if (isValidMove(gb, gm)) {
            return true;
        }
    }
    return false;
}

void moveChecker(GameBoard* gb, GameMove gm) {
    int32_t pipIndex = gm.srcPip;
    int32_t nextPip = getNextPip(pipIndex, gm.player, gm.amount);

    // if we hit our opponents blot, bar the checker
    if (isPipOpponentBlot(gb, nextPip)) {
        Checker* blot = getTopCheckerOnPip(gb, nextPip);
        barChecker(gb, blot);
    }

    Checker* c = getTopCheckerOnPip(gb, pipIndex);

    c->pipOffset = getNumCheckersOnPip(gb, nextPip);
    c->pipIndex = nextPip;
    log_debug("moved checker from pip %d to pip %d", pipIndex, nextPip);
}

int32_t getPlayerScore(GameBoard* gb, Color player) {
    int32_t score = 0;
    Checker* checkers;
    if (player == LIGHT) {
        checkers = gb->lightCheckers;
    } else {
        checkers = gb->darkCheckers;
    }
    for (int32_t i = 0; i < 15; i++) {
        int32_t pipIndex = checkers[i].pipIndex;
        if (pipIndex == LIGHT_HOME || pipIndex == DARK_HOME) {
            // add nothing here because checker is home
            continue;
        }

        if (pipIndex == LIGHT_BAR || pipIndex == DARK_BAR) {
            score += 25;
        }

        if (player == LIGHT) {
            score += (LIGHT_HOME - pipIndex);
        } else {
            score += pipIndex;
        }
    }
    return score;
}

void deepCopy(GameBoard* dst, GameBoard* src) {
    memcpy(dst, src, sizeof(GameBoard));
}

void initCheckerSetup(void) {
    int32_t lightSetup[] = {1, 1, 12, 12, 12, 12, 12, 17, 17, 17, 19, 19, 19, 19, 19};
    int32_t darkSetup[] = {24, 24, 13, 13, 13, 13, 13, 8, 8, 8, 6, 6, 6, 6, 6};
    // int32_t darkSetup[] = { 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2 };

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

void gameboard_init(void) {
    FSM.gb.die1 = (GameDie){1, 0, 0, 0, DICE_NONE};
    FSM.gb.die2 = (GameDie){2, 1, 0, 0, DICE_NONE};
    FSM.gb.confirm = (GameButton){CONFIRM_BTN, false, 1};
    FSM.gb.undo = (GameButton){UNDO_BTN, false, 1};
    FSM.gb.activePlayer = LIGHT;
    FSM.gb.aiPlayer = DARK;
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
