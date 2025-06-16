#ifndef DYNARRAY_H
#define DYNARRAY_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define VECTOR_INITIAL_CAPACITY 8

/* put this in some utilities header or something */
#if defined(__GNUC__) || defined(__clang__)

#define UNLIKELY(x) __builtin_expect(!!(x), 0)
#define LIKELY(x) __builtin_expect(!!(x), 1)

#else   /*MSVC is the only crappy compiler that doesnt support this*/

#define UNLIKELY(x) (x)
#define LIKELY(x) (x)

#endif
    

typedef enum
{
    VECTOR_NO_ERROR,
    VECTOR_ALLOCATION_ERROR,
    VECTOR_FREED
} vector_err_flag;

typedef struct
{
    size_t capacity;    /* THIS IS IN BYTES, REMEMBER THIS YOU MONGREL */
    size_t size;        /* NOT IN BYTES, NIGGER, THIS IS THE COUNT BASICALLY */
    size_t elem_size;

    vector_err_flag status;
    void *data;
} vector;

vector initialize_vec(size_t elem_size);

void free_vector(vector *vec);

/* remember to turn it into force_inline */
static inline int vector_push_back(vector *vec, const void *element)
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

#define VEC_POP_BACK(vec) vec.size--

#endif

