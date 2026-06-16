#include "path_stack.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define INITIAL_STACK_CAPACITY 64

int stack_init(PathStack *s)
{
    s->capacity = INITIAL_STACK_CAPACITY;
    s->paths = malloc(s->capacity * sizeof(char *));
    if (!s->paths) return -1;
    s->top = 0;
    return 0;
}

static int stack_grow(PathStack *s)
{
    size_t new_cap = s->capacity * 2;
    char **new_paths = realloc(s->paths, new_cap * sizeof(char *));
    if (!new_paths) return -1;
    s->paths = new_paths;
    s->capacity = new_cap;
    return 0;
}

int stack_push(PathStack *s, const char *path)
{
    if (s->top == s->capacity) {
        if (stack_grow(s) != 0) return -1;
    }
    s->paths[s->top] = strdup(path);
    if (!s->paths[s->top]) return -1;
    s->top++;
    return 0;
}

char *stack_pop(PathStack *s)
{
    if (s->top == 0) return NULL;
    s->top--;
    return s->paths[s->top];
}

size_t stack_size(const PathStack *s)
{
    return s->top;
}

void stack_destroy(PathStack *s)
{
    for (size_t i = 0; i < s->top; i++) {
        free(s->paths[i]);
    }
    free(s->paths);
    s->paths = NULL;
    s->capacity = 0;
    s->top = 0;
}
