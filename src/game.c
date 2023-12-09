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
                gm->board->clickedSprite = spe->sprite_id;
                free(spe);
            } else if (event.type == LOCATION_CLICK_EVENT) {
                LocationClickEvent* lce = (LocationClickEvent*)event.user.data1;
                log_debug("Location click! %d", lce->location);
                gm->board->clickedLocation = lce->location;
                Board_moveIfPossible(gm->board, gm->board->clickedLocation, 1);
                free(lce);
            }
            break;
        }
    }
    return false;
}

void delayFrame(uint32_t frameStart) {
    uint32_t frameTime = SDL_GetTicks() - frameStart;
    if (frameTime < MS_PER_FRAME) {
        SDL_Delay(MS_PER_FRAME - frameTime);
    }
}

void createSprites(GameManager* gm){
    // Create background sprite
    SDL_Rect src;
    uint32_t x, y;
    src = (SDL_Rect){ 0, 0, 1560, 1080 };
    x = WINDOW_SIDE_OFFSET;
    y = WINDOW_TOP_OFFSET;
    VM_createSprite(gm->vm, gm->mm->textures.board, src, x, y, Z_BOARD, false, NULL, NULL, NULL, NULL);

    // Create pip sprites
    for (int i = 0; i < 24; i++){
        Pip_createSprite(i, gm->mm, gm->vm);
    }

    // Create checker sprites
    for (int i = 0; i < 30; i++){
        Checker_createSprite(&gm->board->checkers[i], gm->mm, gm->vm);
    }

    // Create debug stats
    DStats_createSnippet(gm->mm, gm->vm, gm->board);
}

void GM_run(GameManager* gm) {
    createSprites(gm);

    while(!processInput(gm)){
        uint32_t frameStart = SDL_GetTicks();
        VM_draw(gm->vm);
        delayFrame(frameStart);
    }
}
