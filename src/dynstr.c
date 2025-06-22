#include <rmgui/dynstr.h>
#include <rmgui/util.h>
#include <wchar.h>



// borrowed from gcc in idea
// only used internally


// at top are helper implementation functions


string initialize_str(char* str, size_t length)
{
	string dynstr = {.size=length, .capacity=round_next_pow2(length), .status=STR_NO_ERROR, .type=STRT_CHAR};

	dynstr.str.c_string = malloc(sizeof(char) * dynstr.capacity);
	if(UNLIKELY(!dynstr.str.c_string)) dynstr.status = STR_ALLOCATION_ERROR;
	
	
	if(!str) return dynstr;

	char* dst = dynstr.str.c_string;

	// haha we love compiler optimizations
	switch(length)
	{
    case 1:
        memcpy(dst, str, 1);
        break;
    case 2:
        memcpy(dst, str, 2);
        break;
    case 4:
        memcpy(dst, str, 4);
        break;
    case 8:
        memcpy(dst, str, 8);
        break;
    default:
        memcpy(dst, str, length);
	}
	
	return dynstr;
}


string initialize_wstr(wchar_t* str, size_t length)
{
	string dynstr = {.size=length, .status=STR_NO_ERROR, .type=STRT_WCHAR};

	dynstr.str.w_string = malloc(sizeof(wchar_t) * length);
	
	if(UNLIKELY(!dynstr.str.w_string)) dynstr.status = STR_ALLOCATION_ERROR;
	
	if(!str) return dynstr;

	wchar_t* dst = dynstr.str.w_string;


	// haha we love compiler optimizations
	switch(length)
	{
    case 1:
        memcpy(dst, str, 1);
        break;
    case 2:
        memcpy(dst, str, 2);
        break;
    case 4:
        memcpy(dst, str, 4);
        break;
    case 8:
        memcpy(dst, str, 8);
        break;
    default:
        memcpy(dst, str, length);
	}
	
	
	return dynstr;
}


string initialize_str_noalloc(char* str, size_t length)
{
    string dynstr = {.size=0, .status=STR_NO_ERROR, .type=STRT_CHAR_NOALLOC, {.c_string=str}};
    return dynstr;
}

int string_resize(string* str, size_t size)
{
    if(str->type == STRT_CHAR_NOALLOC || str->type == STRT_WCHAR_NOALLOC) 
    {
        str->status = STR_TYPE_ERROR;
        return -1;
    }
    
    //only realloc when size > capacity
    if(UNLIKELY(size * str->type > str->capacity))
    {
        //the usual doubling
        size_t new_cap_elem = (str->capacity / str->type) * 2;
        
        //if this rare case happens, run pow2
        if(new_cap_elem < size)
        {
            new_cap_elem = round_next_pow2(size);
        }
        
        void *tmp;
        if (str->type == STRT_CHAR) tmp = realloc(str->str.c_string, new_cap_elem * str->type);
        else tmp = realloc(str->str.w_string, new_cap_elem * str->type);
        
        if(!tmp)
        {
            str->status = STR_ALLOCATION_ERROR;
            return -1;
        }
        
        str->capacity = new_cap_elem * str->type;
        if (str->type == STRT_CHAR) 
        {
            str->str.c_string = tmp;
            memset((char *)str->str.c_string + str->size * str->type, 0, (size - str->size) * str->type);
        }
        else
        {
            str->str.w_string = tmp;
            memset((char *)str->str.w_string + str->size * str->type, 0, (size - str->size) * str->type);
        }
    }

    str->size = size;

    return 0;
}


int string_cpy(const string src, string* dest)
{
    if(src.type != dest->type)
    {
        dest->status = STR_TYPE_ERROR;
        return -1;
    }
    
    if(dest->capacity < src.size)
    {
        dest->capacity *= 2;
        string_resize(dest, dest->capacity);
    }
    
    if(src.type == STRT_CHAR) memcpy_s(dest->str.c_string, dest->capacity, src.str.c_string, src.size);
    else memcpy_s(dest->str.c_string, dest->capacity * 2,src.str.c_string, src.size * 2);
    
    dest->size = src.size;
	return 0;
}


void free_string(string* str)
{
	
	if(str->type == STRT_CHAR) free(str->str.c_string);
	else free(str->str.w_string);
	
	str->status = STR_FREED;
}
