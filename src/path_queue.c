#include "path_queue.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define INITIAL_QUEUE_CAPACITY 64

int queue_init(PathQueue *q)
{
    q->capacity = INITIAL_QUEUE_CAPACITY;
    q->paths = malloc(q->capacity * sizeof(char *));
    if (!q->paths) return -1;
    q->head = 0;
    q->tail = 0;
    q->count = 0;
    return 0;
}

static int queue_grow(PathQueue *q)
{
    size_t new_cap = q->capacity * 2;
    char **new_paths = malloc(new_cap * sizeof(char *));
    if (!new_paths) return -1;

    for (size_t i = 0; i < q->count; i++) {
        new_paths[i] = q->paths[(q->head + i) % q->capacity];
    }
    free(q->paths);
    q->paths = new_paths;
    q->head = 0;
    q->tail = q->count;
    q->capacity = new_cap;
    return 0;
}

int queue_push(PathQueue *q, const char *path)
{
    if (q->count == q->capacity) {
        if (queue_grow(q) != 0) return -1;
    }
    q->paths[q->tail] = strdup(path);
    if (!q->paths[q->tail]) return -1;
    q->tail = (q->tail + 1) % q->capacity;
    q->count++;
    return 0;
}

char *queue_pop(PathQueue *q)
{
    if (q->count == 0) return NULL;
    char *path = q->paths[q->head];
    q->head = (q->head + 1) % q->capacity;
    q->count--;
    return path;
}

size_t queue_size(const PathQueue *q)
{
    return q->count;
}

void queue_destroy(PathQueue *q)
{
    for (size_t i = 0; i < q->count; i++) {
        free(q->paths[(q->head + i) % q->capacity]);
    }
    free(q->paths);
    q->paths = NULL;
    q->capacity = 0;
    q->head = 0;
    q->tail = 0;
    q->count = 0;
}
