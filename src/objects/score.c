/**
 * @file score.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#include "score.h"
#include "fsm.h"
#include "game.h"
#include "sage.h"
#include "snippet.h"
#include "util.h"

extern FiniteStateMachine FSM;

void p1ScoreUpdate(ViewManager* vm, Snippet* snippet, void* data) {
    (void)vm;
    (void)data;
    GameBoard gb = FSM.gb;
    Sint32 p1Score = getPlayerScore(&gb, PLAYER_ONE);
    char text[200];
    sprintf(text, "%d", p1Score);
    Snippet_setText(snippet, text);
}

void p2ScoreUpdate(ViewManager* vm, Snippet* snippet, void* data) {
    (void)vm;
    (void)data;
    GameBoard gb = FSM.gb;
    Sint32 p2Score = getPlayerScore(&gb, PLAYER_TWO);
    char text[200];
    sprintf(text, "%d", p2Score);
    Snippet_setText(snippet, text);
}

void p1WonRoundUpdate(ViewManager* vm, Snippet* snippet, void* data) {
    (void)vm;
    (void)data;
    GameBoard gb = FSM.gb;
    char text[200];
    sprintf(text, "%d / %d", gb.dialog.roundsWon, gb.dialog.maxGames);
    Snippet_setText(snippet, text);
}

void p2WonRoundUpdate(ViewManager* vm, Snippet* snippet, void* data) {
    (void)vm;
    (void)data;
    GameBoard gb = FSM.gb;
    char text[200];
    sprintf(text, "%d / %d", gb.dialog.roundsLost, gb.dialog.maxGames);
    Snippet_setText(snippet, text);
}

void createScore(void) {
    char p1Score[100];
    char p2Score[100];
    sprintf(p1Score, " ");
    sprintf(p2Score, " ");
    SageFont* sf = Sage_loadBitmapFont("assets/NotoSans-Medium_0.png", "assets/NotoSans-Medium.fnt",
                                       SCORE_FONT_SRC_SIZE, SCORE_FONT_SIZE);

    SDL_Color fontColor = {0, 0, 0, 255};
    Snippet* p1ScoreSnippet = Snippet_create(sf, fontColor, SCORE_RIGHT_X, SCORE_Y, Z_DEBUG, true);
    Snippet* p2ScoreSnippet = Snippet_create(sf, fontColor, SCORE_LEFT_X, SCORE_Y, Z_DEBUG, true);
    Snippet* p1WonRoundsSnippet = Snippet_create(sf, fontColor, SCORE_RIGHT_X, MATCH_SCORE_Y, Z_DEBUG, true);
    Snippet* p2WonRoundsSnippet = Snippet_create(sf, fontColor, SCORE_LEFT_X, MATCH_SCORE_Y, Z_DEBUG, true);

    p1ScoreSnippet->useViewport = true;
    p1ScoreSnippet->centerAlign = true;
    p2ScoreSnippet->useViewport = true;
    p2ScoreSnippet->centerAlign = true;
    p1WonRoundsSnippet->useViewport = true;
    p1WonRoundsSnippet->centerAlign = true;
    p2WonRoundsSnippet->useViewport = true;
    p2WonRoundsSnippet->centerAlign = true;
    Sage_registerSnippet(p1ScoreSnippet);
    Sage_registerSnippet(p2ScoreSnippet);
    Sage_registerSnippet(p1WonRoundsSnippet);
    Sage_registerSnippet(p2WonRoundsSnippet);
    Snippet_setText(p1ScoreSnippet, p1Score);
    Snippet_setText(p2ScoreSnippet, p2Score);
    Snippet_setText(p1WonRoundsSnippet, p1Score);
    Snippet_setText(p2WonRoundsSnippet, p2Score);
    Snippet_registerUpdateFn(p1ScoreSnippet, p1ScoreUpdate, NULL);
    Snippet_registerUpdateFn(p2ScoreSnippet, p2ScoreUpdate, NULL);
    Snippet_registerUpdateFn(p1WonRoundsSnippet, p1WonRoundUpdate, NULL);
    Snippet_registerUpdateFn(p2WonRoundsSnippet, p2WonRoundUpdate, NULL);
}
