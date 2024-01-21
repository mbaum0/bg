/**
 * @file dstats.c
 * @author Michael Baumgarten
 * @brief Debug stats for the game
 */
#include "dstats.h"

void DStats_update(ViewManager* vm, Snippet* snippet, void* data){
    (void)vm;
    (void)snippet;
    (void)data;
    // Sage* s = (Sage*)data;
    // char debugText[100];
    // int32_t fps = (int32_t)s->fps;
    // sprintf(debugText, "FPS: %d", fps);
    // Snippet_setText(snippet, debugText);   
}

void DStats_create(Sage* sage, GameBoard* board){
    (void)board;
    char debugText[100];
    sprintf(debugText, "FPS: %d", FPS);
    SDL_Color color = {255, 255, 255, 255 };
    TTF_Font* debugFont = Sage_loadFont(sage, "assets/VeraMono.ttf", 32);
    Snippet* snippet = Snippet_create(debugFont, color, 1, 1, 10, true);
    Sage_registerSnippet(sage, snippet);
    Snippet_setText(snippet, debugText);
    Snippet_registerUpdateFn(snippet, DStats_update, sage);
}
