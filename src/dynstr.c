#include <rmgui/dynstr.h>
#include <rmgui/util.h>



// borrowed from gcc in idea
// only used internally


// at top are helper implementation functions


string* initialize_str(char* str, size_t length)
{
	string* dynstring = {.size=0, .capacity=round_next_pow2(length), .status=STR_NO_ERROR, .type=STRT_CHAR};

	dynstring.str.c_string = malloc(sizeof(char) * dynstring.capacity);
	if(UNLIKELY(!dynstring.str.c_string)) dynstring.status = STR_ALLOCATION_ERROR;
	
	
	if(!str) return dynstring;

	char* dst = dynstring.str.c_string;

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
	
	return dynstring;
}


string* initialize_wstr(wchar_t* str, size_t length);
{
	string dynstr = {.size=0, .status=STR_NO_ERROR, .type=STRT_WCHAR_T};

	dynstr.c_string = malloc(sizeof(wchar_t) * length);
	
	if(UNLIKELY(!dynstr.data)) dynstr.status = STR_ALLOCATION_ERROR;
	
	if(!str) return dynstring;

	char* dst = dynstring.str.w_string;


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
	
	
	return dynstring;
	return dystr;
}

int string_cpy(string* src, string* dest)
{
	
}


void free_string(string* str)
{
	
	if(str->type == STRT_CHAR) free(str->c_string);
	else free(str->wstring);
	
	str->status = STR_FREED;
}
