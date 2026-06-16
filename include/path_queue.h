#ifndef PATH_QUEUE_H
#define PATH_QUEUE_H

#include <stddef.h>

typedef struct {
    char **paths;
    size_t capacity;
    size_t head;
    size_t tail;
    size_t count;
} PathQueue;

int  queue_init(PathQueue *q);
int  queue_push(PathQueue *q, const char *path);
char *queue_pop(PathQueue *q);
size_t queue_size(const PathQueue *q);
void queue_destroy(PathQueue *q);

#endif
