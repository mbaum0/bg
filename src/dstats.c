/**
 * @file dstats.c
 * @author Michael Baumgarten
 * @brief Debug stats for the game
 */
#include "dstats.h"
#include "fsm.h"
#include "game.h"
#include "sage.h"
#include "util.h"

extern FiniteStateMachine FSM;
extern Sage sage;

void clickDstats(ViewManager* vm, Snippet* snippet, void* object, void* context, Sint32 code) {
    (void)vm;
    (void)context;
    (void)code;
    (void)object;
    snippet->visible = !snippet->visible;
}

void DStats_update(ViewManager* vm, Snippet* snippet, void* data) {
    (void)vm;
    (void)data;
    char debugText[100];

    Uint64 ticksNow = SDL_GetPerformanceCounter();
    float timestep = (float)(ticksNow - sage.ticks) / SDL_GetPerformanceFrequency();
    sage.ticks = ticksNow;
    float fps = (1.0 / timestep);
    sprintf(debugText, "fps: %.1f", fps);
    Snippet_setText(snippet, debugText);
}

void DStats_create(void) {
    char debugText[200];
    sprintf(debugText, "  ");
    SDL_Color color = {255, 0, 0, 255};
    SageFont* sf = Sage_loadBitmapFont( "assets/NotoSans-Medium_0.png", "assets/NotoSans-Medium.fnt", DEBUG_FONT_SRC_SIZE, DEBUG_FONT_SIZE);

    Snippet* snippet = Snippet_create(sf, color, DEBUG_X, DEBUG_Y, Z_DEBUG, true);
    Sage_registerSnippet(snippet);
    Snippet_setText(snippet, debugText);
    Snippet_registerUpdateFn(snippet, DStats_update, NULL);
    Snippet_registerClickFn(snippet, clickDstats, NULL, NULL, 0);
}
