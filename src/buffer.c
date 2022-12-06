#include <stdlib.h>

#include "headers/buffer.h"

buffer_t *malloc_buffer(int size) {
    buffer_t *buffer = (buffer_t *) malloc(sizeof(buffer_t));
    buffer->buffer = (void **) malloc(size * sizeof(int *));
    buffer->size = size;
    buffer->first = 0;
    buffer->last = 0;
    buffer->in = 0;
    return buffer;
}

void put(buffer_t *buffer, int *item){
    // If the buffer is already full and there is no free place, return
    if (buffer->in == buffer->size) return;

    (buffer->buffer)[buffer->last] = item;
    buffer->last = ((buffer->last) + 1) % (buffer->size);
    (buffer->in)++;
}

int *get(buffer_t *buffer){
    // If empty buffer
    if (buffer->in == 0) return NULL;

    // The element to be removed from the buffer
    int *removed = (buffer->buffer)[buffer->first];
    (buffer->buffer)[buffer->first] = NULL;
    buffer->first = ((buffer->first) + 1) % (buffer->size);
    (buffer->in)--;
    return removed;

}

void free_buffer(buffer_t *buffer){
    // Free matrix
    free(buffer->buffer);
    free(buffer);
}