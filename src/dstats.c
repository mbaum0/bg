/**
 * @file dstats.c
 * @author Michael Baumgarten
 * @brief Debug stats for the game
 */
#include "dstats.h"

DStats stats = {0};

void DStats_update(ViewManager* vm, Snippet* snippet, void* data){
    (void)vm;
    (void)data;
    extern DStats stats;
    char debugText[200];
    float fps, gameHeight, gameWidth, pipHeight, pipWidth, checkerHeight, checkerWidth;
    fps = stats.fps;
    gameHeight = stats.gameHeight;
    gameWidth = stats.gameWidth;
    pipHeight = stats.pipHeight;
    pipWidth = stats.pipWidth;
    checkerHeight = stats.checkerHeight;
    checkerWidth = stats.checkerWidth;
    sprintf(debugText, "FPS: %f\ngame height: %f\ngame width: %f\npipHeight: %f\npipWidth: %f\ncheckerHeight: %f\ncheckerWidth: %f ", fps, gameHeight, gameWidth, pipHeight, pipWidth, checkerHeight, checkerWidth);
    Snippet_setText(snippet, debugText);   
}

void DStats_create(Sage* sage){
    extern DStats stats;
    char debugText[200];
    sprintf(debugText, "FPS: %f\ngame height: %f\ngame width: %f\npipHeight: %f\npipWidth: %f\ncheckerHeight: %f\ncheckerWidth: %f ", stats.fps, stats.gameHeight, stats.gameWidth, stats.pipHeight, stats.pipWidth, stats.checkerHeight, stats.checkerWidth);
    SDL_Color color = {255, 255, 255, 255 };
    TTF_Font* debugFont = Sage_loadFont(sage, "assets/VeraMono.ttf", 20);
    Snippet* snippet = Snippet_create(debugFont, color, 1, 1, 10, true);
    Sage_registerSnippet(sage, snippet);
    Snippet_setText(snippet, debugText);
    Snippet_registerUpdateFn(snippet, DStats_update, NULL);
}
