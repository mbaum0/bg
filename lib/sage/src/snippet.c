/**
 * @file snippet.c
 * @author Michael Baumgarten
 * @brief Snippet implementation for the game engine
 */
#include "snippet.h"
#include <stdlib.h>

void Snippet_setLocation(Snippet* snippet, Sint32 x, Sint32 y) {
    (void)snippet;
    (void)x;
    (void)y;
    // SDL_DestroyTexture(snippet->texture);
    // snippet->x = x;
    // snippet->y = y;
    // SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(snippet->font, snippet->text, snippet->color, 0);
    // snippet->texture = SDL_CreateTextureFromSurface(snippet->renderer, surface);
    // snippet->dst_rect = (SDL_FRect){snippet->x, snippet->y, surface->w, surface->h};
    // SDL_DestroySurface(surface);
}

void Snippet_setText(Snippet* snippet, char* text) {
    (void)snippet;
    (void)text;

    // exit early if strings are equals
    // if (snippet->text != NULL && strcmp(snippet->text, text) == 0){
    //     return;
    // }
    snippet->textLen = strlen(text);

    snippet->text = SDL_realloc(snippet->text, snippet->textLen + 1);
    strcpy(snippet->text, text);

    snippet->chars = SDL_realloc(snippet->chars,snippet->textLen * sizeof(SnippetChar));

    SDL_FRect src, dst;
    BMFontChar bmchar;
    char c;
    float fontScale = snippet->font->scale;
    Uint32 fontSrcSize = snippet->font->srcSize;
    Uint32 fontDstSize = snippet->font->dstSize;
    Sint32 cOffset = 0;
    for (Sint32 i = 0; i < snippet->textLen; i++){
        c = text[i];
        bmchar = snippet->font->layout.chars.chars[c - 32];
        src.h = bmchar.height;
        src.w = bmchar.width;
        src.x = bmchar.x;
        src.y = bmchar.y;
        dst.h = NORMALIZE_FONT(bmchar.height, fontSrcSize, fontDstSize);
        dst.w = NORMALIZE_FONT(bmchar.width, fontSrcSize, fontDstSize);
        dst.x = snippet->x + NORMALIZE_FONT(cOffset, fontSrcSize, fontDstSize);
        dst.y = snippet->y + NORMALIZE_FONT(bmchar.yoffset, fontSrcSize, fontDstSize);
        snippet->chars[i].c = c;
        snippet->chars[i].src = src;
        snippet->chars[i].dst = dst;
        cOffset += bmchar.xadvance;
    }
    snippet->boundBox.w = bmchar.width * snippet->textLen * fontScale;
    snippet->boundBox.h = bmchar.height * fontScale;
    snippet->boundBox.x = snippet->x * fontScale;
    snippet->boundBox.y = snippet->y * fontScale;
}

void Snippet_setVisible(Snippet* snippet, bool visible) {
    snippet->visible = visible;
}

Snippet* Snippet_create(SageFont* font, SDL_Color color, Sint32 x, Sint32 y, Sint32 z, bool visible) {
    Snippet* snippet = SDL_malloc(sizeof(Snippet));
    snippet->id = 0;
    snippet->font = font;
    snippet->x = x;
    snippet->y = y;
    snippet->z = z;
    snippet->visible = visible;
    snippet->update_fn = NULL;
    snippet->update_data = NULL;
    snippet->click_fn = NULL;
    snippet->click_object = NULL;
    snippet->click_context = NULL;
    snippet->useViewport = false;
    snippet->textLen = 0;
    snippet->color = color;
    return snippet;
}
