/**
 * @file game.c
 * @author Michael Baumgarten
 * @brief GameManager implementation
 */
#include "game.h"

struct GameManager {
    MediaManager* mm;
    ViewManager* vm;
    GameBoard* board;
};

GameManager* GM_init(void) {
    GameManager* gm = malloc(sizeof(GameManager));
    init_events();
    gm->mm = MM_init();
    gm->vm = VM_init(gm->mm->renderer);
    gm->board = Board_init();
    return gm;
}

void GM_free(GameManager* gm) {
    VM_free(gm->vm);
    MM_free(gm->mm);
    Board_free(gm->board);
    free(gm);
}

bool processInput(GameManager* gm) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            log_debug("Received SDL_QUIT event");
            return true;
        default:
            if (event.type == SPRITE_CLICK_EVENT) {
                SpriteClickEvent* spe = (SpriteClickEvent*)event.user.data1;
                log_debug("Sprite click! %d", spe->sprite_id);
                gm->board->lastClickedChecker = spe->sprite_id;
                free(spe);
            }
            break;
        }
    }
    return false;
}

void updateCheckerSprite(Sprite* sprite, void* data){
    Checker* checker = (Checker*)data;
    uint32_t x = GET_CHECKER_X(checker->location);
    uint32_t y = GET_CHECKER_Y(checker->location, checker->index);
    Sprite_setLocation(sprite, x, y);
}

void createSprites(GameManager* gm){
    SDL_Rect src;
    uint32_t x, y;
    // Create the board
    src = (SDL_Rect){0, 0, 1560, 1080};
    x = WINDOW_SIDE_OFFSET;
    y = WINDOW_TOP_OFFSET;
    VM_createSprite(gm->vm, gm->mm->textures.board, src, x, y, Z_BOARD, false, NULL, NULL);

    // Create the pips
    for (int i = 0; i < 24; i++){
        bool flip = (i < 12) ? true : false;
        uint32_t color = (i % 2 == 0) ? 0 : PIP_WIDTH;
        src = (SDL_Rect){color, 0, PIP_WIDTH, PIP_HEIGHT};
        x = GET_PIP_X(i);
        y = GET_PIP_Y(i);
        VM_createSprite(gm->vm, gm->mm->textures.pip, src, x, y, Z_PIP, flip, NULL, NULL);
    }

    // Create the checkers
    for (int i = 0; i < 30; i++){
        Checker* checker = &gm->board->checkers[i];
        uint32_t colorOffset = (checker->player == P_Light) ? 0 : CHECKER_SIZE;
        src = (SDL_Rect){colorOffset, 0, CHECKER_SIZE, CHECKER_SIZE};
        x = GET_CHECKER_X(checker->location);
        y = GET_CHECKER_Y(checker->location, checker->index);
        VM_createSprite(gm->vm, gm->mm->textures.checker, src, x, y, Z_CHECKER, false, updateCheckerSprite, checker);
    }
}

void updateDebugStats(Snippet* snippet, void* data){
    GameManager* gm = (GameManager*)data;
    char lastClickedText[100];
    sprintf(lastClickedText, "Last clicked: %d", gm->board->lastClickedChecker);
    Snippet_setText(snippet, lastClickedText);
}

void createDebugStats(GameManager* gm){
    char lastClickedText[100];
    sprintf(lastClickedText, "Last clicked: %d", 0);
    SDL_Color color = {255, 0, 0, 255};
    VM_createSnippet(gm->vm, gm->mm->fonts.debug, color, lastClickedText, 0, 0, Z_DEBUGTEXT, updateDebugStats, gm);
}

void delayFrame(uint32_t frameStart) {
    uint32_t frameTime = SDL_GetTicks() - frameStart;
    if (frameTime < MS_PER_FRAME) {
        SDL_Delay(MS_PER_FRAME - frameTime);
    }
}

void GM_run(GameManager* gm) {
    createSprites(gm);
    createDebugStats(gm);

    while(!processInput(gm)){
        uint32_t frameStart = SDL_GetTicks();
        VM_draw(gm->vm);
        delayFrame(frameStart);
    }
}
