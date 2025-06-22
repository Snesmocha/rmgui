#ifndef DYNSTR_H
#define DYNSTR_H

#ifdef __cplusplus
extern "C" {
#endif


#include <wchar.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <string.h>

#include "util.h"

#include "dynarray.h"


typedef enum
{
    STR_NO_ERROR,
    STR_ALLOCATION_ERROR,
    STR_TYPE_ERROR,
    STR_FREED
} str_err_flag;

typedef enum
{
	STRT_CHAR = 1,
	STRT_WCHAR = 2,
	STRT_CHAR_NOALLOC = 3,
	STRT_WCHAR_NOALLOC = 4,
} str_type_flag;

typedef struct
{
	size_t size;        /* represents element count not size*/
	size_t capacity;

	str_err_flag status;
	
	uint32_t type;
	union
	{
		wchar_t* w_string;
		char* c_string;
	} str;
} string;


// memory, capacity, and initialization
string initialize_str(char* str, size_t length);	// to initialize as empty just type in NULL
string initialize_wstr(wchar_t* str, size_t length);	// same as top

string initialize_str_noalloc(char* str, size_t length);

int str_to_wstr(string* str);
int wstr_to_str(string* str);


int string_resize(string* str, size_t size);
bool string_reserve(string* str, size_t reserve);
bool string_shrink_to_fit(string* str);

// force inlined code
FORCE_INLINE string initialize_strn(char* str)
{
    return initialize_str(str, strlen(str));
}

FORCE_INLINE string initialize_wstrn(wchar_t* str)
{
    return initialize_wstr(str, wcslen(str));
}

void free_string(string* str);

int string_atoi(string str);
long string_atol(string str);
unsigned int string_atou(string str);
unsigned long string_atoul(string str);

// direct write funcs
int string_cpy(const string src, string* dest);
int string_concat(const string a, const string b, string* dest);
int string_tok(const string str, const string delim, size_t* count, string* dest);
int string_sub(const string str, const string substr, string* dest);
int string_dup(const string src, string* dest);
int string_format(const string str, string* dest, ...); //note each one must return with size

// returns copies
string string_c_concat(const string a, const string b);
string string_c_tok(const string str, const string delim, size_t* count);
string string_c_sub(const string str, const string substr);
string string_c_dup(const string src);
string string_c_format(const string str, ...);

// relies on other lib for this function
vector string_split(const string str, const string delim);

int string_cmp(const string a, const string b);
int string_starts(const string str, const string start);
int string_ends(const string str, const string end);
int string_contains(const string str, const string contain);


// returns the pointer, does not return
LEAF FORCE_INLINE char* to_cstr(string* str) 
{
	return str->str.c_string;
}

LEAF FORCE_INLINE wchar_t* to_cwstr(string* str)
{
	return str->str.w_string;
}

// utility 
#ifdef __cplusplus
}
#endif

#endif
