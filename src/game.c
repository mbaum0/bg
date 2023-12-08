/**
 * @file game.c
 * @author Michael Baumgarten
 * @brief GameManager implementation
 */
#include "game.h"

struct GameManager {
    MediaManager* mm;
    ViewManager* vm;
};

GameManager* GM_init(void) {
    GameManager* gm = malloc(sizeof(GameManager));
    init_events();
    gm->mm = MM_init();
    gm->vm = VM_init(gm->mm->renderer);
    return gm;
}

void GM_free(GameManager* gm) {
    VM_free(gm->vm);
    MM_free(gm->mm);
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

void createSprites(GameManager* gm){
    SDL_Rect src;
    uint32_t x, y;
    // Create the board
    src = (SDL_Rect){0, 0, 1560, 1080};
    x = (WINDOW_WIDTH - BOARD_WIDTH) / 2;
    y = (WINDOW_HEIGHT - BOARD_HEIGHT) / 2;
    VM_createSprite(gm->vm, gm->mm->textures.board, src, x, y, NULL, NULL);
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
