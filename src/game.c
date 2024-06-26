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
#include "score.h"
#include "util.h"
#include <stdlib.h>

#define SETUP_MODE_NORMAL 1
#define SETUP_MODE_ALL_HOME 2
#define SETUP_MODE_AI_BARRED 3
#define SETUP_MODE_ALL_AI_HOME 4
#define SETUP_1_MOVE_FROM_WINNING 5

#define SETUP_MODE SETUP_MODE_NORMAL
#if SETUP_MODE == SETUP_MODE_ALL_HOME
Sint32 DARKSETUP[] = {19, 19, 19, 20, 20, 20, 21, 21, 21, 22, 22, 23, 23, 24, 24};
Sint32 LIGHTSETUP[] = {1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 5, 5, 6, 6};
#elif SETUP_MODE == SETUP_MODE_AI_BARRED
Sint32 DARKSETUP[] = {DARK_BAR, DARK_BAR, DARK_BAR, DARK_BAR, DARK_BAR, DARK_BAR, DARK_BAR, DARK_BAR,
                      DARK_BAR, DARK_BAR, DARK_BAR, DARK_BAR, DARK_BAR, DARK_BAR, DARK_BAR};
Sint32 LIGHTSETUP[] = {1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 5, 5, 6, 6};
#elif SETUP_MODE == SETUP_1_MOVE_FROM_WINNING
Sint32 DARKSETUP[] = {DARK_BAR, DARK_BAR, DARK_BAR, DARK_BAR, DARK_BAR, DARK_BAR, DARK_BAR, DARK_BAR,
                      DARK_BAR, DARK_BAR, DARK_BAR, DARK_BAR, DARK_BAR, DARK_BAR, DARK_BAR};
Sint32 LIGHTSETUP[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
#elif SETUP_MODE == SETUP_MODE_ALL_AI_HOME
Sint32 DARKSETUP[] = {19, 19, 19, 20, 20, 20, 21, 21, 21, 22, 22, 23, 23, 24, 24};
Sint32 LIGHTSETUP[] = {LIGHT_BAR, LIGHT_BAR, LIGHT_BAR, LIGHT_BAR, LIGHT_BAR, LIGHT_BAR, LIGHT_BAR, LIGHT_BAR,
                       LIGHT_BAR, LIGHT_BAR, LIGHT_BAR, LIGHT_BAR, LIGHT_BAR, LIGHT_BAR, LIGHT_BAR};
#else
Sint32 DARKSETUP[] = {1, 1, 12, 12, 12, 12, 12, 17, 17, 17, 19, 19, 19, 19, 19};
Sint32 LIGHTSETUP[] = {24, 24, 13, 13, 13, 13, 13, 8, 8, 8, 6, 6, 6, 6, 6};
#endif

Sint32 DARKSETUPNORMAL[] = {1, 1, 12, 12, 12, 12, 12, 17, 17, 17, 19, 19, 19, 19, 19};
Sint32 LIGHTSETUPNORMAL[] = {24, 24, 13, 13, 13, 13, 13, 8, 8, 8, 6, 6, 6, 6, 6};

extern FiniteStateMachine FSM;

void saveCheckerState(GameBoard* gb) {
    memcpy(gb->lightCheckersSave, gb->lightCheckers, sizeof(gb->lightCheckersSave));
    memcpy(gb->darkCheckersSave, gb->darkCheckers, sizeof(gb->darkCheckersSave));
}

void loadCheckerState(GameBoard* gb) {
    memcpy(gb->lightCheckers, gb->lightCheckersSave, sizeof(gb->lightCheckers));
    memcpy(gb->darkCheckers, gb->darkCheckersSave, sizeof(gb->darkCheckers));
}

bool isPipOpponentBlot(GameBoard* gb, Sint32 pipIndex) {
    Color opponent = OPPONENT_COLOR(gb->activePlayer);

    Sint32 numCheckers = getNumCheckersOnPip(gb, pipIndex);
    if (numCheckers == 1) {
        Checker* c = getTopCheckerOnPip(gb, pipIndex);
        return (c->color == opponent);
    }
    return false;
}

Color getRoundWinner(GameBoard* gb) {
    Uint32 lightScore = getPlayerScore(gb, LIGHT);
    Uint32 darkScore = getPlayerScore(gb, DARK);

    if (lightScore == 0) {
        return LIGHT;
    } else if (darkScore == 0) {
        return DARK;
    }
    return NONE;
}

bool playerHasCheckersOnBar(GameBoard* gb) {
    if (gb->activePlayer == LIGHT) {
        return getNumCheckersOnPip(gb, LIGHT_BAR) > 0;
    } else {
        return getNumCheckersOnPip(gb, DARK_BAR) > 0;
    }
}

Sint32 getNextDieValue(GameBoard* gb) {
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
    for (Sint32 i = 0; i < 15; i++) {
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

Sint32 getNumCheckersOnPip(GameBoard* gb, Sint32 pipIndex) {
    Sint32 count = 0;
    Checker* checker;
    for (Sint32 i = 0; i < 15; i++) {
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

Checker* getTopCheckerOnPip(GameBoard* gb, Sint32 pipIndex) {
    Checker* topChecker = NULL;
    Sint32 topIndex = -1;
    for (Sint32 i = 0; i < 15; i++) {
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

Sint32 getNextPip(Sint32 pipIndex, Color player, Sint32 amount) {
    Sint32 oldPip = pipIndex;
    Sint32 direction = (player == LIGHT) ? -1 : 1;
    Sint32 newPip;
    if (oldPip == LIGHT_BAR) {
        newPip = DARK_HOME + (direction * amount);
    } else if (oldPip == DARK_BAR) {
        newPip = LIGHT_HOME + (direction * amount);
    } else {
        newPip = oldPip + (direction * amount);
    }
    newPip = CAP_PIP_INDEX(newPip);
    return newPip;
}

// // returns true if a move is valid
bool isValidMove(GameBoard* gb, GameMove gm) {
    Checker* topChecker;
    Color player = gm.player;
    Sint32 oldPip = gm.srcPip;
    Sint32 newPip = getNextPip(oldPip, player, gm.amount);

    // player must be active
    if (player != gb->activePlayer) {
        return false;
    }

    // not valid if the src pip isn't owned by the player
    topChecker = getTopCheckerOnPip(gb, oldPip);
    if (topChecker == NULL || topChecker->color != player) {
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

    Sint32 numCheckers = getNumCheckersOnPip(gb, newPip);
    // not valid move if moving checker to opponent owned-pip
    if (numCheckers > 1) {
        topChecker = getTopCheckerOnPip(gb, newPip);
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

void rollDice(GameBoard* gb) {
    gb->die1.value = generateRandomNumber(1, 6);
    gb->die2.value = generateRandomNumber(1, 6);
}

bool haveDiceBeenUsed(GameBoard* gb) {
    return (gb->die1.uses != 0 || gb->die2.uses != 0);
}

void swapDiceIfAllowed(GameBoard* gb) {
    if (!haveDiceBeenUsed(gb) && playerHasMoves(gb)) {
        Sint32 index1 = gb->die1.index;
        gb->die1.index = gb->die2.index;
        gb->die2.index = index1;
    }
}

// returns true if the player has used all of their moves
Sint32 allMovesTaken(GameBoard* gb) {
    if (DOUBLES_ROLLED(gb)) {
        return (gb->die1.uses + gb->die2.uses) == 4;
    }
    return (gb->die1.uses + gb->die2.uses) == 2;
}

Sint32 incrementMoveCount(GameBoard* gb) {
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

void initBoardForPlayerMove(GameBoard* gb) {
    gb->die1.uses = 0;
    gb->die2.uses = 0;
    gb->die1.animation = DICE_SWAP;
    gb->die2.animation = DICE_SWAP;
    gb->roll.visible = false;
    gb->undo.visible = false;
    gb->confirm.visible = false;
    if (gb->activePlayer == gb->aiPlayer) {
        gb->die1.side = 0;
        gb->die2.side = 0;
        gb->nomoves.location = BTN_LEFT;
    } else {
        gb->die1.side = 1;
        gb->die2.side = 1;
        gb->nomoves.location = BTN_RIGHT;
    }
}

void updateBoardForPlayerMove(GameBoard* gb) {
    if (NUM_MOVES_MADE(gb) > 0 && (gb->activePlayer != gb->aiPlayer)) {
        gb->undo.visible = true;
    } else {
        gb->undo.visible = false;
    }
}

void initBoardForDiceRoll(GameBoard* gb) {
    gb->die1.uses = 0;
    gb->die2.uses = 0;
    gb->die1.animation = DICE_MOVE;
    gb->die2.animation = DICE_MOVE;
    gb->confirm.visible = false;
    gb->undo.visible = false;
    gb->nomoves.visible = false;

    gb->roll.visible = false;
    // if (gb->activePlayer == gb->aiPlayer) {
    //     gb->roll.visible = false;
    // } else {
    //     gb->roll.visible = true;
    // }

    if (gb->activePlayer == gb->aiPlayer) {
        gb->die1.side = 0;
        gb->die2.side = 0;
    } else {
        gb->die1.side = 1;
        gb->die2.side = 1;
    }
}

void initBoardForRollForFirst(GameBoard* gb) {
    gb->die1.uses = 0;
    gb->die2.uses = 0;
    gb->die1.animation = DICE_MOVE;
    gb->die2.animation = DICE_MOVE;
    gb->die1.side = 0;
    gb->die2.side = 1;
    gb->roll.visible = false;
}

void initBoardForInit(GameBoard* gb) {
    gb->roll.visible = true;
    if (gb->activePlayer == gb->aiPlayer) {
        gb->confirm.location = BTN_LEFT;
        gb->roll.location = BTN_LEFT;
    } else {
        gb->confirm.location = BTN_RIGHT;
        gb->roll.location = BTN_RIGHT;
    }
}

void initBoardForConfirm(GameBoard* gb) {
    gb->confirm.visible = true;
    gb->undo.visible = true;
    if (gb->activePlayer == gb->aiPlayer) {
        gb->confirm.location = BTN_LEFT;
        gb->roll.location = BTN_LEFT;
    } else {
        gb->confirm.location = BTN_RIGHT;
        gb->roll.location = BTN_RIGHT;
    }
}

bool playerHasMoves(GameBoard* gb) {
    Checker* c;
    GameDie* firstDie = FIRST_DIE(gb);
    GameDie* secondDie = SECOND_DIE(gb);
    GameMove gm;
    Sint32 usesPerDie = (DOUBLES_ROLLED(gb) ? 2 : 1);
    bool hasBothDiceLeft = (firstDie->uses < usesPerDie);
    for (Sint32 i = 0; i < 15; i++) {
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

void updateCheckerNeighbors(GameBoard* gb) {
    for (Sint32 i = 0; i < 15; i++) {
        Checker* checker = &gb->lightCheckers[i];
        checker->numNeighbors = getNumCheckersOnPip(&FSM.gb, checker->pipIndex);
        checker = &gb->darkCheckers[i];
        checker->numNeighbors = getNumCheckersOnPip(&FSM.gb, checker->pipIndex);
    }
}

void moveChecker(GameBoard* gb, GameMove gm) {
    Sint32 pipIndex = gm.srcPip;
    Sint32 nextPip = getNextPip(pipIndex, gm.player, gm.amount);

    // if we hit our opponents blot, bar the checker
    if (isPipOpponentBlot(gb, nextPip)) {
        Checker* blot = getTopCheckerOnPip(gb, nextPip);
        barChecker(gb, blot);
    }

    Checker* c = getTopCheckerOnPip(gb, pipIndex);

    c->pipOffset = getNumCheckersOnPip(gb, nextPip);
    c->pipIndex = nextPip;
    updateCheckerNeighbors(gb);
}

Sint32 getPlayerScore(GameBoard* gb, Color player) {
    Sint32 score = 0;
    Checker* checkers;
    if (player == LIGHT) {
        checkers = gb->lightCheckers;
    } else {
        checkers = gb->darkCheckers;
    }
    for (Sint32 i = 0; i < 15; i++) {
        Sint32 pipIndex = checkers[i].pipIndex;
        if (pipIndex == LIGHT_HOME || pipIndex == DARK_HOME) {
            // add nothing here because checker is home
            continue;
        }

        if (pipIndex == LIGHT_BAR || pipIndex == DARK_BAR) {
            score += 25;
        }

        if (player == DARK) {
            score += (DARK_HOME - pipIndex);
        } else {
            score += pipIndex;
        }
    }
    return score;
}

void deepCopy(GameBoard* dst, GameBoard* src) {
    memcpy(dst, src, sizeof(GameBoard));
}

void initCheckerSetup(Sint32* lightSetup, Sint32* darkSetup) {
    for (Sint32 i = 0; i < 15; i++) {
        FSM.gb.lightCheckers[i].pipIndex = LIGHT_HOME;
        FSM.gb.darkCheckers[i].pipIndex = DARK_HOME;
    }

    Sint32 pipIndex;
    for (Sint32 i = 0; i < 15; i++) {
        pipIndex = lightSetup[i];
        FSM.gb.lightCheckers[i].pipOffset = getNumCheckersOnPip(&FSM.gb, pipIndex);
        FSM.gb.lightCheckers[i].pipIndex = pipIndex;
        FSM.gb.lightCheckers[i].color = LIGHT;
    }

    for (Sint32 i = 0; i < 15; i++) {
        pipIndex = darkSetup[i];
        FSM.gb.darkCheckers[i].pipOffset = getNumCheckersOnPip(&FSM.gb, pipIndex);
        FSM.gb.darkCheckers[i].pipIndex = pipIndex;
        FSM.gb.darkCheckers[i].color = DARK;
    }
}

void gameboard_reset(GameBoard* gb) {
    (void)gb;
    FSM.gb.die1 = (GameDie){1, 0, 0, 0, DICE_NONE, false};
    FSM.gb.die2 = (GameDie){2, 1, 0, 0, DICE_NONE, false};
    FSM.gb.confirm = (GameButton){CONFIRM_BTN, false, BTN_RIGHT};
    FSM.gb.undo = (GameButton){UNDO_BTN, false, BTN_CENTER};
    FSM.gb.roll = (GameButton){ROLL_BTN, false, BTN_RIGHT};
    FSM.gb.dub = (GameButton){DUB_BTN, true, BTN_TOP};
    FSM.gb.nomoves = (GameButton){NM_BTN, false, BTN_RIGHT};
    FSM.gb.dialog.visible = false;
    FSM.gb.dialog.alpha = 0;
    initCheckerSetup(LIGHTSETUP, DARKSETUP);
    updateCheckerNeighbors(gb);
}

void gameboard_init(void) {
    FSM.gb.die1 = (GameDie){1, 0, 0, 0, DICE_NONE, false};
    FSM.gb.die2 = (GameDie){2, 1, 0, 0, DICE_NONE, false};
    FSM.gb.confirm = (GameButton){CONFIRM_BTN, false, BTN_RIGHT};
    FSM.gb.undo = (GameButton){UNDO_BTN, false, BTN_CENTER};
    FSM.gb.roll = (GameButton){ROLL_BTN, false, BTN_RIGHT};
    FSM.gb.dub = (GameButton){DUB_BTN, true, BTN_TOP};
    FSM.gb.nomoves = (GameButton){NM_BTN, false, BTN_RIGHT};
    FSM.gb.dialog = (Dialog){false, false, 0, false, 5, 0, 0, 0, 0};
    FSM.gb.activePlayer = LIGHT;
    FSM.gb.aiPlayer = DARK;
    FSM.gb.aiMoves = SDL_calloc(MAX_AI_SEQUENCES, sizeof(GameMoveSequence));
    initCheckerSetup(LIGHTSETUP, DARKSETUP);
    createBoardSprites();
    createDiceSprites(&FSM.gb.die1, &FSM.gb.die2);
    createButtonSprites(&FSM.gb.undo, &FSM.gb.confirm, &FSM.gb.roll, &FSM.gb.dub, &FSM.gb.nomoves);

    for (Sint32 i = 0; i < 24; i++) {
        FSM.gb.pips[i].alpha = 0;
        FSM.gb.pips[i].index = i + 1;
        FSM.gb.pips[i].color = PIP_BLUE;
        createPipSprite(&FSM.gb.pips[i]);
    }

    for (Sint32 i = 0; i < 15; i++) {
        createCheckerSprite(&FSM.gb.lightCheckers[i]);
        createCheckerSprite(&FSM.gb.darkCheckers[i]);
    }
    for (Sint32 i = 0; i < 15; i++) {
        Checker* checker = &FSM.gb.lightCheckers[i];
        checker->numNeighbors = getNumCheckersOnPip(&FSM.gb, checker->pipIndex);
        checker = &FSM.gb.darkCheckers[i];
        checker->numNeighbors = getNumCheckersOnPip(&FSM.gb, checker->pipIndex);
    }

    createScore();
    createDialogSprite(&FSM.gb.dialog);
}
