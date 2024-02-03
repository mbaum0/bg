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

void rollDiceIfPossible(GameBoard* gb);
void swapDiceIfPossible(GameBoard* gb);
void updateGameState(GameBoard* gb, GameState state);
void moveCheckerIfPossible(GameBoard* gb, Checker* c);
bool isValidMove(GameBoard* gb, Color player, Checker* checker, int32_t amount);
Color getPipOwner(GameBoard* gb, int32_t pipIndex);
bool playerHasAllCheckersHome(GameBoard* gb, Color player);
bool playerHasMoves(GameBoard* gb, Color player);
void endPlayerTurnIfNoMoves(GameBoard* gb, Color player);
Checker* getTopCheckerOnPip(GameBoard* gb, int32_t pipIndex);
int32_t getNumCheckersOnPip(GameBoard* gb, int32_t pipIndex);

void updateGameState(GameBoard* gb, GameState state) {
    switch (state)
    {
    case LIGHT_DICE_ROLL:
        log_debug("Entered state: LIGHT_DICE_ROLL");
        gb->die1.side = 0;
        gb->die2.side = 0;
        gb->die1.animation = DICE_MOVE;
        gb->die2.animation = DICE_MOVE;
        break;
    case LIGHT_MOVE_ONE:
        log_debug("Entered state: LIGHT_MOVE_ONE");
        gb->die1.animation = DICE_SWAP;
        gb->die2.animation = DICE_SWAP;
        endPlayerTurnIfNoMoves(gb, LIGHT);
        break;
    case LIGHT_MOVE_TWO:
        log_debug("Entered state: LIGHT_MOVE_TWO");
        endPlayerTurnIfNoMoves(gb, LIGHT);
        break;
    case DARK_DICE_ROLL:
        log_debug("Entered state: DARK_DICE_ROLL");
        gb->die1.side = 1;
        gb->die2.side = 1;
        gb->die1.animation = DICE_MOVE;
        gb->die2.animation = DICE_MOVE;
        break;
    case DARK_MOVE_ONE:
        log_debug("Entered state: DARK_MOVE_ONE");
        gb->die1.animation = DICE_SWAP;
        gb->die2.animation = DICE_SWAP;
        endPlayerTurnIfNoMoves(gb, DARK);
        break;
    case DARK_MOVE_TWO:
        log_debug("Entered state: DARK_MOVE_TWO");
        endPlayerTurnIfNoMoves(gb, DARK);
        break;
    default:
        break;
    }
    gb->state = state;
}

Color getPipOwner(GameBoard* gb, int32_t pipIndex) {
    if (pipIndex == DARK_HOME) {
        return DARK;
    }
    if (pipIndex == LIGHT_HOME) {
        return LIGHT;
    }
    Checker* c = getTopCheckerOnPip(gb, pipIndex);
    if (c == NULL) {
        return NONE;
    }
    int32_t numCheckers = getNumCheckersOnPip(gb, pipIndex);
    if (numCheckers == 1) {
        return BLOT;
    }
    if (c->color == DARK) {
        return DARK;
    }
    if (c->color == LIGHT) {
        return LIGHT;
    }
    return NONE;
}

/**
 * @brief Returns true if all of the players checkers are home
 */
bool playerHasAllCheckersHome(GameBoard* gb, Color player) {
    Checker checker;
    for (int32_t i = 0; i < 15; i++) {
        if (player == LIGHT) {
            checker = gb->lightCheckers[i];
        }
        else {
            checker = gb->darkCheckers[i];
        }

        if (player == LIGHT && checker.pipIndex > LIGHT_HOME_START) {
            return false;
        }
        if (player == DARK && checker.pipIndex < DARK_HOME_START) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Returns true if the player has any valid moves
 *
 * @param gb
 * @return true
 * @return false
 */
bool playerHasMoves(GameBoard* gb, Color player) {
    bool allCheckersHome = playerHasAllCheckersHome(gb, player);
    int32_t direction = (player == LIGHT) ? 1 : -1;
    for (int32_t i = 0; i < 25;i++) {
        if (getPipOwner(gb, i) == player) {
            int32_t newIndex = i + (gb->die1.value * direction);
            newIndex = CAP_PIP_INDEX(newIndex);

            if (newIndex == PLAYER_HOME(player)) {
                // if this is the players home, they must have all checkers home
                if (allCheckersHome) {
                    return true;
                }
            }
            else {
                Color newIndexOwner = getPipOwner(gb, newIndex);
                if (newIndexOwner != OPPONENT_COLOR(player) && newIndexOwner != BLOT) {
                    return true;
                }
            }
            // try the other die
            newIndex = i + (gb->die2.value * direction);
            newIndex = CAP_PIP_INDEX(newIndex);

            if (newIndex == PLAYER_HOME(player)) {
                if (allCheckersHome) {
                    return true;
                }
            }
            else {
                Color newIndexOwner = getPipOwner(gb, newIndex);
                if (newIndexOwner != OPPONENT_COLOR(player) && newIndexOwner != BLOT) {
                    return true;
                }
            }

        }
    }
    return false;
}

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

// returns true if a move is valid
bool isValidMove(GameBoard* gb, Color player, Checker* c, int32_t amount) {
    if (c->color != player) {
        return false;
    }
    int32_t direction = (c->color == LIGHT) ? 1 : -1;
    int32_t newPipIndex = c->pipIndex + (amount * direction);

    Color newOwner = getPipOwner(gb, newPipIndex);
    if (newOwner == player || newOwner == NONE || newOwner == BLOT) {
        return true;
    }
    return false;
}

// return whether the move was successful
bool moveChecker(GameBoard* gb, int32_t pipIndex, int32_t amount) {
    // get the top checker on the pip
    Checker* c = NULL;
    int32_t topIndex = -1;

    if (pipIndex == LIGHT_HOME || pipIndex == DARK_HOME) {
        return false;
    }

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
        return false;
    }

    // move the checker
    int32_t direction = (c->color == LIGHT) ? 1 : -1;
    amount *= direction;
    int32_t newIndex = c->pipIndex + amount;
    newIndex = CAP_PIP_INDEX(newIndex);

    Color pipOwner = getPipOwner(gb, newIndex);
    if (pipOwner == OPPONENT_COLOR(c->color) || pipOwner == BLOT) {
        return false;
    }

    c->pipOffset = gb->pipCounts[newIndex];
    c->pipIndex = newIndex;
    gb->pipCounts[newIndex]++;
    gb->pipCounts[pipIndex]--;
    log_debug("moved checker from pip %d to pip %d", pipIndex, c->pipIndex);
    return true;
}

void handlePipClick(uint32_t eventType, SDL_Event* e, void* data) {
    (void)eventType;
    GameBoard* gb = (GameBoard*)data;
    uint32_t pipIndex = e->user.code;
    log_debug("pip %d clicked", pipIndex);
    Checker* checker = getTopCheckerOnPip(gb, pipIndex);
    if (checker != NULL) {
        moveCheckerIfPossible(gb, checker);
    }

}

void handleCheckerClick(uint32_t eventType, SDL_Event* e, void* data) {
    (void)eventType;
    GameBoard* gb = (GameBoard*)data;
    Checker* checker = (Checker*)e->user.data1;
    log_debug("checker on pip %d clicked", checker->pipIndex);
    moveCheckerIfPossible(gb, checker);
}



void handleDiceClick(uint32_t eventType, SDL_Event* e, void* data) {
    (void)eventType;
    (void)e;
    GameBoard* gb = (GameBoard*)data;
    log_debug("got dice click event");
    swapDiceIfPossible(gb);
    rollDiceIfPossible(gb);
}

void rollDiceIfPossible(GameBoard* gb) {
    if (gb->state == LIGHT_DICE_ROLL) {
        gb->die1.value = generateRandomNumber(1, 6);
        gb->die2.value = generateRandomNumber(1, 6);
        updateGameState(gb, LIGHT_MOVE_ONE);
    }
    else if (gb->state == DARK_DICE_ROLL) {
        gb->die1.value = generateRandomNumber(1, 6);
        gb->die2.value = generateRandomNumber(1, 6);
        updateGameState(gb, DARK_MOVE_ONE);
    }
}

void swapDiceIfPossible(GameBoard* gb) {
    if (gb->state == LIGHT_MOVE_ONE || gb->state == DARK_MOVE_ONE) {
        int32_t index1 = gb->die1.index;
        gb->die1.index = gb->die2.index;
        gb->die2.index = index1;
    }
}

/**
 * @brief Skip to next player's dice roll if the player has no moves
 */
void endPlayerTurnIfNoMoves(GameBoard* gb, Color player) {
    if (!playerHasMoves(gb, player)) {
        if (player == LIGHT){
            updateGameState(gb, DARK_DICE_ROLL);
        } else {
            updateGameState(gb, LIGHT_DICE_ROLL);
        }
    }
}

void moveCheckerIfPossible(GameBoard* gb, Checker* c) {
    int32_t pipIndex = c->pipIndex;
    int32_t firstDie = (gb->die1.index == 0) ? gb->die1.value : gb->die2.value;
    int32_t secondDie = (gb->die2.index == 1) ? gb->die2.value : gb->die1.value;
    if (c->color == LIGHT) {
        if (gb->state == LIGHT_MOVE_ONE) {
            if (moveChecker(gb, pipIndex, firstDie)) {
                updateGameState(gb, LIGHT_MOVE_TWO);
            }

        }
        else if (gb->state == LIGHT_MOVE_TWO) {
            if (moveChecker(gb, pipIndex, secondDie)) {
                updateGameState(gb, DARK_DICE_ROLL);
            }

        }
    }
    else {
        if (gb->state == DARK_MOVE_ONE) {
            if (moveChecker(gb, pipIndex, firstDie)) {
                updateGameState(gb, DARK_MOVE_TWO);
            }

        }
        else if (gb->state == DARK_MOVE_TWO) {
            if (moveChecker(gb, pipIndex, secondDie)) {
                updateGameState(gb, LIGHT_DICE_ROLL);
            }

        }
    }
}


void initCheckerSetup(GameBoard* gb) {
    int32_t lightSetup[] = { 1, 1, 12, 12, 12, 12, 12, 17, 17, 17, 19, 19, 19, 19, 19 };
    int32_t darkSetup[] = { 24, 24, 13, 13, 13, 13, 13, 8, 8, 8, 6, 6, 6, 6, 6 };

    int32_t pipIndex;
    for (int32_t i = 0; i < 15; i++) {
        pipIndex = lightSetup[i];
        gb->lightCheckers[i].pipIndex = pipIndex;
        gb->lightCheckers[i].pipOffset = gb->pipCounts[pipIndex];
        gb->lightCheckers[i].color = LIGHT;
        gb->pipCounts[pipIndex]++;
    }

    for (int32_t i = 0; i < 15; i++) {
        pipIndex = darkSetup[i];
        gb->darkCheckers[i].pipIndex = pipIndex;
        gb->darkCheckers[i].pipOffset = gb->pipCounts[pipIndex];
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
    gb->die1 = (GameDie){ 1, 0, 1, DICE_NONE };
    gb->die2 = (GameDie){ 2, 1, 1, DICE_NONE };
    initCheckerSetup(gb);
    createBoardSprites();
    createPipSprites();
    createDiceSprites(&gb->die1, &gb->die2);
    for (int32_t i = 0; i < 15; i++) {
        createCheckerSprite(&gb->lightCheckers[i]);
        createCheckerSprite(&gb->darkCheckers[i]);
    }
    initEventCallbacks(gb);
    updateGameState(gb, DARK_DICE_ROLL);
    return gb;
}
