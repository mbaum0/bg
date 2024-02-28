/**
 * @file score.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief 
 */
#include "score.h"
#include "fsm.h"
#include "snippet.h"
#include "game.h"
#include "util.h"
#include "sage.h"

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
    Sint32 lightScore = getPlayerScore(&gb, DARK);
    char text[200];
    sprintf(text, "%d", lightScore);
    Snippet_setText(snippet, text);
}

void createScore(void){
    char darkScore[100];
    char lightScore[100];
    sprintf(darkScore, " ");
    sprintf(lightScore, " ");
    SageFont* sf = Sage_loadBitmapFont( "assets/NotoSans-Medium_0.png", "assets/NotoSans-Medium.fnt");

    SDL_Color fontColor = {0, 0, 0, 255};
    Snippet* lightSnippet = Snippet_create(sf, fontColor, SCORE_RIGHT_X, SCORE_Y, Z_DEBUG, true);
    Snippet* darkSnippet = Snippet_create(sf, fontColor, SCORE_LEFT_X, SCORE_Y, Z_DEBUG, true);
    lightSnippet->useViewport = true;
    darkSnippet->useViewport = true;
    Sage_registerSnippet(lightSnippet);
    Sage_registerSnippet(darkSnippet);
    Snippet_setText(lightSnippet, lightScore);
    Snippet_setText(darkSnippet, darkScore);
    Snippet_registerUpdateFn(lightSnippet, lightScoreUpdate, NULL);
    Snippet_registerUpdateFn(darkSnippet, darkScoreUpdate, NULL);
}
