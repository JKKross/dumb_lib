/* ============================================================================

dumb_lib_tests.c - tests for dumb_lib.h

===============================================================================

version 0.5.0
Copyright © 2025 Honza Kříž

https://github.com/JKKross
https://x.com/honza_kriz_bass

===============================================================================

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

============================================================================= */

#include <stdio.h>
#include <string.h>

#define DUMB_LIB_IMPLEMENTATION
#include "../src/dumb_lib.h"

#define DUMB_PRINT_FAILURE() { printf("\n - FAIL ON LINE %-34d", __LINE__); }

void mem_test(void);
void arena_test(void);

void array_add_get_test(void);
void array_add_get_large_test(void);
void array_pop_test(void);

void string_from_test(void);
void string_new_append_string_test(void);
void string_new_push_pop_test(void);
void string_utf8_test(void);
void string_split_by_char_test(void);
void string_trim_whitespace_test(void);
void string_compare_test(void);

int
main(void)
{
	mem_test();
	arena_test();

	array_add_get_test();
	array_add_get_large_test();
	array_pop_test();

	string_from_test();
	string_new_append_string_test();
	string_new_push_pop_test();
	string_utf8_test();
	string_split_by_char_test();
	string_trim_whitespace_test();
	string_compare_test();

	return 0;
}

void
mem_test(void)
{
	int passed;

	#define ARRAY_LEN 128
	double arr_1[ARRAY_LEN];
	double arr_2[ARRAY_LEN];

	int i, result;

	unsigned long a, b;

	printf("%-50s", "Running 'mem_test()'... ");

	passed = 1;

	/* PART I: dumb_memcpy */
	for (i = 0; i < ARRAY_LEN; i++)
	{
		arr_1[i] = (double)i * 3.1415;
	}

	dumb_memcpy(arr_2, arr_1, (sizeof(double) * ARRAY_LEN));

	for (i = 0; i < ARRAY_LEN; i++)
	{
		if (arr_1[i] != arr_2[i]) { passed = 0; DUMB_PRINT_FAILURE(); }
	}

	/* PART II: dumb_memcmp */
	result = dumb_memcmp(arr_1, arr_2, (ARRAY_LEN * sizeof(double)));
	if (result != 0) { passed = 0; DUMB_PRINT_FAILURE(); }

	a = 0xFFFFFFFF;
	b = 0xFFF0FFFF;

	result = dumb_memcmp(&a, &b, sizeof(unsigned long));
	if (result != 1) { passed = 0; DUMB_PRINT_FAILURE(); }

	result = dumb_memcmp(&b, &a, sizeof(unsigned long));
	if (result != -1) { passed = 0; DUMB_PRINT_FAILURE(); }

	/* PART III: dumb_memset */
	dumb_memset(arr_1, 0, (ARRAY_LEN * sizeof(double)));

	for (i = 0; i < ARRAY_LEN; i++)
	{
		if (arr_1[i] != 0) { passed = 0; DUMB_PRINT_FAILURE(); }
	}

	if (passed) { printf("%50s", "\033[1;32mPASSED\033[0m\n"); }
	else        { printf("%50s", "\033[1;31mFAILED\033[0m\n"); }
}

void
arena_test(void)
{
	int passed;

	int i;
	Dumb_Arena *arena;
	char *str;

	printf("%-50s", "Running 'arena_test()'... ");

	passed = 1;

	/* PART I: */
	arena = dumb_arena_create(0);
	if (arena->_current->_capacity != DUMB_ARENA_MIN_CAPACITY) { passed = 0; DUMB_PRINT_FAILURE(); }
	if (arena->_current->_position != 0)                       { passed = 0; DUMB_PRINT_FAILURE(); }

	for (i = 0; i < arena->_current->_capacity; i++)
	{
		char current_byte = arena->_current->_memory[i];
		if (current_byte != 0) { passed = 0; DUMB_PRINT_FAILURE(); break; }
	}

	/* PART II: */
	#define MAGICAL_VALUE (DUMB_ARENA_MIN_CAPACITY + 300)
	str = (char *)dumb_arena_push(arena, MAGICAL_VALUE);
	if (arena->_current->_capacity < MAGICAL_VALUE) { passed = 0; DUMB_PRINT_FAILURE(); }
	if (arena->_current->_position < MAGICAL_VALUE) { passed = 0; DUMB_PRINT_FAILURE(); }

	memcpy(str, "Hello, sailor!\n", 15);
	if (memcmp("Hello, sailor!\n", str, 15) != 0) { passed = 0; DUMB_PRINT_FAILURE(); }


	/* PART III: */
	dumb_arena_pop(arena, MAGICAL_VALUE);
	if (arena->_current->_position != 0)          { passed = 0; DUMB_PRINT_FAILURE(); }
	if (memcmp("Hello, sailor!\n", str, 15) == 0) { passed = 0; DUMB_PRINT_FAILURE(); }

	/* PART IV: */
	dumb_arena_destroy(arena);

	if (passed) { printf("%50s", "\033[1;32mPASSED\033[0m\n"); }
	else        { printf("%50s", "\033[1;31mFAILED\033[0m\n"); }
}

void
array_add_get_test(void)
{
	int passed;

	#define E 2
	int i;

	Dumb_Arena *arena;
	Dumb_Array  a;

	printf("%-50s", "Running 'array_add_get_test()'... ");

	passed = 1;
	arena = dumb_arena_create(0);

	/* PART I: dumb_array_init */
	a = dumb_array_init(arena, sizeof(i));
	if (a._count != 0)              { passed = 0; DUMB_PRINT_FAILURE(); }
	if (a._capacity < a._count)     { passed = 0; DUMB_PRINT_FAILURE(); }
	if (a._elem_size != sizeof(i)) { passed = 0; DUMB_PRINT_FAILURE(); }
	if (a._elements == NULL)       { passed = 0; DUMB_PRINT_FAILURE(); }

	/* PART II: dumb_array_add */
	for (i = 0; i < 10; i++)
	{
		int x = i * E;
		dumb_array_add(arena, &a, &x);

		if (a._count != (i + 1))    { passed = 0; DUMB_PRINT_FAILURE(); break; }
		if (a._capacity < a._count) { passed = 0; DUMB_PRINT_FAILURE(); break; }
	}
	if (a._elements == NULL) { passed = 0; DUMB_PRINT_FAILURE(); }

	/* PART III: dumb_array_get */
	for (i = 0; i < a._count; i++)
	{
		int *px = (int *) dumb_array_get(&a, i);
		int x = *px;

		if (x != (i * E)) { passed = 0; DUMB_PRINT_FAILURE(); break; }
	}
	if (a._elements == NULL) { passed = 0; DUMB_PRINT_FAILURE(); }

	/* PART IV: dumb_array_clear */
	size_t array_capacity = a._capacity;
	size_t array_count = a._count;
	dumb_array_clear(&a);

	for (i = 0; i < array_count; i++)
	{
		char *elements_ptr = a._elements;
		int result = *(int *)(elements_ptr + (a._elem_size * i));
		if (result != 0) { passed = 0; DUMB_PRINT_FAILURE(); }
	}
	if (a._count != 0)                  { passed = 0; DUMB_PRINT_FAILURE(); }
	if (a._capacity != array_capacity) { passed = 0; DUMB_PRINT_FAILURE(); }

	dumb_arena_destroy(arena);

	if (passed) { printf("%50s", "\033[1;32mPASSED\033[0m\n"); }
	else        { printf("%50s", "\033[1;31mFAILED\033[0m\n"); }
}

void
array_add_get_large_test(void)
{
	int passed;

	#define COUNT DUMB_MB(300)
	int i;
	int x;

	Dumb_Arena *arena;
	Dumb_Array  a;

	printf("%-50s", "Running 'array_add_get_large_test()'... ");

	passed = 1;
	/* Intentionally 0, to stress-test the arena implementation */
	arena = dumb_arena_create(0);

	/* PART I: dumb_array_init */
	a = dumb_array_init_precise(arena, sizeof(i), COUNT);
	if (a._count != 0)              { passed = 0; DUMB_PRINT_FAILURE(); }
	if (a._capacity < a._count)     { passed = 0; DUMB_PRINT_FAILURE(); }
	if (a._elem_size != sizeof(i)) { passed = 0; DUMB_PRINT_FAILURE(); }
	if (a._elements == NULL)       { passed = 0; DUMB_PRINT_FAILURE(); }

	/* PART II: dumb_array_add */
	for (i = 0; i < COUNT; i++)
	{
		x = i;
		dumb_array_add(arena, &a, &x);
	}

	/* PART III: dumb_array_get */
	for (i = 0; i < a._count; i++)
	{
		int *px = (int *) dumb_array_get(&a, i);
		int x = *px;

		if (x != i) { passed = 0; DUMB_PRINT_FAILURE(); break; }
	}

	if (a._count      != COUNT) { passed = 0; DUMB_PRINT_FAILURE(); }
	if (a._capacity   < COUNT) { passed = 0; DUMB_PRINT_FAILURE(); }
	if (a._elements  == NULL)  { passed = 0; DUMB_PRINT_FAILURE(); }

	/* PART IV: dumb_array_clear */
	size_t array_capacity = a._capacity;
	size_t array_count = a._count;
	dumb_array_clear(&a);

	for (i = 0; i < array_count; i++)
	{
		char *elements_ptr = a._elements;
		int result = *(int *)(elements_ptr + (a._elem_size * i));
		if (result != 0) { passed = 0; DUMB_PRINT_FAILURE(); }
	}
	if (a._count != 0)                  { passed = 0; DUMB_PRINT_FAILURE(); }
	if (a._capacity != array_capacity) { passed = 0; DUMB_PRINT_FAILURE(); }

	dumb_arena_destroy(arena);

	if (passed) { printf("%50s", "\033[1;32mPASSED\033[0m\n"); }
	else        { printf("%50s", "\033[1;31mFAILED\033[0m\n"); }
}

void
array_pop_test(void)
{
	int passed;

	int i;
	float x;
	float float_buf;

	Dumb_Arena *arena;
	Dumb_Array  arr;

	printf("%-50s", "Running 'array_pop_test()'... ");

	passed = 1;
	arena = dumb_arena_create(0);

	/* PART I: dumb_array_init */
	arr = dumb_array_init(arena, sizeof(x));
	if (arr._count != 0)              { passed = 0; DUMB_PRINT_FAILURE(); }
	if (arr._capacity < arr._count)     { passed = 0; DUMB_PRINT_FAILURE(); }
	if (arr._elem_size != sizeof(x)) { passed = 0; DUMB_PRINT_FAILURE(); }
	if (arr._elements == NULL)       { passed = 0; DUMB_PRINT_FAILURE(); }

	/* PART II: dumb_array_add */
	for (i = 0; i < 128; i++)
	{
		x = i * 3.14;
		dumb_array_add(arena, &arr, &x);

		if (arr._count != (i + 1))      { passed = 0; DUMB_PRINT_FAILURE(); break; }
		if (arr._capacity < arr._count) { passed = 0; DUMB_PRINT_FAILURE(); break; }
	}
	if (arr._elements == NULL) { passed = 0; DUMB_PRINT_FAILURE(); }

	/* PART III: dumb_array_pop */
	dumb_array_pop(&arr, (char *)&float_buf);

	if (float_buf != x)       { passed = 0; DUMB_PRINT_FAILURE(); }
	if (arr._count != (i - 1)) { passed = 0; DUMB_PRINT_FAILURE(); }

	dumb_arena_destroy(arena);

	if (passed) { printf("%50s", "\033[1;32mPASSED\033[0m\n"); }
	else        { printf("%50s", "\033[1;31mFAILED\033[0m\n"); }
}

void
string_from_test(void)
{
	int passed;

	Dumb_Arena  *arena;
	Dumb_String  s;

	printf("%-50s", "Running 'string_from_test()'... ");

	passed = 1;
	arena = dumb_arena_create(0);

	s = dumb_string_from(arena, "Hello, World!");
	if (s.chars == NULL)                  { passed = 0; DUMB_PRINT_FAILURE(); }
	if (strcmp(s.chars, "Hello, World!")) { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._count != 13)                    { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._capacity < s._count) { passed = 0; DUMB_PRINT_FAILURE(); }

	dumb_arena_destroy(arena);

	if (passed) { printf("%50s", "\033[1;32mPASSED\033[0m\n"); }
	else        { printf("%50s", "\033[1;31mFAILED\033[0m\n"); }
}

void
string_new_append_string_test(void)
{
	int passed;

	size_t i;

	Dumb_Arena  *arena;
	Dumb_String  s;

	printf("%-50s", "Running 'string_new_append_string_test()'... ");

	passed = 1;
	arena = dumb_arena_create(0);

	/* PART I: dumb_string_new */
	s = dumb_string_new(arena);
	if (s.chars == NULL)       { passed = 0; DUMB_PRINT_FAILURE(); }
	if (strcmp(s.chars, ""))   { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._count != 0)          { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._capacity < s._count) { passed = 0; DUMB_PRINT_FAILURE(); }

	/* PART II: dumb_string_append */
	dumb_string_append(arena, &s, "Hello");
	if (strcmp(s.chars, "Hello")) { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._count != 5)             { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._capacity < s._count)    { passed = 0; DUMB_PRINT_FAILURE(); }

	dumb_string_append(arena, &s, ", ");
	if (strcmp(s.chars, "Hello, ")) { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._count != 7)               { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._capacity < s._count)      { passed = 0; DUMB_PRINT_FAILURE(); }

	dumb_string_append(arena, &s, "World!");
	if (strcmp(s.chars, "Hello, World!")) { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._count != 13)                    { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._capacity < s._count)            { passed = 0; DUMB_PRINT_FAILURE(); }

	/* PART III: dumb_string_clear */
	size_t string_capacity = s._capacity;
	dumb_string_clear(&s);

	for (i = 0; i < s._capacity; i++)
	{
		if (s.chars[i] != 0) { passed = 0; DUMB_PRINT_FAILURE(); }
	}
	if (s._count != 0)                   { passed = 0; DUMB_PRINT_FAILURE(); }
	if (s._capacity != string_capacity) { passed = 0; DUMB_PRINT_FAILURE(); }

	dumb_arena_destroy(arena);

	if (passed) { printf("%50s", "\033[1;32mPASSED\033[0m\n"); }
	else        { printf("%50s", "\033[1;31mFAILED\033[0m\n"); }
}

void
string_new_push_pop_test(void)
{
	int passed;

	size_t i;

	Dumb_Arena  *arena;
	Dumb_String  s;

	printf("%-50s", "Running 'string_new_push_pop_test()'... ");

	passed = 1;
	arena = dumb_arena_create(0);

	/* PART I: dumb_string_new */
	s = dumb_string_new(arena);

	if (s.chars == NULL)       { passed = 0; DUMB_PRINT_FAILURE(); }
	if (strcmp(s.chars, ""))   { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._count != 0)          { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._capacity < s._count) { passed = 0; DUMB_PRINT_FAILURE(); }

	/* PART II: dumb_string_push */
	dumb_string_push(arena, &s, 'A');
	if (strcmp(s.chars, "A"))  { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._count != 1)          { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._capacity < s._count) { passed = 0; DUMB_PRINT_FAILURE(); }

	dumb_string_push(arena, &s, 'B');
	if (strcmp(s.chars, "AB")) { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._count != 2)          { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._capacity < s._count) { passed = 0; DUMB_PRINT_FAILURE(); }

	/* PART III: dumb_string_pop */
	dumb_string_pop(&s);
	if (strcmp(s.chars, "A"))  { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._count != 1)          { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._capacity < s._count) { passed = 0; DUMB_PRINT_FAILURE(); }

	dumb_string_pop(&s);
	if (strcmp(s.chars, ""))   { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._count != 0)          { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._capacity < s._count) { passed = 0; DUMB_PRINT_FAILURE(); }

	dumb_string_pop(&s);
	if (s.chars == NULL)       { passed = 0; DUMB_PRINT_FAILURE(); }
	if (strcmp(s.chars, ""))   { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._count != 0)          { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._capacity < s._count) { passed = 0; DUMB_PRINT_FAILURE(); }

	dumb_string_pop(&s);
	if (s.chars == NULL)       { passed = 0; DUMB_PRINT_FAILURE(); }
	if (strcmp(s.chars, ""))   { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._count != 0)          { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._capacity < s._count) { passed = 0; DUMB_PRINT_FAILURE(); }

	/* PART IV: dumb_string_clear */
	size_t string_capacity = s._capacity;
	dumb_string_clear(&s);

	for (i = 0; i < s._capacity; i++)
	{
		if (s.chars[i] != 0) { passed = 0; DUMB_PRINT_FAILURE(); }
	}
	if (s._count != 0)                   { passed = 0; DUMB_PRINT_FAILURE(); }
	if (s._capacity != string_capacity) { passed = 0; DUMB_PRINT_FAILURE(); }

	dumb_arena_destroy(arena);

	if (passed) { printf("%50s", "\033[1;32mPASSED\033[0m\n"); }
	else        { printf("%50s", "\033[1;31mFAILED\033[0m\n"); }
}

void
string_utf8_test(void)
{
	int passed;

	Dumb_Arena  *arena;
	Dumb_String  s;
	int i;

	printf("%-50s", "Running 'string_utf8_test()'... ");

	passed = 1;
	arena = dumb_arena_create(0);

	s = dumb_string_from(arena, "ě š č ř ž ý á í é ú ů ó ť\nĚ Š Č Ř Ž Ý Á Í É Ú Ů Ó Ť");

#ifdef WDD_C_PLUS_PLUS_TESTS
/* Almost forgot how great C++ is, how much more productive it makes you & how incredibly readable it is! Sigh... */
	char codepoints[] = {
	         static_cast<signed char>(0xc4), static_cast<signed char>(0x9b), static_cast<signed char>(0x20), static_cast<signed char>(0xc5), static_cast<signed char>(0xa1),
	         static_cast<signed char>(0x20), static_cast<signed char>(0xc4), static_cast<signed char>(0x8d), static_cast<signed char>(0x20), static_cast<signed char>(0xc5),
	         static_cast<signed char>(0x99), static_cast<signed char>(0x20), static_cast<signed char>(0xc5), static_cast<signed char>(0xbe), static_cast<signed char>(0x20),
	         static_cast<signed char>(0xc3), static_cast<signed char>(0xbd), static_cast<signed char>(0x20), static_cast<signed char>(0xc3), static_cast<signed char>(0xa1),
	         static_cast<signed char>(0x20), static_cast<signed char>(0xc3), static_cast<signed char>(0xad), static_cast<signed char>(0x20), static_cast<signed char>(0xc3),
	         static_cast<signed char>(0xa9), static_cast<signed char>(0x20), static_cast<signed char>(0xc3), static_cast<signed char>(0xba), static_cast<signed char>(0x20),
	         static_cast<signed char>(0xc5), static_cast<signed char>(0xaf), static_cast<signed char>(0x20), static_cast<signed char>(0xc3), static_cast<signed char>(0xb3),
	         static_cast<signed char>(0x20), static_cast<signed char>(0xc5), static_cast<signed char>(0xa5), static_cast<signed char>(0x0a), static_cast<signed char>(0xc4),
	         static_cast<signed char>(0x9a), static_cast<signed char>(0x20), static_cast<signed char>(0xc5), static_cast<signed char>(0xa0), static_cast<signed char>(0x20),
	         static_cast<signed char>(0xc4), static_cast<signed char>(0x8c), static_cast<signed char>(0x20), static_cast<signed char>(0xc5), static_cast<signed char>(0x98),
	         static_cast<signed char>(0x20), static_cast<signed char>(0xc5), static_cast<signed char>(0xbd), static_cast<signed char>(0x20), static_cast<signed char>(0xc3),
	         static_cast<signed char>(0x9d), static_cast<signed char>(0x20), static_cast<signed char>(0xc3), static_cast<signed char>(0x81), static_cast<signed char>(0x20),
	         static_cast<signed char>(0xc3), static_cast<signed char>(0x8d), static_cast<signed char>(0x20), static_cast<signed char>(0xc3), static_cast<signed char>(0x89),
	         static_cast<signed char>(0x20), static_cast<signed char>(0xc3), static_cast<signed char>(0x9a), static_cast<signed char>(0x20), static_cast<signed char>(0xc5),
	         static_cast<signed char>(0xae), static_cast<signed char>(0x20), static_cast<signed char>(0xc3), static_cast<signed char>(0x93), static_cast<signed char>(0x20),
	         static_cast<signed char>(0xc5), static_cast<signed char>(0xa4),
	                    };
#else
	char codepoints[] = {
	         0xc4, 0x9b, 0x20, 0xc5, 0xa1,
	         0x20, 0xc4, 0x8d, 0x20, 0xc5,
	         0x99, 0x20, 0xc5, 0xbe, 0x20,
	         0xc3, 0xbd, 0x20, 0xc3, 0xa1,
	         0x20, 0xc3, 0xad, 0x20, 0xc3,
	         0xa9, 0x20, 0xc3, 0xba, 0x20,
	         0xc5, 0xaf, 0x20, 0xc3, 0xb3,
	         0x20, 0xc5, 0xa5, 0x0a, 0xc4,
	         0x9a, 0x20, 0xc5, 0xa0, 0x20,
	         0xc4, 0x8c, 0x20, 0xc5, 0x98,
	         0x20, 0xc5, 0xbd, 0x20, 0xc3,
	         0x9d, 0x20, 0xc3, 0x81, 0x20,
	         0xc3, 0x8d, 0x20, 0xc3, 0x89,
	         0x20, 0xc3, 0x9a, 0x20, 0xc5,
	         0xae, 0x20, 0xc3, 0x93, 0x20,
	         0xc5, 0xa4,
	                    };
#endif

	for (i = 0; i < s._count; i++)
	{
		if (s.chars[i] != codepoints[i]) { passed = 0; DUMB_PRINT_FAILURE(); break; }
	}

	dumb_arena_destroy(arena);

	if (passed) { printf("%50s", "\033[1;32mPASSED\033[0m\n"); }
	else        { printf("%50s", "\033[1;31mFAILED\033[0m\n"); }
}

void
string_split_by_char_test(void)
{
	int passed;

	Dumb_Arena  *arena;
	Dumb_String  str;
	Dumb_Array   strings;
	int i;

	printf("%-50s", "Running 'string_split_by_char_test()'... ");

	passed = 1;

	arena = dumb_arena_create(0);

	str = dumb_string_from(arena, "Hello there...\nThe name's Bond. James Bond.");
	strings = dumb_string_split_by_char(arena, &str, ' ');

	char *test_strings[] =
	{
		"Hello",
		"there...\nThe",
		"name's",
		"Bond.",
		"James",
		"Bond.",
	};

	for (i = 0; i < strings._count; i++)
	{
		Dumb_String *ps = (Dumb_String *) dumb_array_get(&strings, i);
		Dumb_String  s  = *ps;

		int result = strcmp(s.chars, test_strings[i]);
		if (result != 0) { passed = 0; DUMB_PRINT_FAILURE(); break; }
	}

	dumb_arena_destroy(arena);

	if (passed) { printf("%50s", "\033[1;32mPASSED\033[0m\n"); }
	else        { printf("%50s", "\033[1;31mFAILED\033[0m\n"); }
}

void
string_trim_whitespace_test(void)
{
	int passed;

	Dumb_Arena  *arena;
	Dumb_String  str;

	printf("%-50s", "Running 'string_trim_whitespace_test()'... ");

	passed = 1;
	arena = dumb_arena_create(0);

	/* Part I: */
	str = dumb_string_from(arena, "  \t\vHello, sailor!\n \t\n \r\n ");
	dumb_string_push(arena, &str, '\0');
	dumb_string_push(arena, &str, '\n');
	dumb_string_push(arena, &str, ' ');

	dumb_string_trim_whitespace(&str);

	int result = strcmp(str.chars, "Hello, sailor!");
	if (result != 0) { passed = 0; DUMB_PRINT_FAILURE(); }

	/* Part II: */
	str = dumb_string_new(arena);

	dumb_string_trim_whitespace(&str);

	result = strcmp(str.chars, "");
	if (result != 0) { passed = 0; DUMB_PRINT_FAILURE(); }

	dumb_arena_destroy(arena);

	if (passed) { printf("%50s", "\033[1;32mPASSED\033[0m\n"); }
	else        { printf("%50s", "\033[1;31mFAILED\033[0m\n"); }
}

void
string_compare_test(void)
{
	int passed;

	Dumb_Arena *arena;
	int result;
	Dumb_String str_a;
	Dumb_String str_b;

	printf("%-50s", "Running 'string_compare_test()'... ");

	passed = 1;
	arena = dumb_arena_create(0);

	/* Part I: */
	str_a = dumb_string_from(arena, "Hello, sailor!");
	str_b = dumb_string_from(arena, "Hello, sailor!");

	result = dumb_string_compare(&str_a, &str_b);
	if (result != 1) { passed = 0; DUMB_PRINT_FAILURE(); }

	/* Part II: */
	str_a = dumb_string_from(arena, "Hello, sailor!");
	str_b = dumb_string_from(arena, "Hello, Sailor!");

	result = dumb_string_compare(&str_a, &str_b);
	if (result != 0) { passed = 0; DUMB_PRINT_FAILURE(); }

	/* Part III: */
	str_a = dumb_string_from(arena, "Žluťoučký kůň skáče do dáli... 😊");
	str_b = dumb_string_from(arena, "Žluťoučký kůň skáče do dáli... 😊");

	result = dumb_string_compare(&str_a, &str_b);
	if (result != 1) { passed = 0; DUMB_PRINT_FAILURE(); }

	dumb_arena_destroy(arena);

	if (passed) { printf("%50s", "\033[1;32mPASSED\033[0m\n"); }
	else        { printf("%50s", "\033[1;31mFAILED\033[0m\n"); }
}
