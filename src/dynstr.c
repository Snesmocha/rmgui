#include <rmgui/dynstr.h>
#include <rmgui/util.h>
#include <wchar.h>



// borrowed from gcc in idea
// only used internally


// at top are helper implementation functions


string initialize_str(char* str, size_t length)
{
	string dynstr = {.size=0, .capacity=round_next_pow2(length), .status=STR_NO_ERROR, .type=STRT_CHAR};

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
	string dynstr = {.size=0, .status=STR_NO_ERROR, .type=STRT_WCHAR};

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

int string_cpy(const string src, string* dest)
{

    
	return 0;
}


void free_string(string* str)
{
	
	if(str->type == STRT_CHAR) free(str->str.c_string);
	else free(str->str.w_string);
	
	str->status = STR_FREED;
}
