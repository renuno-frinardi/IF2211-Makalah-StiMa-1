#ifndef PATH_STACK_H
#define PATH_STACK_H

#include <stddef.h>

typedef struct {
    char **paths;
    size_t capacity;
    size_t top;
} PathStack;

int  stack_init(PathStack *s);
int  stack_push(PathStack *s, const char *path);
char *stack_pop(PathStack *s);
size_t stack_size(const PathStack *s);
void stack_destroy(PathStack *s);

#endif
