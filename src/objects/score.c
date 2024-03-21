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

void lightScoreUpdate(ViewManager* vm, Snippet* snippet, void* data) {
    (void)vm;
    (void)data;
    GameBoard gb = FSM.gb;
    Sint32 lightScore = getPlayerScore(&gb, LIGHT);
    char text[200];
    sprintf(text, "%d", lightScore);
    Snippet_setText(snippet, text);
}

void darkScoreUpdate(ViewManager* vm, Snippet* snippet, void* data) {
    (void)vm;
    (void)data;
    GameBoard gb = FSM.gb;
    Sint32 darkScore = getPlayerScore(&gb, DARK);
    char text[200];
    sprintf(text, "%d", darkScore);
    Snippet_setText(snippet, text);
}

void lightWonRoundUpdate(ViewManager* vm, Snippet* snippet, void* data) {
    (void)vm;
    (void)data;
    GameBoard gb = FSM.gb;
    char text[200];
    sprintf(text, "%d / %d", gb.dialog.roundsWon, gb.dialog.maxGames);
    Snippet_setText(snippet, text);
}

void darkWonRoundUpdate(ViewManager* vm, Snippet* snippet, void* data) {
    (void)vm;
    (void)data;
    GameBoard gb = FSM.gb;
    char text[200];
    sprintf(text, "%d / %d", gb.dialog.roundsLost, gb.dialog.maxGames);
    Snippet_setText(snippet, text);
}

void createScore(void) {
    char darkScore[100];
    char lightScore[100];
    sprintf(darkScore, " ");
    sprintf(lightScore, " ");
    SageFont* sf = Sage_loadBitmapFont("assets/NotoSans-Medium_0.png", "assets/NotoSans-Medium.fnt",
                                       SCORE_FONT_SRC_SIZE, SCORE_FONT_SIZE);

    SDL_Color fontColor = {0, 0, 0, 255};
    Snippet* lightScoreSnippet = Snippet_create(sf, fontColor, SCORE_RIGHT_X, SCORE_Y, Z_DEBUG, true);
    Snippet* darkScoreSnippet = Snippet_create(sf, fontColor, SCORE_LEFT_X, SCORE_Y, Z_DEBUG, true);
    Snippet* lightWonRoundsSnippet = Snippet_create(sf, fontColor, SCORE_RIGHT_X, MATCH_SCORE_Y, Z_DEBUG, true);
    Snippet* darkWonRoundsSnippet = Snippet_create(sf, fontColor, SCORE_LEFT_X, MATCH_SCORE_Y, Z_DEBUG, true);

    lightScoreSnippet->useViewport = true;
    lightScoreSnippet->centerAlign = true;
    darkScoreSnippet->useViewport = true;
    darkScoreSnippet->centerAlign = true;
    lightWonRoundsSnippet->useViewport = true;
    lightWonRoundsSnippet->centerAlign = true;
    darkWonRoundsSnippet->useViewport = true;
    darkWonRoundsSnippet->centerAlign = true;
    Sage_registerSnippet(lightScoreSnippet);
    Sage_registerSnippet(darkScoreSnippet);
    Sage_registerSnippet(lightWonRoundsSnippet);
    Sage_registerSnippet(darkWonRoundsSnippet);
    Snippet_setText(lightScoreSnippet, lightScore);
    Snippet_setText(darkScoreSnippet, darkScore);
    Snippet_setText(lightWonRoundsSnippet, lightScore);
    Snippet_setText(darkWonRoundsSnippet, darkScore);
    Snippet_registerUpdateFn(lightScoreSnippet, lightScoreUpdate, NULL);
    Snippet_registerUpdateFn(darkScoreSnippet, darkScoreUpdate, NULL);
    Snippet_registerUpdateFn(lightWonRoundsSnippet, lightWonRoundUpdate, NULL);
    Snippet_registerUpdateFn(darkWonRoundsSnippet, darkWonRoundUpdate, NULL);
}
