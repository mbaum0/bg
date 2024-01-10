/**
 * @file sand.c
 * @author Michael Baumgarten
 * @brief
 */
#include "sand.h"

void click_cb(uint32_t eventType, SDL_Event* e, void* data) {
    (void)e;
    Sage* sage = (Sage*)data;
    if (eventType == SDL_MOUSEMOTION) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        Sand_create(sage, x, y);
    }
}

void updateSand(ViewManager* vm, Sprite* sprite, void* data) {
    Sand* s = (Sand*)data;
    float newV = s->vel + (s->acc * .0116);
    int32_t newY = s->y + newV;

    if (newY >= 600 - 1) {
        s->y = 600 - 1;
        return;
    }

    Sprite* collision = VM_findSpriteCollision(vm, sprite);
    if (collision == NULL) {
        s->y = newY;
        s->vel = newV;
        Sprite_setLocation(sprite, s->x, s->y);
    }
}

Sand* Sand_create(Sage* sage, int32_t x, int32_t y) {
    Sand* s = malloc(sizeof(Sand));
    s->x = x;
    s->y = y;
    s->acc = 9.8;
    s->vel = 0;

    SDL_Texture* texture = Sage_loadTexture(sage, "assets/sand.png");
    SDL_Rect src_rect = { 0, 0, 1, 1 };
    Sprite* sprite = Sprite_create(texture, src_rect, x, y, 0, false);
    Sprite_registerUpdateFn(sprite, updateSand, s);
    Sage_registerSprite(sage, sprite);
    
    return s;
}

void Sand_destroy(Sand* s) {
    free(s);
}

void Sand_init(Sage* sage){
    Sage_registerEventCallback(sage, SDL_MOUSEMOTION, click_cb, sage);
}
