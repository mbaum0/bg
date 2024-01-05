/**
 * @file snippet.c
 * @author Michael Baumgarten
 * @brief Snippet implementation for the game engine
 */
#include "snippet.h"

void SnippetArray_init(SnippetArray* snippetArray) {
    snippetArray->snippets = NULL;
    snippetArray->size = 0;
}

void SnippetArray_append(SnippetArray* snippetArray, Snippet* snippet) {
    snippetArray->size++;
    snippetArray->snippets = realloc(snippetArray->snippets, snippetArray->size * sizeof(Snippet*));
    snippetArray->snippets[snippetArray->size - 1] = snippet;
}

void SnippetArray_free(SnippetArray* snippetArray) {
    for (int32_t i = 0; i < snippetArray->size; i++) {
        free(snippetArray->snippets[i]->text);
        free(snippetArray->snippets[i]);
    }
    free(snippetArray->snippets);
    snippetArray->size = 0;
}

void Snippet_setLocation(Snippet* snippet, int32_t x, int32_t y) {
    SDL_DestroyTexture(snippet->texture);
    snippet->x = x;
    snippet->y = y;
    SDL_Surface* surface = TTF_RenderText_Solid_Wrapped(snippet->font, snippet->text, snippet->color, 0);
    snippet->texture = SDL_CreateTextureFromSurface(snippet->renderer, surface);
    snippet->dst_rect = (SDL_Rect){ snippet->x, snippet->y, surface->w, surface->h };
    SDL_FreeSurface(surface);
}

void Snippet_setText(Snippet* snippet, char* text) {
    free(snippet->text);
    snippet->text = malloc(strlen(text) + 1);
    strcpy(snippet->text, text);
    SDL_DestroyTexture(snippet->texture);
    SDL_Surface* surface = TTF_RenderText_Solid_Wrapped(snippet->font, snippet->text, snippet->color, 0);
    snippet->texture = SDL_CreateTextureFromSurface(snippet->renderer, surface);
    snippet->dst_rect = (SDL_Rect){ snippet->x, snippet->y, surface->w, surface->h };
    SDL_FreeSurface(surface);
}

void Snippet_setVisible(Snippet* snippet, bool visible) {
    snippet->visible = visible;
}
