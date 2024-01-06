/**
 * @file sage.c
 * @author Michael Baumgarten
 * @brief Simply-Another-Game-Engine (SAGE)
 */
#include "sage.h"

Sage* Sage_create(char* title, int width, int height){
    Sage* s = malloc(sizeof(Sage));
    s->mm = MM_init(title, width, height);
    s->vm = VM_init(s->mm->renderer);
    return s;
}

void Sage_destroy(Sage* s){
    VM_free(s->vm);
    MM_free(s->mm);
    free(s);
}

void Sage_step(Sage* s){
    VM_draw(s->vm);
}
