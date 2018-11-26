#include <FC/buffer.h>
#include <stdlib.h>
#include <assert.h>

#define DEFAULT_CAPACITY 10
#define DEFAULT_SIZE 0

Buffer
Buffer_Create(unsigned int element_size, int capacity)
{
    Buffer buffer = (Buffer)
    {
        .capacity       = 0,
        .size           = 0,
        .element_size   = 0,
        .memory         = NULL
    };
    if (buffer.element_size > 0)
    {
        if (buffer.capacity <= 0)
        {
            buffer.capacity = DEFAULT_CAPACITY;
        }
        if (element_size > 0)
        {
            buffer.memory = malloc(buffer.element_size * buffer.capacity);
            assert(buffer.memory != NULL);
        }
    }
    return buffer;
}

void *
Buffer_Add_Element(Buffer *buffer, void *data)
{
    // TODO(nick):
    if (buffer->size >= buffer->capacity)
    {
        Buffer_Resize(buffer);
    }
    return NULL;
}

void *
Buffer_Get_Element(Buffer *buffer, unsigned int index)
{
    // TODO(nick):
    return NULL;
}

void
Buffer_Resize(Buffer *buffer)
{
    if (buffer && buffer->memory)
    {
        assert(1 == 0);
        // TODO(nick):
    }
}

void
Buffer_Free(Buffer *buffer)
{
    if (buffer && buffer->memory)
    {
        free(buffer->memory);
    }
}
