/**
 * @file dstats.c
 * @author Michael Baumgarten
 * @brief Debug stats for the game
 */
#include "dstats.h"
#include "fsm.h"
#include "game.h"

extern FiniteStateMachine FSM;

void DStats_update(ViewManager* vm, Snippet* snippet, void* data) {
    (void)vm;
    (void)data;
    GameBoard gb = FSM.gb;
    int32_t lightScore = getPlayerScore(&gb, LIGHT);
    int32_t darkScore = getPlayerScore(&gb, DARK);
    char debugText[200];
    sprintf(debugText, "lightScore: %d\ndarkScore: %d\n", lightScore, darkScore);
    Snippet_setText(snippet, debugText);
}

void DStats_create(void) {
    char debugText[200];
    sprintf(debugText, "  ");
    SDL_Color color = {255, 255, 255, 255};
    TTF_Font* debugFont = Sage_loadFont("assets/VeraMono.ttf", 20);
    Snippet* snippet = Snippet_create(debugFont, color, 1, 1, 10, true);
    Sage_registerSnippet(snippet);
    Snippet_setText(snippet, debugText);
    Snippet_registerUpdateFn(snippet, DStats_update, NULL);
}
