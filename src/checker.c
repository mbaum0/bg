/**
 * @file checker.c
 * @author Michael Baumgarten
 * @brief 
 */
#include "checker.h"
#include "sprite.h"

void clickChecker(void* data){
    Checker* c = (Checker*)data;
    log_debug("Clicked checker %d\n", c->index);
    c->index++;
}

void updateChecker(Sprite* sprite, void* data){
    Checker* c = (Checker*)data;
    SDL_Rect src_rect;
    if (c->index % 2 == 0){
        src_rect = (SDL_Rect){ 0, 0, CHECKER_SIZE, CHECKER_SIZE };
    } else {
        src_rect = (SDL_Rect){ CHECKER_SIZE, 0, CHECKER_SIZE*2, CHECKER_SIZE };
    }
    Sprite_setSourceRect(sprite, src_rect);
}

void Checker_create(Sage* sage, int32_t index){
    Checker* c = malloc(sizeof(Checker));
    c->index = index;

    SDL_Texture* texture = Sage_loadTexture(sage, "assets/checker.png");
    SDL_Rect src_rect = {0, 0, CHECKER_SIZE, CHECKER_SIZE};
    Sprite* s = Sprite_create(texture, src_rect, 0, 0, 0, false, updateChecker, c, clickChecker, c);
    Sage_registerSprite(sage, s);
}
