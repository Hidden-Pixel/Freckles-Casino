typedef struct _buffer
{
    unsigned int capacity;
    unsigned int size;
    unsigned int element_size;
    void *memory;
} Buffer;

Buffer
Buffer_Create(unsigned int element_size, int capacity);

void
Buffer_Resize(Buffer *buffer);

void *
Buffer_Add_Element(Buffer *buffer, void *data);

void *
Buffer_Get_Element(Buffer *buffer, unsigned int index);

void
Buffer_Free(Buffer *buffer);
