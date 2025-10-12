/* ============================================================================

dumb_lib.h - something like my personal "standard library"/"C extension".
dumb_memset, dumb_memcpy, dumb_memcmp, Dumb_Arena, Dumb_Array & Dumb_String.
This library depends on stdlib.h - for malloc/calloc & free only.
If you want to know more, see: https://github.com/JKKross/dumb_lib/

===============================================================================

version 0.5.2
Copyright © 2025 Honza Kříž

https://github.com/JKKross
https://x.com/honza_kriz_bass

===============================================================================

	Index of this file
	------------------

	|SECTION| - LICENSE
	|SECTION| - DOCUMENTATION
	|SECTION| - COMPILE CHECKS
	|SECTION| - INCLUDES
	|SECTION| - DECLARATIONS
		- |CONSTANTS|
		- |MACROS|
		- |TYPES|
		- |MEMORY|
		- |ARRAY|
		- |STRING|
	|SECTION| - IMPLEMENTATION
		- |MEMORY IMPLEMENTATION|
		- |ARRAY IMPLEMENTATION|
		- |STRING IMPLEMENTATION|

===============================================================================

	|SECTION| - LICENSE
	----------------------------

The MIT License (MIT)
---------------------

Copyright © 2025 Honza Kříž

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the “Software”),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons
to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

===============================================================================

	|SECTION| - DOCUMENTATION
	----------------------------

Most "documentation" for this library is (at least for now) the code itself.
But a few notes:

- 1)
You _MUST_ '#define DUMB_LIB_IMPLEMENTATION'
in EXACTLY _one_ C or C++ file that includes this header, BEFORE the
include, like this:

	#define DUMB_LIB_IMPLEMENTATION
	#include "dumb_lib.h"

All other files should just #include "dumb_lib.h" without the #define.

- 2)
You can '#define DUMB_DEBUG', which turns on DUMB_ASSERT().
DUMB_ASSERT() does bounds checking on array access &
makes sure you got valid memory from dumb_arena_create().
Make sure you define it before including dumb_lib.h, like this:

	#define DUMB_DEBUG
	#define DUMB_LIB_IMPLEMENTATION
	#include "dumb_lib.h"

============================================================================ */

/*
	|SECTION| - COMPILE CHECKS
	--------------------------
*/

#if !(__GNUC__ || __CLANG__ || _MSC_VER || __TINYC__ )
	#error YOU ARE USING A COMPILER THIS LIBRARY WAS NOT TESTED WITH! \
If you wish to proceed, read through the source, run the tests & make sure everything works correctly!
#endif

#if !(defined(_WIN32) || defined(__linux__))
	#error THIS LIBRARY HAS NOT BEEN TESTED ON THIS PLATFORM! \
If you wish to proceed, read through the source, run the tests & make sure everything works correctly!
#endif

/*
	|SECTION| - INCLUDES
	--------------------
*/

/* @NOTE(Honza): For malloc/calloc & free only! */
#include <stdlib.h>

/*
	|SECTION| - DECLARATIONS
	------------------------
*/

#ifdef __cplusplus
extern "C" {
#endif

/* --- |CONSTANTS| --- */

/* Defaults */
#define DUMB_ARENA_MIN_CAPACITY         4096 /* Memory page size on most computers in 2025 */
#define DUMB_DEFAULT_ARRAY_SIZE         256  /* No reason so far */
#define DUMB_DEFAULT_STRING_SIZE_BYTES  32   /* No reason so far */

/* --- |MACROS| --- */

#ifdef DUMB_DEBUG
	/*
	   If the assert fails, we crash, and can see where in a debugger.
	   No need to get fancier - at least for now.
	*/
	#define DUMB_ASSERT(condition) if (!(condition)) { *(volatile int *)0 = 0; }
#else
	/* Do nothing in release builds. */
	#define DUMB_ASSERT(condition)
#endif

#define DUMB_KB(n)  (n * 1024)
#define DUMB_MB(n)  (n * 1048576)
#define DUMB_GB(n)  (n * 1073741824)
#define DUMB_TB(n)  (n * 1099511627776)

/* --- |TYPES| --- */

typedef enum Dumb_Comparison_Result
{
	A_GREATER_THEN_B = 1,
	A_EQUALS_B       = 0,
	A_LESS_THEN_B    = -1,
} Dumb_Comparison_Result;

typedef struct Dumb_Arena Dumb_Arena;

struct Dumb_Arena {
	Dumb_Arena    *_previous; /* Set to NULL on the very first arena in chain. */
	Dumb_Arena    *_current;  /* Set to NULL on all arenas except the very first one in chain. */
	size_t         _capacity;
	size_t         _position;
	unsigned char *_memory;
};

typedef struct Dumb_Array {
	size_t         _count;
	size_t         _capacity;
	size_t         _elem_size;
	unsigned char *_elements;
} Dumb_Array;

typedef struct Dumb_String {
	size_t         _count;
	size_t         _capacity;
	unsigned char *_chars;
} Dumb_String;

/* --- |MEMORY| --- */

void                   dumb_memcpy(void *to, void *from, size_t num_bytes);
Dumb_Comparison_Result dumb_memcmp(void *a, void *b, size_t num_bytes);
void                   dumb_memset(void *memory, unsigned char byte, size_t num_bytes);

Dumb_Arena *dumb_arena_create(size_t size);
void        dumb_arena_destroy(Dumb_Arena *arena);
void       *dumb_arena_push(Dumb_Arena *arena, size_t size);
void        dumb_arena_pop(Dumb_Arena *arena, size_t size);

/* --- |ARRAY| --- */

Dumb_Array  dumb_array_create(Dumb_Arena *arena, size_t elem_size);
Dumb_Array  dumb_array_create_precise(Dumb_Arena *arena, size_t elem_size, size_t number_of_elems);
void        dumb_array_clear(Dumb_Array *array);
void        dumb_array_push(Dumb_Arena *arena, Dumb_Array *a, void *new_elem);
void        dumb_array_pop(Dumb_Array *arr, unsigned char *ret_buf);
void       *dumb_array_get(Dumb_Array *a, size_t index);

/* --- |STRING| --- */

Dumb_String            dumb_string_create(Dumb_Arena *arena);
Dumb_String            dumb_string_create_precise(Dumb_Arena *arena, size_t capacity);
Dumb_String            dumb_string_from(Dumb_Arena *arena, const char *str);
void                   dumb_string_clear(Dumb_String *str);
void                   dumb_string_push(Dumb_Arena *arena, Dumb_String *str, unsigned char c);
unsigned char          dumb_string_pop(Dumb_String *str);
void                   dumb_string_append(Dumb_Arena *arena, Dumb_String *str_a, const char *str_b);
Dumb_Array             dumb_string_split_by_char(Dumb_Arena *arena, Dumb_String *str, unsigned char c);
void                   dumb_string_trim_whitespace(Dumb_String *str);
Dumb_Comparison_Result dumb_string_compare(Dumb_String *str_a, Dumb_String *str_b);

void PRIVATE_dumb_string_change_capacity(Dumb_Arena *arena, Dumb_String *str, size_t new_capacity);

/*
	|SECTION| - IMPLEMENTATION
	--------------------------
*/

#ifdef DUMB_LIB_IMPLEMENTATION

/* --- |MEMORY IMPLEMENTATION| --- */

void
dumb_memcpy(void *to, void *from, size_t num_bytes)
{
	size_t i;
	unsigned char *to_char   = (unsigned char *) to;
	unsigned char *from_char = (unsigned char *) from;

	for (i = 0; i < num_bytes; i++)
	{
		to_char[i] = from_char[i];
	}
}

Dumb_Comparison_Result
dumb_memcmp(void *a, void *b, size_t num_bytes)
{
	size_t i;
	unsigned char *aa = (unsigned char *) a;
	unsigned char *bb = (unsigned char *) b;

	for (i = 0; i < num_bytes; i++)
	{
		if      (aa[i] > bb[i]) { return A_GREATER_THEN_B; }
		else if (aa[i] < bb[i]) { return A_LESS_THEN_B; }
	}
	return A_EQUALS_B;
}

void
dumb_memset(void *memory, unsigned char byte, size_t num_bytes)
{
	int i;
	unsigned char *mem;

	mem = (unsigned char *)memory;

	for (i = 0; i < num_bytes; i++)
	{
		mem[i] = byte;
	}
}

Dumb_Arena *
dumb_arena_create(size_t size)
{
	size_t capacity;
	Dumb_Arena *new_arena;

	/*
	   @NOTE(Honza): Is this a good idea?

	   The reasoning is, that in most situations, if you need
	   less memory than DUMB_ARENA_MIN_CAPACITY,
	   you might as well use the stack or malloc/calloc.
	*/
	if (size < DUMB_ARENA_MIN_CAPACITY) { capacity = DUMB_ARENA_MIN_CAPACITY; }
	else                                { capacity = size; }

	new_arena = (Dumb_Arena *)calloc(sizeof(Dumb_Arena), sizeof(Dumb_Arena));

	new_arena->_previous = NULL;
	new_arena->_current  = new_arena;
	new_arena->_capacity = capacity;
	new_arena->_position = 0;
	new_arena->_memory   = (unsigned char *)calloc(capacity, sizeof(unsigned char));

	DUMB_ASSERT(new_arena->_memory != NULL)

	return new_arena;
}

void
dumb_arena_destroy(Dumb_Arena *arena)
{
	Dumb_Arena *tmp_ptr;

	while (1)
	{
		arena->_current->_capacity = 0;
		arena->_current->_position = 0;

		free(arena->_current->_memory);

		arena->_current->_memory = NULL;

		if (arena->_current->_previous == NULL)
		{
			arena->_current = NULL;
			free(arena);
			arena = NULL;
			return;
		}
		else
		{
			tmp_ptr = arena->_current;
			arena->_current = arena->_current->_previous;
			free(tmp_ptr);
		}
	}
}

void *
dumb_arena_push(Dumb_Arena *arena, size_t size)
{
	size_t old_position;
	size_t new_position;

	Dumb_Arena *new_arena;

	old_position = arena->_current->_position;
	new_position = arena->_current->_position + size;

	if (new_position > arena->_current->_capacity)
	{
		/* @NOTE(Honza): maybe this should be size * 2 ??? */
		new_arena = dumb_arena_create(size);

		new_arena->_previous = arena->_current;
		new_arena->_current  = NULL;
		new_arena->_position = size;

		arena->_current = new_arena;

		return arena->_current->_memory;
	}
	else
	{
		arena->_current->_position = new_position;
		return arena->_current->_memory + old_position;
	}
}

void
dumb_arena_pop(Dumb_Arena *arena, size_t size)
{
	int num_bytes_left_to_pop;
	Dumb_Arena *tmp_ptr;

	if (arena->_current->_position >= size)
	{
		arena->_current->_position -= size;
		dumb_memset((arena->_current->_memory + arena->_current->_position), 0, size - 1);
	}
	else
	{
		num_bytes_left_to_pop = size;

		while (num_bytes_left_to_pop != 0)
		{
			if ((arena->_current->_position == 0) && (arena->_current->_previous == NULL))
			{
				/*
				   The user can pass a huge value to the size parameter to clear the
				   whole arena chain & get to _position == 0 on the first arena
				   without destroying it.
				*/
				return;
			}

			if (arena->_current->_position < num_bytes_left_to_pop)
			{
				/* Destroy current arena. */
				arena->_current->_capacity = 0;
				free(arena->_current->_memory);
				arena->_current->_memory = NULL;

				/* Decrement num_bytes_left_to_pop. */
				num_bytes_left_to_pop -= arena->_current->_position;
				arena->_current->_position = 0;

				/* Set new current arena. */
				tmp_ptr = arena->_current;
				arena->_current = arena->_current->_previous;

				free(tmp_ptr);
			}
			else
			{
				arena->_current->_position -= num_bytes_left_to_pop;
				dumb_memset((arena->_current->_memory + arena->_current->_position), 0, num_bytes_left_to_pop);
				num_bytes_left_to_pop = 0;
			}
		}
	}
}

/* --- |ARRAY IMPLEMENTATION| --- */

Dumb_Array
dumb_array_create(Dumb_Arena *arena, size_t elem_size)
{
	return dumb_array_create_precise(arena, elem_size, DUMB_DEFAULT_ARRAY_SIZE);
}

Dumb_Array
dumb_array_create_precise(Dumb_Arena *arena, size_t elem_size, size_t number_of_elems)
{
	Dumb_Array a;

	if (number_of_elems < 1) { number_of_elems = 1; }

	a._count      = 0;
	a._capacity  = elem_size * number_of_elems;
	a._elem_size = elem_size;
	a._elements  = (unsigned char *)dumb_arena_push(arena, a._capacity);

	DUMB_ASSERT(a._elements != NULL)

	return a;
}

void
dumb_array_clear(Dumb_Array *array)
{
	dumb_memset(array->_elements, 0, (array->_count * array->_elem_size));
	array->_count = 0;
}

void
dumb_array_push(Dumb_Arena *arena, Dumb_Array *a, void *new_elem)
{
	unsigned char *new_elem_destination;

	size_t  new_capacity;
	unsigned char   *tmp;

	if ((a->_count * a->_elem_size) == a->_capacity)
	{
		new_capacity = a->_capacity * 2;
		tmp = (unsigned char *)dumb_arena_push(arena, new_capacity);

		dumb_memcpy(tmp, a->_elements, a->_capacity);
		/*
		   @NOTE(Honza): This obviously leaks memory.
		   In quick & dirty tools working on small data,
		   this may be fine, since the arena presumably gets
		   released at some point.

		   Do I need to worry about it?
		   The user (me, most likely) should know
		   what they're doing, and not allocate in their
		   persistent arena in cases when the data can grow arbitrarily...
		*/
		a->_elements = tmp;
		a->_capacity = new_capacity;
	}
	new_elem_destination = a->_elements + (a->_count * a->_elem_size);
	dumb_memcpy(new_elem_destination, new_elem, a->_elem_size);
	a->_count++;
}

/*
   User MUST supply a return buffer ('ret_buf')
   of at least the size of the array element!
*/
void
dumb_array_pop(Dumb_Array *arr, unsigned char *ret_buf)
{
	unsigned char *elem_ptr;

	/*
	   @NOTE(Honza): Should this crash?
	   Or fill ret_buf with 0xCDCDCDCD or something?
	*/
	if (arr->_count == 0) { return; }

	arr->_count--;

	elem_ptr = arr->_elements + (arr->_count * arr->_elem_size);
	dumb_memcpy(ret_buf, elem_ptr, arr->_elem_size);

	dumb_memset(elem_ptr, 0, arr->_elem_size);
}

void *
dumb_array_get(Dumb_Array *a, size_t index)
{
	unsigned char *result;

	DUMB_ASSERT(index < a->_count)

	result = a->_elements + (index * a->_elem_size);
	return (void *) result;
}

/* --- |STRING IMPLEMENTATION| --- */

Dumb_String
dumb_string_create(Dumb_Arena *arena)
{
	return dumb_string_create_precise(arena, DUMB_DEFAULT_STRING_SIZE_BYTES);
}

Dumb_String
dumb_string_create_precise(Dumb_Arena *arena, size_t capacity)
{
	Dumb_String s;

	if (capacity < 2) { capacity = 2; }

	s._count     = 0;
	s._capacity  = capacity;
	s._chars     = (unsigned char *)dumb_arena_push(arena, s._capacity);
	s._chars[0]  = '\0';

	DUMB_ASSERT(s._chars != NULL)

	return s;
}

Dumb_String
dumb_string_from(Dumb_Arena *arena, const char *str)
{
	Dumb_String s = dumb_string_create_precise(arena, DUMB_DEFAULT_STRING_SIZE_BYTES);

	size_t i = 0;

	while (str[i] != '\0')
	{
		dumb_string_push(arena, &s, (unsigned char)str[i]);
		i++;
	}
	return s;
}

void
dumb_string_clear(Dumb_String *str)
{
	dumb_memset(str->_chars, 0, str->_count);
	str->_count = 0;
}

void
dumb_string_push(Dumb_Arena *arena, Dumb_String *str, unsigned char c)
{
/*
	@NOTE(Honza): We do count + 1 because of compatibility with c-style
	strings, which are ended by '\0'.
	The count we provide for the end user is just the byte count
	of the UTF-8 encoded string, so we need to check for +1 here.
*/
	if ((str->_count + 1) == str->_capacity)
	{
		PRIVATE_dumb_string_change_capacity(arena, str, (str->_capacity * 2));
	}
	str->_chars[str->_count] = c;
	str->_count++;
	str->_chars[str->_count] = '\0'; /* @NOTE(Honza): see the comment at the top of the function. */
}

unsigned char
dumb_string_pop(Dumb_String *str)
{
	unsigned char result;
	size_t index;

	if (str->_count == 0) { return '\0'; }

	index = str->_count - 1;
	result = str->_chars[index];

	str->_chars[index] = '\0';
	str->_count--;

	return result;
}

void
dumb_string_append(Dumb_Arena *arena, Dumb_String *str_a, const char *str_b)
{
	size_t i = 0;

	while (str_b[i] != '\0')
	{
/*
		@NOTE(Honza): We do count + 1 because of compatibility with c-style
		strings, which are ended by '\0'.
		The count we provide for the end user is just the byte count
		of the UTF-8 encoded string, so we need to check for +1 here.
*/
		if ((str_a->_count + 1) == str_a->_capacity)
		{
			PRIVATE_dumb_string_change_capacity(arena, str_a, (str_a->_capacity * 2));
		}
		str_a->_chars[str_a->_count] = (unsigned char)str_b[i];
		str_a->_count++;
		i++;
	}
	str_a->_chars[str_a->_count] = '\0'; /* @NOTE(Honza): see the comment at the top of the function. */
}

Dumb_Array
dumb_string_split_by_char(Dumb_Arena *arena, Dumb_String *str, unsigned char c)
{
	size_t i;

	Dumb_Array    result;
	Dumb_String   buf;
	Dumb_String   buf_2;
	unsigned char current_char;

	result = dumb_array_create(arena, sizeof(Dumb_String));
	buf    = dumb_string_create(arena);


	for (i = 0; i < str->_count; i++)
	{
		current_char = str->_chars[i];

		if (current_char == c)
		{
			buf_2 = dumb_string_from(arena, (char *)buf._chars);
			dumb_array_push(arena, &result, &buf_2);

			buf = dumb_string_create(arena);
		}
		else
		{
			dumb_string_push(arena, &buf, current_char);
		}
	}
	dumb_array_push(arena, &result, &buf);

	return result;
}

void
dumb_string_trim_whitespace(Dumb_String *str)
{
	size_t low_index, high_index;

	low_index = 0;
	high_index = str->_count;

	while ((low_index < str->_count) && (str->_chars[low_index] <= 0x20))
	{
		low_index++;
	}

	while ((high_index > low_index) && (str->_chars[high_index] <= 0x20))
	{
		high_index--;
	}

	str->_count = high_index - low_index + 1;
	dumb_memcpy(str->_chars, (str->_chars + low_index), str->_count);
	/* @NOTE(Honza): Should I set the next bytes to 0 as well? */
	str->_chars[str->_count] = '\0';
}

/*
Returns 0  if the strings are the same.
Returns 1  if str_a is "greater than" str_b.
Returns -1 if str_a is "less than" str_b.

@NOTE(Honza): At least for now, this is a simple byte by byte comparison.
Due to the nature of how UTF-8 strings can be encoded,
two strings that appear identical to the reader may result
in the function returning a non-zero value.
 */
Dumb_Comparison_Result
dumb_string_compare(Dumb_String *str_a, Dumb_String *str_b)
{
	/* @NOTE(Honza): Should this function behave more like
	   string comparison in Swift's stdlib? */
	Dumb_Comparison_Result result;

	if (str_a->_count > str_b->_count)
	{
		result = dumb_memcmp((void *)str_a->_chars, (void *)str_b->_chars, str_b->_count);

		if (result == 0) { return A_GREATER_THEN_B; }
		else             { return result; }
	}
	else if (str_a->_count < str_b->_count)
	{
		result = dumb_memcmp((void *)str_a->_chars, (void *)str_b->_chars, str_a->_count);

		if (result == 0) { return A_LESS_THEN_B; }
		else             { return result; }
	}
	else
	{
		return dumb_memcmp((void *)str_a->_chars, (void *)str_b->_chars, str_a->_count);
	}
}

void
PRIVATE_dumb_string_change_capacity(Dumb_Arena *arena, Dumb_String *str, size_t new_capacity)
{
	void *tmp = dumb_arena_push(arena, new_capacity);

	DUMB_ASSERT(tmp != NULL)

	/* @NOTE(Honza): Should this be count? Or Min(new_capacity, str->_capacity)? */
	dumb_memcpy(tmp, str->_chars, str->_capacity);
	str->_chars = (unsigned char *) tmp;
	str->_capacity = new_capacity;
}

#endif /* DUMB_LIB_IMPLEMENTATION */

#ifdef __cplusplus
} /* extern "C" */
#endif
