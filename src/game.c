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

bool processInput(void) {
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
                free(spe);
            }
            else if (event.type == SPRITE_HOVER_EVENT) {
                SpriteHoverEvent* she = (SpriteHoverEvent*)event.user.data1;
                log_debug("Sprite hover change! %d %d", she->sprite_id, she->hovered);
                free(she);
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
    VM_createSprite(gm->vm, gm->mm->textures.board, src, x, y, Z_BOARD, NULL, NULL);

    // Create the checkers
    for (int i = 0; i < 30; i++){
        Checker* checker = &gm->board->checkers[i];
        uint32_t colorOffset = (checker->player == P_Light) ? 0 : CHECKER_SIZE;
        src = (SDL_Rect){colorOffset, 0, CHECKER_SIZE, CHECKER_SIZE};
        x = GET_CHECKER_X(checker->location);
        y = GET_CHECKER_Y(checker->location, checker->index);
        VM_createSprite(gm->vm, gm->mm->textures.checker, src, x, y, Z_CHECKER, updateCheckerSprite, checker);
    }
}

void delayFrame(uint32_t frameStart) {
    uint32_t frameTime = SDL_GetTicks() - frameStart;
    if (frameTime < MS_PER_FRAME) {
        SDL_Delay(MS_PER_FRAME - frameTime);
    }
}

void GM_run(GameManager* gm) {
    createSprites(gm);

    while(!processInput()){
        uint32_t frameStart = SDL_GetTicks();
        VM_draw(gm->vm);
        delayFrame(frameStart);
    }
}
