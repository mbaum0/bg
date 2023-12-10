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
        if (event.type == SDL_QUIT) {
            log_debug("Received SDL_QUIT event");
            return true;
        }
        else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_s:
                log_debug("Saving game to file");
                Board_export(gm->board, "save.txt");
                break;
            case SDLK_l:
                log_debug("Loading game from file");
                Board_import(gm->board, "save.txt");
                break;
            default:
                break;
            }
        }
        else {
            if (event.type == SPRITE_CLICK_EVENT) {
                SpriteClickEvent* spe = (SpriteClickEvent*)event.user.data1;
                log_debug("Sprite click! %d", spe->sprite_id);
                gm->board->clickedSprite = spe->sprite_id;
                free(spe);
            }
            else if (event.type == LOCATION_CLICK_EVENT) {
                LocationClickEvent* lce = (LocationClickEvent*)event.user.data1;
                log_debug("Location click! %d", lce->location);
                gm->board->clickedLocation = lce->location;
                free(lce);
            }
            else if (event.type == DIE_CLICK_EVENT) {
                DieClickEvent* dce = (DieClickEvent*)event.user.data1;
                log_debug("Die click! %d", dce->value);
                gm->board->diceRolled = true;
                free(dce);
            }
        }
    }
    return false;
}

void delayFrame(int32_t frameStart) {
    int32_t frameTime = SDL_GetTicks() - frameStart;
    if (frameTime < MS_PER_FRAME) {
        SDL_Delay(MS_PER_FRAME - frameTime);
    }
}

void createSprites(GameManager* gm) {
    // Create background sprite
    SDL_Rect src;
    int32_t x, y;
    src = (SDL_Rect){ 0, 0, 1560, 1080 };
    x = WINDOW_SIDE_OFFSET;
    y = WINDOW_TOP_OFFSET;
    VM_createSprite(gm->vm, gm->mm->textures.board, src, x, y, Z_BOARD, false, NULL, NULL, NULL, NULL);

    // Create pip sprites
    for (int i = 0; i < 24; i++) {
        Pip_createSprite(i, gm->mm, gm->vm);
    }

    // Create checker sprites
    for (int i = 0; i < 30; i++) {
        Checker_createSprite(&gm->board->checkers[i], gm->mm, gm->vm);
    }

    // Create dice sprites
    Dice_createSprite(&gm->board->die0, gm->mm, gm->vm);
    Dice_createSprite(&gm->board->die1, gm->mm, gm->vm);

    // Create debug stats
    DStats_createSnippet(gm->mm, gm->vm, gm->board);
}

void GM_run(GameManager* gm) {
    createSprites(gm);

    while (!processInput(gm)) {
        int32_t frameStart = SDL_GetTicks();
        gm->board->state(gm->board);
        VM_draw(gm->vm);
        delayFrame(frameStart);
    }
}
