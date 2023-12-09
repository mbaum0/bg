/**
 * @file dstats.c
 * @author Michael Baumgarten
 * @brief Debug stats for the game
 */
#include "dstats.h"

void updateDstatsSnippet(Snippet* snippet, void* data){
    GameBoard* board = (GameBoard*)data;
    char lastClickedText[100];
    sprintf(lastClickedText, "Clicked loc: %d\nClicked sprite: %d", board->clickedLocation, board->clickedSprite);
    Snippet_setText(snippet, lastClickedText);
}

void DStats_createSnippet(MediaManager* mm, ViewManager* vm, GameBoard* board){
    char lastClickedText[100];
    sprintf(lastClickedText, "Clicked loc: %d\nClicked sprite: %d", board->clickedLocation, board->clickedSprite);
    SDL_Color color = { 255, 0, 0, 255 };
    VM_createSnippet(vm, mm->fonts.debug, color, lastClickedText, 0, 0, Z_DEBUGTEXT, updateDstatsSnippet, board);
}
