#ifndef DYNARRAY_H
#define DYNARRAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "util.h"

#ifdef _MSC_VER
#include <intrin.h>
#endif

#define VECTOR_INITIAL_CAPACITY 8

typedef enum
{
    VECTOR_NO_ERROR,
    VECTOR_ALLOCATION_ERROR,
    VECTOR_FREED
} vector_err_flag;

typedef struct
{
    size_t capacity;    /* THIS IS IN BYTES, REMEMBER THIS YOU MONGREL */
    size_t size;        /* NOT IN BYTES, THIS IS THE COUNT BASICALLY */
    size_t elem_size;

    vector_err_flag status;
    void *data;
} vector;

vector initialize_vec(size_t elem_size);

void free_vector(vector *vec);

/* remember to turn it into force_inline */
FORCE_INLINE int vector_push_back(vector *vec, const void *element)
{   
    if(UNLIKELY((vec->size + 1) * vec->elem_size > vec->capacity)) 
    {
        //FUCKING SHIT BRO, this prevents a bug, so we dont end up losing the old ptr when 
        //reallocing, anyways this doubles the capacity
        void *tmp = realloc(vec->data, vec->capacity * 2); 
        if(!tmp)
        {
            vec->status = VECTOR_ALLOCATION_ERROR;
            return -1;
        }
        vec->capacity *= 2;

        vec->data = tmp;
    }
    
    memcpy((char *)vec->data + vec->size * vec->elem_size, element, vec->elem_size);
    vec->size++;

    return 0;
}

int resize_vector(vector *vec, size_t size);

int insert_vector(vector *vec, const void *elements, size_t start, size_t end);

FORCE_INLINE int shrink_to_fit_vector(vector *vec)
{
    if(!vec)
        return -1;

    size_t bytesize = vec->size * vec->elem_size;
    if(bytesize < vec->capacity)
    {
        void *tmp = realloc(vec->data, bytesize);
        
        if(!tmp)
        {
            vec->status = VECTOR_ALLOCATION_ERROR;
            return -1;
        }

        vec->data = tmp;
        vec->capacity = bytesize;
    }

    return 0;
}

#define VEC_POP_BACK(vec) vec.size--

#ifdef __cplusplus
}
#endif


#endif

