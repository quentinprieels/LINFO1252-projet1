#ifndef BUFFER_H
#define BUFFER_H

typedef struct {
    void **buffer;
    int size;  // The length of the buffer buffer
    int first; // The pointer to the buffer index where you can find the first inserted element that's still in the buffer(or more exactly the pointer to the first element, **if any**)
    int last;  // The pointer to the buffer index where you can find the first empty space in the buffer (or more exactly the first NULL pointer in the buffer, **if any**)
    int in;    // The number of structures in the buffer
} buffer_t;

/**
 * Create a new buffer.
 * 
 * @param size : The length of the buffer.
 * @return buffer_t* initialized with the good size
 */
buffer_t *malloc_buffer(int size);

/** 
* Put a new structure in the buffer buffer.
*
* @param buffer : the buffer buffer to fill in with the pointers of the structures
* @param item : the item that has to be inserted in the buffer
*/
void put(buffer_t *buffer, int *item);

/**
* Get a structure from the buffer buffer.
*
* @param buffer : the buffer buffer when the element is taken
* @return the pointer to the element that you get if no error, NULL otherwise
*/
int *get(buffer_t *buffer);

/**
 * Free the memory allocated to a buffer_t struct
 * 
 * @param system The buffer_t to free
 */
void free_buffer(buffer_t *buffer);

#endif /* BUFFER_H */