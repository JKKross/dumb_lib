/* ============================================================================

dumb_lib.h - something like my personal "standard library"/"C extension".

===============================================================================

version 0.2.1
Copyright © 2025 Honza Kříž

https://github.com/JKKross
https://x.com/honza_kriz_bass

===============================================================================

	Index of this file
	------------------

	|SECTION| - LICENSE
	|SECTION| - README
	|SECTION| - NOTES
	|SECTION| - DOCUMENTATION
	|SECTION| - COMPILE OPTIONS
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

	|SECTION| - README
	----------------------------

This is a single-file (or "header-only") MIT licensed library for C/C++,
meant primarily for:
	A) My personal use, as a "C extension"/my own "standard library"
	B) My own learning purposes

That means that the library is still very much "under construction",
and also, that I will most likely not accept any pull requests.
(I may change my mind on that, obviously)

Thus, I ask you: if you want to use it, READ THROUGH THE CODE, VALIDATE IT,
TEST IT OUT YOURSELF AND TAKE THE "NO WARRANTY..." PART OF THE LICENSE SERIOUSLY.

(For this reason, I may never take this lib over the v1.0 mark - we'll see)

===============================================================================

	|SECTION| - NOTES
	----------------------------

- Originally, I pretty much tried to copy the stb libraries style (i.e. "define
your own allocator" etc.), but since this library is meant basically just for
my own use, I decided to make it more opinionated.

Once again, that basically means: DO NOT USE THIS LIBRARY!
Of course you are free to do so, or to read through the source, yank the parts
you like out etc.


- If you're wondering why there are no "//" comments, it's because they are not part
of the C89 standard. For rationale to comply with C89 see:

	https://www.dependablec.org/


- If you are wondering why I decided to use single-header style for the library, see:

	https://github.com/nothings/stb?tab=readme-ov-file#why-single-file-headers

===============================================================================

	|SECTION| - DOCUMENTATION
	----------------------------

You MUST '#define DUMB_LIB_IMPLEMENTATION'
in EXACTLY _one_ C or C++ file that includes this header, BEFORE the
include, like this:

   #define DUMB_LIB_IMPLEMENTATION
   #include "dumb_lib.h"

All other files should just #include "dumb_lib.h" without the #define.

============================================================================ */

/*
	|SECTION| - COMPILE OPTIONS
	---------------------------
*/

#define DUMB_DEBUG 1

/*
	|SECTION| - COMPILE CHECKS
	--------------------------
*/

#if !(__GNUC__ || __CLANG__ || _MSC_VER || __TINYC__ )
	#error YOU ARE USING A COMPILER THIS LIBRARY WAS NOT TESTED WITH! \
If you wish to proceed, read through the source & make sure everything works correctly!
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
#define DUMB_ARENA_MIN_CAPACITY         1024
#define DUMB_DEFAULT_ARRAY_SIZE         256
#define DUMB_DEFAULT_STRING_SIZE_BYTES  32

/* Math constants */
#define DUMB_PI   3.14159265358979323846
#define DUMB_TAU  6.28318530717958647693

/* In case you need to make sure you get a float instead of a double: */
#define DUMB_PI_32   3.14159265358979323846f
#define DUMB_TAU_32  6.28318530717958647693f

/* --- |MACROS| --- */

#ifdef DUMB_DEBUG
	/*
	   @NOTE(Honza): If the assert fails, we crash, and can see where in a debugger.
	   No need to get fancier - at least for now.
	*/
	#define DUMB_ASSERT(condition) if (!(condition)) { *(int *)0 = 0; }
#else
	/* Do nothing in release builds. */
	#define DUMB_ASSERT(condition)
#endif

#define DUMB_ALLOC_CONTEXT NULL
#define DUMB_ALLOC(context, size) malloc(size)
#define DUMB_FREE(context, ptr) free(ptr)

/* --- |TYPES| --- */

typedef struct Dumb_Arena {
	size_t  _capacity;
	size_t  _position;
	char   *_memory;
} Dumb_Arena;

typedef struct Dumb_Array {
	size_t  count;
	size_t  _capacity;
	size_t  _elem_size;
	void   *_elements;
} Dumb_Array; /* @NOTE(Honza): Switch to macro approach? */

typedef struct Dumb_String {
	char   *chars;
	size_t  count;
	size_t  _capacity;
} Dumb_String;

/* --- |MEMORY| --- */

void
dumb_memcpy(void *to, void *from, size_t num_bytes);

int
dumb_memcmp(void *a, void *b, size_t num_bytes);

Dumb_Arena
dumb_arena_create(size_t size);

void
dumb_arena_destroy(Dumb_Arena *arena);

void *
dumb_arena_push(Dumb_Arena *arena, size_t size);

void
dumb_arena_pop(Dumb_Arena *arena, size_t size);

/* --- |ARRAY| --- */

Dumb_Array
dumb_array_init(size_t elem_size);

Dumb_Array
dumb_array_init_precise(size_t elem_size, size_t number_of_elems);

void
dumb_array_free(Dumb_Array *a);

void
dumb_array_add(Dumb_Array *a, void *new_elem);

void *
dumb_array_get(Dumb_Array *a, size_t index);

/* --- |STRING| --- */

Dumb_String
dumb_string_new(void);

Dumb_String
dumb_string_new_precise(size_t capacity);

Dumb_String
dumb_string_from(const char *str);

void
dumb_string_free(Dumb_String *str);

void
dumb_string_push(Dumb_String *str, char c);

char
dumb_string_pop(Dumb_String *str);

void
dumb_string_append(Dumb_String *str_a, const char *str_b);

Dumb_Array
dumb_string_split_by_char(Dumb_String *str, char c);

void
dumb_string_trim_whitespace(Dumb_String *str);

/*
Returns 1 if the strings are the same, returns 0 if they differ.

@NOTE(Honza): At least for now, this is a simple byte by byte comparison.
Due to the nature of how UTF-8 strings can be encoded,
two strings that appear identical to the reader may result
in the function returning '0'.
 */
int
dumb_string_compare(Dumb_String *str_a, Dumb_String *str_b);

void
PRIVATE_dumb_string_change_capacity(Dumb_String *str, size_t new_capacity);

/* --- |RANDOM| --- */


/* --- |FILE OPERATIONS| --- */

/*
	|SECTION| - IMPLEMENTATION
	--------------------------
*/

#ifdef DUMB_LIB_IMPLEMENTATION

/* --- |MEMORY IMPLEMENTATION| --- */

void
dumb_memcpy(void *to, void *from, size_t num_bytes) {
	size_t i;
	char *to_char   = (char *) to;
	char *from_char = (char *) from;

	for (i = 0; i < num_bytes; i++) {
		to_char[i] = from_char[i];
	}
}

int
dumb_memcmp(void *a, void *b, size_t num_bytes) {
	size_t i;
	char *aa = (char *) a;
	char *bb = (char *) b;

	for (i = 0; i < num_bytes; i++) {
		if      (aa[i] > bb[i]) { return 1; }
		else if (aa[i] < bb[i]) { return -1; }
	}
	return 0;
}

Dumb_Arena
dumb_arena_create(size_t size)
{
	size_t capacity;
	Dumb_Arena new_arena;

	/*
	   @NOTE(Honza): Is this a good idea?

	   On one hand, in most situations, if you need
	   less memory than DUMB_ARENA_MIN_CAPACITY,
	   you might as well use the stack or malloc/calloc.

	   On the other hand, this might be a surprise for the user.
	*/
	if (size < DUMB_ARENA_MIN_CAPACITY) { capacity = DUMB_ARENA_MIN_CAPACITY; }
	else { capacity = size; }

	new_arena._capacity = capacity;
	new_arena._position = 0;
	new_arena._memory   = (char *) calloc(capacity, sizeof(char));

	DUMB_ASSERT(new_arena._memory != NULL)

	return new_arena;
}

void
dumb_arena_destroy(Dumb_Arena *arena)
{
	arena->_capacity = 0;
	arena->_position = 0;
	free(arena->_memory);
	arena->_memory = NULL;
}

void *
dumb_arena_push(Dumb_Arena *arena, size_t size)
{
	size_t old_position;
	size_t new_position;

	old_position = arena->_position;
	new_position = arena->_position + size;

	if (new_position > arena->_capacity)
	{
		char *buf;

		arena->_capacity = arena->_capacity * 2;
		buf = calloc(arena->_capacity, sizeof(char));

		dumb_memcpy(buf, arena->_memory, old_position);

		free(arena->_memory);

		arena->_memory = buf;
	}
	arena->_position = new_position;
	return arena->_memory + old_position;
}

void
dumb_arena_pop(Dumb_Arena *arena, size_t size)
{
	size_t i;
	size_t original_position;
	size_t new_position;

	/* @NOTE(Honza): Check always? */
	DUMB_ASSERT(size <= arena->_position)

	original_position = arena->_position;
	new_position = original_position - size;

	arena->_position = new_position;

	for (i = arena->_position; i < original_position; i++)
	{
		arena->_memory[i] = 0;
	}
}

/* --- |ARRAY IMPLEMENTATION| --- */

Dumb_Array
dumb_array_init(size_t elem_size) {
	return dumb_array_init_precise(elem_size, DUMB_DEFAULT_ARRAY_SIZE);
}

Dumb_Array
dumb_array_init_precise(size_t elem_size, size_t number_of_elems) {
	Dumb_Array a;

	if (number_of_elems < 1) { number_of_elems = 1; }

	a.count      = 0;
	a._capacity  = elem_size * number_of_elems;
	a._elem_size = elem_size;
	a._elements  = DUMB_ALLOC(&DUMB_ALLOC_CONTEXT, a._capacity);

	/* @NOTE(Honza): Maybe check always? */
	DUMB_ASSERT(a._elements != NULL)

	return a;
}

void
dumb_array_free(Dumb_Array *a) {
	a->_capacity  = 0;
	a->count      = 0;
	a->_elem_size = 0;

	/* @NOTE(Honza): Maybe check always? */
	DUMB_ASSERT(a->_elements != NULL)

	DUMB_FREE(&DUMB_ALLOC_CONTEXT, a->_elements);
	a->_elements = NULL;
}

void
dumb_array_add(Dumb_Array *a, void *new_elem) {
	char *new_elem_destination;

	if ((a->count * a->_elem_size) == a->_capacity) {
		size_t new_capacity = a->_capacity * 2;
		void *tmp = DUMB_ALLOC(&DUMB_ALLOC_CONTEXT, new_capacity);

		/* @NOTE(Honza): Maybe check always? */
		DUMB_ASSERT(tmp != NULL)

		dumb_memcpy(tmp, a->_elements, a->_capacity);
		DUMB_FREE(&DUMB_ALLOC_CONTEXT, a->_elements);
		a->_elements = tmp;
		a->_capacity = new_capacity;
	}

	new_elem_destination = (char *) a->_elements + (a->count * a->_elem_size);
	dumb_memcpy(new_elem_destination, new_elem, a->_elem_size);
	a->count++;
}

void *
dumb_array_get(Dumb_Array *a, size_t index) {
	char *result;

	/* @NOTE(Honza): Maybe check always? */
	DUMB_ASSERT(index < a->count)

	result = (char *) a->_elements + (index * a->_elem_size);
	return (void *) result;
}

/* --- |STRING IMPLEMENTATION| --- */

Dumb_String
dumb_string_new(void) {
	return dumb_string_new_precise(DUMB_DEFAULT_STRING_SIZE_BYTES);
}

Dumb_String
dumb_string_new_precise(size_t capacity) {
	Dumb_String s;

	if (capacity < 2) { capacity = 2; }

	s.count     = 0;
	s._capacity = capacity;
	s.chars     = (char *) DUMB_ALLOC(&DUMB_ALLOC_CONTEXT, s._capacity);
/*
	'malloc' doesn't initialize the memory,
	so we do this to prevent weird interop issues with c strings.
	Still costs a few instructions, but it should be less than 'calloc'.
*/
	s.chars[0]  = '\0';

	/* @NOTE(Honza): Maybe check always? */
	DUMB_ASSERT(s.chars != NULL)

	return s;
}

Dumb_String
dumb_string_from(const char *str) {
	Dumb_String s = dumb_string_new_precise(DUMB_DEFAULT_STRING_SIZE_BYTES);

	size_t i = 0;

	while (str[i] != '\0') {
		dumb_string_push(&s, str[i]);
		i++;
	}
	return s;
}

void
dumb_string_free(Dumb_String *str) {
	str->_capacity = 0;
	str->count     = 0;

	/* @NOTE(Honza): Maybe check always? */
	DUMB_ASSERT(str->chars != NULL)

	DUMB_FREE(&DUMB_ALLOC_CONTEXT, str->chars);
	str->chars = NULL;
}

void
dumb_string_push(Dumb_String *str, char c) {
/*
	@NOTE(Honza): We do count + 1 because of compatibility with c-style
	strings, which are ended by '\0'.
	The count we provide for the end user is just the byte count
	of the UTF-8 encoded string, so we need to check for +1 here.
*/
	if ((str->count + 1) == str->_capacity) {
		PRIVATE_dumb_string_change_capacity(str, (str->_capacity * 2));
	}
	str->chars[str->count] = c;
	str->count++;
	str->chars[str->count] = '\0'; /* @NOTE(Honza): see the comment at the top of the function. */
}

char
dumb_string_pop(Dumb_String *str) {
	char result;
	size_t index;

	if (str->count == 0) { return '\0'; }

	index = str->count - 1;
	result = str->chars[index];

	str->chars[index] = '\0';
	str->count--;

	return result;
}

void
dumb_string_append(Dumb_String *str_a, const char *str_b) {
	size_t i = 0;

	while (str_b[i] != '\0') {
/*
		@NOTE(Honza): We do count + 1 because of compatibility with c-style
		strings, which are ended by '\0'.
		The count we provide for the end user is just the byte count
		of the UTF-8 encoded string, so we need to check for +1 here.
*/
		if ((str_a->count + 1) == str_a->_capacity) {
			PRIVATE_dumb_string_change_capacity(str_a, (str_a->_capacity * 2));
		}
		str_a->chars[str_a->count] = str_b[i];
		str_a->count++;
		i++;
	}
	str_a->chars[str_a->count] = '\0'; /* @NOTE(Honza): see the comment at the top of the function. */
}

Dumb_Array
dumb_string_split_by_char(Dumb_String *str, char c) {
	Dumb_Array  result = dumb_array_init(sizeof(Dumb_String));
	Dumb_String buf    = dumb_string_new();

	size_t i;

	for (i = 0; i < str->count; i++) {
		char current = str->chars[i];

		if (current == c) {
			Dumb_String buf_2 = dumb_string_from(buf.chars);
			dumb_array_add(&result, &buf_2);

			dumb_string_free(&buf);
			buf = dumb_string_new();
		} else {
			dumb_string_push(&buf, current);
		}
	}
	dumb_array_add(&result, &buf);

	return result;
}

void
dumb_string_trim_whitespace(Dumb_String *str) {
	void        *copy_to;
	void        *copy_from;
	size_t       count;
	Dumb_String  new_string;

	size_t low_index  = 0;
	size_t high_index = str->count - 1;

	while ((low_index < str->count) && (str->chars[low_index] <= 0x20)) {
		low_index++;
	}

	if (low_index >= str->count) {
		Dumb_String empty = dumb_string_new();

		dumb_string_free(str);
		dumb_memcpy(str, &empty, sizeof(Dumb_String));
		return;
	}

	while ((high_index > low_index) && (str->chars[high_index] <= 0x20)) {
		high_index--;
	}

	/* +1 because indices */
	count = high_index - low_index + 1;
	/* +1 because of how our strings work - see 'dumb_string_push' for details. */
	new_string = dumb_string_new_precise(count + 1);

	copy_to   = (void *) new_string.chars;
	copy_from = (void *) (str->chars + low_index);
	dumb_memcpy(copy_to, copy_from, count);
	new_string.count = count;
	new_string.chars[count] = '\0';

	dumb_string_free(str);
	dumb_memcpy(str, &new_string, sizeof(Dumb_String));
}

int
dumb_string_compare(Dumb_String *str_a, Dumb_String *str_b) {
	/* @NOTE(Honza): Should this function behave more like
	   string comparison in Swift's stdlib? */
	int result;

	if (str_a->count != str_b->count) { return 0; }


	result = dumb_memcmp((void *)str_a->chars,
	                     (void *)str_b->chars,
	                     str_a->count);

	/* @NOTE(Honza): dumb_memcp returns 1 or -1
	   if the memory differs and 0 if it's the same */
	if (result == 0) { return 1; }
	else             { return 0; }
}

void
PRIVATE_dumb_string_change_capacity(Dumb_String *str, size_t new_capacity) {
	void *tmp = DUMB_ALLOC(&DUMB_ALLOC_CONTEXT, new_capacity);

	/* @NOTE(Honza): Maybe check always? */
	DUMB_ASSERT(tmp != NULL)

	/* @NOTE(Honza): Should this be count? Or Min(new_capacity, str->_capacity)? */
	dumb_memcpy(tmp, str->chars, str->_capacity);
	DUMB_FREE(&DUMB_ALLOC_CONTEXT, str->chars);
	str->chars = (char *) tmp;
	str->_capacity = new_capacity;
}

/* --- |RANDOM IMPLEMENTATION| --- */

/* --- |FILE OPERATIONS IMPLEMENTATION| --- */

#endif /* DUMB_LIB_IMPLEMENTATION */

#ifdef __cplusplus
} /* extern "C" */
#endif
