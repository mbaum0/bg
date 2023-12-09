/**
 * @file view.c
 * @author Michael Baumgarten
 * @brief ViewManager implementation
 */
#include "view.h"

typedef struct SpriteArray SpriteArray;
typedef struct SnippetArray SnippetArray;

struct ViewManager {
    SDL_Renderer* renderer;
    SpriteArray* sprites;
    SnippetArray* snippets;
};

struct SpriteArray {
    Sprite** sprites;
    uint32_t size;
};

struct Sprite {
    SDL_Texture* texture;
    SDL_Rect src_rect;
    SDL_Rect dst_rect;
    bool visible;
    bool hovered;
    SpriteUpdate_fn update_fn;
    void* update_data;
    int32_t z;
};

struct Snippet {
    TTF_Font* font;
    SDL_Color color;
    uint32_t x;
    uint32_t y;
    uint32_t z;
    char* text;
    SDL_Rect dst_rect;
    SDL_Texture* texture;
    SnippetUpdate_fn update_fn;
    void* update_data;
    SDL_Renderer* renderer; // Snippets get a special pointer to the renderer so they can update their texture
};

struct SnippetArray {
    Snippet** snippets;
    uint32_t size;
};

void initSpriteArray(SpriteArray* spriteArray) {
    spriteArray->sprites = NULL;
    spriteArray->size = 0;
}

void appendSprite(SpriteArray* spriteArray, Sprite* sprite) {
    spriteArray->size++;
    spriteArray->sprites = realloc(spriteArray->sprites, spriteArray->size * sizeof(Sprite*));
    spriteArray->sprites[spriteArray->size - 1] = sprite;
}

void freeSpriteArray(SpriteArray* spriteArray) {
    for (size_t i = 0; i < spriteArray->size; i++) {
        free(spriteArray->sprites[i]);
    }
    free(spriteArray->sprites);
    spriteArray->size = 0;
}

void initSnippetArray(SnippetArray* snippetArray) {
    snippetArray->snippets = NULL;
    snippetArray->size = 0;
}

void appendSnippet(SnippetArray* snippetArray, Snippet* snippet) {
    snippetArray->size++;
    snippetArray->snippets = realloc(snippetArray->snippets, snippetArray->size * sizeof(Snippet*));
    snippetArray->snippets[snippetArray->size - 1] = snippet;
}

void freeSnippetArray(SnippetArray* snippetArray) {
    for (size_t i = 0; i < snippetArray->size; i++) {
        free(snippetArray->snippets[i]->text);
        free(snippetArray->snippets[i]);
    }
    free(snippetArray->snippets);
    snippetArray->size = 0;
}

/**
 * @brief Used to pickup on mouse clicks on sprites
 *
 * @param data sprite array
 * @param event event that occured
 * @return int always 1 (this value is ignored)
 */
int handle_event(void* data, SDL_Event* event) {
    SpriteArray* spriteArray = (SpriteArray*)data;
    uint32_t topSpriteID = 0;
    int32_t currentTopZ = -1;
    switch (event->type) {
    case SDL_MOUSEBUTTONUP:
        for (size_t i = 0; i < spriteArray->size; i++) {
            Sprite* sprite = spriteArray->sprites[i];
            if (sprite->visible) {
                SDL_Point p = { event->button.x, event->button.y };
                if (SDL_PointInRect(&p, &sprite->dst_rect)) {
                    if (sprite->z > currentTopZ) {
                        currentTopZ = sprite->z;
                        topSpriteID = i;
                    }
                }
            }

        }
        if (currentTopZ != -1) {
            SpriteClickEvent* spe = malloc(sizeof(SpriteClickEvent));
            spe->sprite_id = topSpriteID;
            SDL_Event e;
            e.type = SPRITE_CLICK_EVENT;
            e.user.data1 = spe;
            SDL_PushEvent(&e);
        }
        break;
    default:
        break;
    }
    return 1;
}

ViewManager* VM_init(SDL_Renderer* renderer) {
    ViewManager* vm = malloc(sizeof(ViewManager));
    vm->renderer = renderer;
    vm->sprites = malloc(sizeof(SpriteArray));
    initSpriteArray(vm->sprites);
    vm->snippets = malloc(sizeof(SnippetArray));
    initSnippetArray(vm->snippets);
    SDL_AddEventWatch(handle_event, vm->sprites);
    return vm;
}

void VM_free(ViewManager* vm) {
    freeSpriteArray(vm->sprites);
    free(vm->sprites);
    freeSnippetArray(vm->snippets);
    free(vm->snippets);
    free(vm);
}

void VM_draw(ViewManager* vm) {
    SDL_RenderClear(vm->renderer);
    for (size_t i = 0; i < vm->sprites->size; i++) {
        Sprite* sprite = vm->sprites->sprites[i];
        if (sprite->update_fn != NULL) {
            sprite->update_fn(sprite, sprite->update_data);
        }
        if (sprite->visible) {
            SDL_RenderCopy(vm->renderer, sprite->texture, &sprite->src_rect, &sprite->dst_rect);
        }
    }
    for (size_t i = 0; i < vm->snippets->size; i++) {
        Snippet* snippet = vm->snippets->snippets[i];
        if (snippet->update_fn != NULL) {
            snippet->update_fn(snippet, snippet->update_data);
        }
        SDL_RenderCopy(vm->renderer, snippet->texture, NULL, &snippet->dst_rect);
    }
    SDL_RenderPresent(vm->renderer);
}

uint32_t VM_createSprite(ViewManager* vm, SDL_Texture* texture, SDL_Rect src, uint32_t x, uint32_t y, uint32_t z, SpriteUpdate_fn update_fn, void* update_data) {
    Sprite* sprite = malloc(sizeof(Sprite));
    sprite->texture = texture;
    sprite->src_rect = src;
    sprite->dst_rect = (SDL_Rect){ x, y, src.w, src.h };
    sprite->visible = true;
    sprite->update_fn = update_fn;
    sprite->z = z;
    sprite->update_data = update_data;
    appendSprite(vm->sprites, sprite);
    return vm->sprites->size - 1;
}

void Sprite_setLocation(Sprite* sprite, uint32_t x, uint32_t y) {
    sprite->dst_rect.x = x;
    sprite->dst_rect.y = y;
}

uint32_t Sprite_getX(Sprite* sprite) {
    return sprite->dst_rect.x;
}

uint32_t Sprite_getY(Sprite* sprite) {
    return sprite->dst_rect.y;
}

void Sprite_setVisible(Sprite* sprite, bool visible) {
    sprite->visible = visible;
}

void Sprite_setSourceRect(Sprite* sprite, SDL_Rect src) {
    sprite->src_rect = src;
}

uint32_t VM_createSnippet(ViewManager* vm, TTF_Font* font, SDL_Color color, char* text, uint32_t x, uint32_t y, uint32_t z, SnippetUpdate_fn update_fn, void* update_data) {
    Snippet* snippet = malloc(sizeof(Snippet));
    snippet->font = font;
    snippet->color = color;
    snippet->x = x;
    snippet->y = y;
    snippet->z = z;
    snippet->text = strdup(text);
    snippet->update_fn = update_fn;
    snippet->update_data = update_data;
    snippet->renderer = vm->renderer;
    SDL_Surface* surface = TTF_RenderText_Solid(snippet->font, snippet->text, snippet->color);
    snippet->texture = SDL_CreateTextureFromSurface(vm->renderer, surface);
    snippet->dst_rect = (SDL_Rect){ snippet->x, snippet->y, surface->w, surface->h };
    SDL_FreeSurface(surface);
    appendSnippet(vm->snippets, snippet);
    return vm->snippets->size - 1;
}

void Snippet_setLocation(Snippet* snippet, uint32_t x, uint32_t y) {
    SDL_DestroyTexture(snippet->texture);
    snippet->x = x;
    snippet->y = y;
    SDL_Surface* surface = TTF_RenderText_Solid(snippet->font, snippet->text, snippet->color);
    snippet->texture = SDL_CreateTextureFromSurface(snippet->renderer, surface);
    snippet->dst_rect = (SDL_Rect){ snippet->x, snippet->y, surface->w, surface->h };
    SDL_FreeSurface(surface);
}

void Snippet_setText(Snippet* snippet, char* text) {
    free(snippet->text);
    snippet->text = strdup(text);
    SDL_DestroyTexture(snippet->texture);
    SDL_Surface* surface = TTF_RenderText_Solid(snippet->font, snippet->text, snippet->color);
    snippet->texture = SDL_CreateTextureFromSurface(snippet->renderer, surface);
    snippet->dst_rect = (SDL_Rect){ snippet->x, snippet->y, surface->w, surface->h };
    SDL_FreeSurface(surface);
}
