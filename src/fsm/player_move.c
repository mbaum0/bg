/**
 * @file player_move.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#include "ai.h"
#include "fsm.h"
#include "game.h"
#include <stdio.h>

uint32_t timerEndPlayerTurnIfNoMoves(uint32_t interval, void* ctx) {
  (void)interval;
  GameBoard* gb = (GameBoard*)ctx;
  GameDie* die1 = FIRST_DIE(gb);
  bool hasBothDiceLeft = (die1->uses == 0);

  if (!playerHasMoves(gb)) {
    log_debug("Active player has no moves available");
    if (hasBothDiceLeft) {
      gb->activePlayer = OPPONENT_COLOR(gb->activePlayer);
      fsm_transition(WAIT_FOR_ROLL_STATE);
    } else {
      fsm_transition(MOVE_CONFIRM_STATE);
    }
  }
  return 0;
}

void doPlayerMove(GameBoard* gb, int32_t pipIndex) {
  int32_t movesLeft = 0;
  Checker* c = getTopCheckerOnPip(gb, pipIndex);
  if (c == NULL) {
    return;
  }

  int32_t dieValue = getNextDieValue(gb);
  GameMove gm = {c->color, c->pipIndex, dieValue};

  if (isValidMove(gb, gm)) {
    moveChecker(gb, gm);
    movesLeft = incrementMoveCount(gb);
  } else {
    return;
  }

  if (movesLeft == 0) {
    fsm_transition(MOVE_CONFIRM_STATE);
  }
  SDL_AddTimer(1000, timerEndPlayerTurnIfNoMoves, gb);
}

void doDiceSwap(GameBoard* gb) {
  swapDiceIfAllowed(gb);
}

void player_move_state(FiniteStateMachine* fsm) {
  GameBoard* gb = &fsm->gb;
  FSMEvent event;
  while (fsm_dequeue_event(&event)) {
    if (event.etype == PIP_CLICKED_EVENT) {
      uint32_t pipIndex = (uintptr_t)event.ctx;
      doPlayerMove(gb, pipIndex);
    }

    if (event.etype == DICE_CLICKED_EVENT) {
      doDiceSwap(gb);
    }
  }
}

void player_move_init_state(FiniteStateMachine* fsm) {
  GameBoard* gb = &fsm->gb;
  log_debug("Entered state: PLAYER_TURN");
  updateBoardForPlayerMove(gb);
  saveCheckerState(gb);
  SDL_AddTimer(1000, timerEndPlayerTurnIfNoMoves, gb);

  // this can be removed later
  GameMoveSequence best = findBestMoveSequence(gb, gb->activePlayer);
  char moveStr[100];
  moveSequenceToString(best, moveStr);
  log_debug("%s", moveStr);
}
