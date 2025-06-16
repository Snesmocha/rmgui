#include <rmgui/dynarray.h>
#include <stdlib.h>
#include <assert.h>


vector initialize_vec(size_t elem_size)
{
    assert(elem_size != 0 && "VECTOR ERROR, ELEMENT SIZE MUSTN'T BE ZERO");
    vector dynarray = {.elem_size = elem_size, .capacity = VECTOR_INITIAL_CAPACITY * elem_size,
                        .size = 0};

    dynarray.data = malloc(dynarray.capacity);
    
    if(!dynarray.data)
        dynarray.status = VECTOR_ALLOCATION_ERROR;
    else
        dynarray.status = VECTOR_NO_ERROR;



    return dynarray;
}

void free_vector(vector *vec)
{
    free(vec->data);
    vec->status = VECTOR_FREED;

}

