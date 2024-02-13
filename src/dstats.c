/**
 * @file dstats.c
 * @author Michael Baumgarten
 * @brief Debug stats for the game
 */
#include "dstats.h"

DStats stats = {0};

void DStats_update(ViewManager* vm, Snippet* snippet, void* data) {
  (void)vm;
  (void)data;
  extern DStats stats;
  char debugText[200];
  float fps, gameHeight, gameWidth, pipHeight, pipWidth, checkerHeight, checkerWidth, checkerRenderedHeight,
      checkerRenderedWidth;
  ;
  fps = stats.fps;
  gameHeight = stats.gameHeight;
  gameWidth = stats.gameWidth;
  pipHeight = stats.pipHeight;
  pipWidth = stats.pipWidth;
  checkerHeight = stats.checkerHeight;
  checkerWidth = stats.checkerWidth;
  checkerRenderedHeight = stats.checkerRenderedHeight;
  checkerRenderedWidth = stats.checkerRenderedWidth;
  sprintf(debugText,
          "FPS: %f\ngame height: %f\ngame width: %f\npipHeight: %f\npipWidth: %f\ncHeight: %f\ncWidth: %f\ncRenderW: "
          "%f\ncRenderH: %f",
          fps, gameHeight, gameWidth, pipHeight, pipWidth, checkerHeight, checkerWidth, checkerRenderedWidth,
          checkerRenderedHeight);
  Snippet_setText(snippet, debugText);
}

void DStats_create(void) {
  extern DStats stats;
  char debugText[200];
  // sprintf(debugText, "FPS: %f\ngame height: %f\ngame width: %f\npipHeight: %f\npipWidth: %f\ncHeight: %f\ncWidth: %f
  // ", stats.fps, stats.gameHeight, stats.gameWidth, stats.pipHeight, stats.pipWidth, stats.checkerHeight,
  // stats.checkerWidth);
  sprintf(debugText, "  ");
  SDL_Color color = {255, 255, 255, 255};
  TTF_Font* debugFont = Sage_loadFont("assets/VeraMono.ttf", 20);
  Snippet* snippet = Snippet_create(debugFont, color, 1, 1, 10, true);
  Sage_registerSnippet(snippet);
  Snippet_setText(snippet, debugText);
  Snippet_registerUpdateFn(snippet, DStats_update, NULL);
}
