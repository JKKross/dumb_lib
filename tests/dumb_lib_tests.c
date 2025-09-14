/* ============================================================================

dumb_lib_tests.c - tests for dumb_lib.h

===============================================================================

version 0.1
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

#define DUMB_PRINT_FAILURE() { printf("\n\tFAIL ON LINE %d;\t", __LINE__); }


void array_add_get_test(void);
void array_add_get_large_test(void);

void string_from_test(void);
void string_new_append_string_test(void);
void string_new_push_pop_test(void);
void string_utf8_test(void);
void string_split_by_char_test(void);
void string_trim_whitespace_test(void);
void string_compare_test(void);

int
main(void) {

	printf("PRINTING SIZES OF THE BASIC TYPES:\n");
	printf("(just to make sure there are no surprises\n");
	printf("when testing with a new compiler or on a new platform)\n");
	printf("\n");
	printf("sizeof(char)      = %zd\n", sizeof(char));
	printf("sizeof(short)     = %zd\n", sizeof(short));
	printf("sizeof(int)       = %zd\n", sizeof(int));
	printf("sizeof(long)      = %zd\n", sizeof(long));
	printf("sizeof(long long) = %zd\n", sizeof(long long));
	printf("\n");
	printf("sizeof(size_t)    = %zd\n", sizeof(size_t));
	printf("sizeof(void *)    = %zd\n", sizeof(void *));
	printf("\n");
	printf("sizeof(float)     = %zd\n", sizeof(float));
	printf("sizeof(double)    = %zd\n", sizeof(double));
	printf("\n");
	printf("============================");
	printf("\n\n");

	array_add_get_test();
	array_add_get_large_test();

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
array_add_get_test(void) {
	int passed = 1;
	printf("Running 'array_add_get_test()'... ");

	#define E 2
	int i;

	Dumb_Array a = dumb_array_init(sizeof(i));

	if (a.count != 0)              { passed = 0; DUMB_PRINT_FAILURE(); }
	if (a._capacity < a.count)     { passed = 0; DUMB_PRINT_FAILURE(); }
	if (a._elem_size != sizeof(i)) { passed = 0; DUMB_PRINT_FAILURE(); }
	if (a._elements == NULL)       { passed = 0; DUMB_PRINT_FAILURE(); }

	for (i = 0; i < 10; i++) {
		int x = i * E;
		dumb_array_add(&a, &x);
		if (a.count != (i + 1))    { passed = 0; DUMB_PRINT_FAILURE(); break; }
		if (a._capacity < a.count) { passed = 0; DUMB_PRINT_FAILURE(); break; }
	}
	if (a._elements == NULL) { passed = 0; DUMB_PRINT_FAILURE(); }

	for (i = 0; i < a.count; i++) {
		int *px = (int *) dumb_array_get(&a, i);
		int x = *px;
		if (x != (i * E)) { passed = 0; DUMB_PRINT_FAILURE(); break; }
	}
	if (a._elements == NULL) { passed = 0; DUMB_PRINT_FAILURE(); }

	dumb_array_free(&a);

	if (a.count      != 0)    { passed = 0; DUMB_PRINT_FAILURE(); }
	if (a._capacity  != 0)    { passed = 0; DUMB_PRINT_FAILURE(); }
	if (a._elements  != NULL) { passed = 0; DUMB_PRINT_FAILURE(); }

	if (passed) { printf("\033[1;32mPASSED\033[0m\n"); }
	else        { printf("\033[1;31mFAILED\033[0m\n"); }
}

void
array_add_get_large_test(void) {
	int passed = 1;
	printf("Running 'array_add_get_large_test()'... ");

	#define C 1048576 /* 1024x1024 */
	int i;
	Dumb_Array a = dumb_array_init(sizeof(i));

	if (a.count != 0)              { passed = 0; DUMB_PRINT_FAILURE(); }
	if (a._capacity < a.count)     { passed = 0; DUMB_PRINT_FAILURE(); }
	if (a._elem_size != sizeof(i)) { passed = 0; DUMB_PRINT_FAILURE(); }
	if (a._elements == NULL)       { passed = 0; DUMB_PRINT_FAILURE(); }

	for (i = 0; i < C; i++) {
		int x = i;
		dumb_array_add(&a, &x);
	}
	for (i = 0; i < a.count; i++) {
		int *px = (int *) dumb_array_get(&a, i);
		int x = *px;

		if (x != i) { passed = 0; DUMB_PRINT_FAILURE(); break; }
	}
	if (a.count      != C)    { passed = 0; DUMB_PRINT_FAILURE(); }
	if (a._capacity   < C)    { passed = 0; DUMB_PRINT_FAILURE(); }
	if (a._elements  == NULL) { passed = 0; DUMB_PRINT_FAILURE(); }

	dumb_array_free(&a);
	if (a.count      != 0)    { passed = 0; DUMB_PRINT_FAILURE(); }
	if (a._capacity  != 0)    { passed = 0; DUMB_PRINT_FAILURE(); }
	if (a._elements  != NULL) { passed = 0; DUMB_PRINT_FAILURE(); }

	if (passed) { printf("\033[1;32mPASSED\033[0m\n"); }
	else        { printf("\033[1;31mFAILED\033[0m\n"); }
}

void
string_from_test(void) {
	int passed = 1;
	printf("Running 'string_from_test()'... ");

	Dumb_String s = dumb_string_from("Hello, World!");

	if (s.chars == NULL)                  { passed = 0; DUMB_PRINT_FAILURE(); }
	if (strcmp(s.chars, "Hello, World!")) { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s.count != 13)                    { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._capacity < s.count) { passed = 0; DUMB_PRINT_FAILURE(); }

	dumb_string_free(&s);
	if (s.chars != NULL)  { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s.count != 0)     { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._capacity != 0) { passed = 0; DUMB_PRINT_FAILURE(); }

	if (passed) { printf("\033[1;32mPASSED\033[0m\n"); }
	else        { printf("\033[1;31mFAILED\033[0m\n"); }
}

void
string_new_append_string_test(void) {
	int passed = 1;
	printf("Running 'string_new_append_string_test()'... ");

	Dumb_String s = dumb_string_new();
	if (s.chars == NULL)       { passed = 0; DUMB_PRINT_FAILURE(); }
	if (strcmp(s.chars, ""))   { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s.count != 0)          { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._capacity < s.count) { passed = 0; DUMB_PRINT_FAILURE(); }

	dumb_string_append(&s, "Hello");
	if (strcmp(s.chars, "Hello")) { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s.count != 5)             { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._capacity < s.count)    { passed = 0; DUMB_PRINT_FAILURE(); }

	dumb_string_append(&s, ", ");
	if (strcmp(s.chars, "Hello, ")) { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s.count != 7)               { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._capacity < s.count)      { passed = 0; DUMB_PRINT_FAILURE(); }

	dumb_string_append(&s, "World!");
	if (strcmp(s.chars, "Hello, World!")) { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s.count != 13)                    { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._capacity < s.count)            { passed = 0; DUMB_PRINT_FAILURE(); }

	dumb_string_free(&s);
	if (s.chars != NULL) { passed = 0; DUMB_PRINT_FAILURE(); }

	if (passed) { printf("\033[1;32mPASSED\033[0m\n"); }
	else        { printf("\033[1;31mFAILED\033[0m\n"); }
}

void
string_new_push_pop_test(void) {
	int passed = 1;
	printf("Running 'string_new_push_pop_test()'... ");

	Dumb_String s = dumb_string_new();
	if (s.chars == NULL)       { passed = 0; DUMB_PRINT_FAILURE(); }
	if (strcmp(s.chars, ""))   { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s.count != 0)          { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._capacity < s.count) { passed = 0; DUMB_PRINT_FAILURE(); }

	dumb_string_push(&s, 'A');
	if (strcmp(s.chars, "A"))  { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s.count != 1)          { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._capacity < s.count) { passed = 0; DUMB_PRINT_FAILURE(); }

	dumb_string_push(&s, 'B');
	if (strcmp(s.chars, "AB")) { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s.count != 2)          { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._capacity < s.count) { passed = 0; DUMB_PRINT_FAILURE(); }

	dumb_string_pop(&s);
	if (strcmp(s.chars, "A"))  { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s.count != 1)          { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._capacity < s.count) { passed = 0; DUMB_PRINT_FAILURE(); }

	dumb_string_pop(&s);
	if (strcmp(s.chars, ""))   { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s.count != 0)          { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._capacity < s.count) { passed = 0; DUMB_PRINT_FAILURE(); }

	dumb_string_pop(&s);
	if (s.chars == NULL)       { passed = 0; DUMB_PRINT_FAILURE(); }
	if (strcmp(s.chars, ""))   { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s.count != 0)          { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._capacity < s.count) { passed = 0; DUMB_PRINT_FAILURE(); }

	dumb_string_pop(&s);
	if (s.chars == NULL)       { passed = 0; DUMB_PRINT_FAILURE(); }
	if (strcmp(s.chars, ""))   { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s.count != 0)          { passed = 0; DUMB_PRINT_FAILURE(); }
 	if (s._capacity < s.count) { passed = 0; DUMB_PRINT_FAILURE(); }

	dumb_string_free(&s);
	if (s.chars != NULL) { passed = 0; DUMB_PRINT_FAILURE(); }

	if (passed) { printf("\033[1;32mPASSED\033[0m\n"); }
	else        { printf("\033[1;31mFAILED\033[0m\n"); }
}

void
string_utf8_test(void) {
	int passed = 1;
	printf("Running 'string_utf8_test()'... ");

	Dumb_String s = dumb_string_from("ě š č ř ž ý á í é ú ů ó ť\nĚ Š Č Ř Ž Ý Á Í É Ú Ů Ó Ť");

#ifdef __cplusplus
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

	int i;
	for (i = 0; i < s.count; i++) {
		if (s.chars[i] != codepoints[i]) { passed = 0; DUMB_PRINT_FAILURE(); break; }
	}

	if (passed) { printf("\033[1;32mPASSED\033[0m\n"); }
	else        { printf("\033[1;31mFAILED\033[0m\n"); }
}

void
string_split_by_char_test(void) {
	int passed = 1;
	printf("Running 'string_split_by_char_test()'... ");

	Dumb_String str = dumb_string_from("Hello there...\nThe name's Bond. James Bond.");
	Dumb_Array strings = dumb_string_split_by_char(&str, ' ');

	char *test_strings[] = {
	          "Hello",
	          "there...\nThe",
	          "name's",
	          "Bond.",
	          "James",
	          "Bond.",
	                       };

	dumb_string_free(&str);

	int i;
	for (i = 0; i < strings.count; i++) {
		Dumb_String *ps = (Dumb_String *) dumb_array_get(&strings, i);
		Dumb_String  s  = *ps;

		int result = strcmp(s.chars, test_strings[i]);
		if (result != 0) { passed = 0; DUMB_PRINT_FAILURE(); break; }

		dumb_string_free(ps);
	}
	dumb_array_free(&strings);

	if (passed) { printf("\033[1;32mPASSED\033[0m\n"); }
	else        { printf("\033[1;31mFAILED\033[0m\n"); }
}

void
string_trim_whitespace_test(void) {
	int passed = 1;
	printf("Running 'string_trim_whitespace_test()'... ");

	/* Part I: */
	Dumb_String str = dumb_string_from("  \t\vHello, sailor!\n \t\n \r\n ");
	dumb_string_push(&str, '\0');
	dumb_string_push(&str, '\n');
	dumb_string_push(&str, ' ');

	dumb_string_trim_whitespace(&str);

	int result = strcmp(str.chars, "Hello, sailor!");
	if (result != 0) { passed = 0; DUMB_PRINT_FAILURE(); }

	dumb_string_free(&str);

	/* Part II: */
	str = dumb_string_new();

	dumb_string_trim_whitespace(&str);

	result = strcmp(str.chars, "");
	if (result != 0) { passed = 0; DUMB_PRINT_FAILURE(); }

	dumb_string_free(&str);
	if (passed) { printf("\033[1;32mPASSED\033[0m\n"); }
	else        { printf("\033[1;31mFAILED\033[0m\n"); }
}

void
string_compare_test(void) {
	int passed;
	int result;
	Dumb_String str_a;
	Dumb_String str_b;

	passed = 1;
	printf("Running 'string_compare_test()'... ");

	/* Part I: */
	str_a = dumb_string_from("Hello, sailor!");
	str_b = dumb_string_from("Hello, sailor!");

	result = dumb_string_compare(&str_a, &str_b);
	if (result != 1) { passed = 0; DUMB_PRINT_FAILURE(); }

	dumb_string_free(&str_a);
	dumb_string_free(&str_b);

	/* Part II: */
	str_a = dumb_string_from("Hello, sailor!");
	str_b = dumb_string_from("Hello, Sailor!");

	result = dumb_string_compare(&str_a, &str_b);
	if (result != 0) { passed = 0; DUMB_PRINT_FAILURE(); }

	dumb_string_free(&str_a);
	dumb_string_free(&str_b);

	/* Part III: */
	str_a = dumb_string_from("Žluťoučký kůň skáče do dáli... 😊");
	str_b = dumb_string_from("Žluťoučký kůň skáče do dáli... 😊");

	result = dumb_string_compare(&str_a, &str_b);
	if (result != 1) { passed = 0; DUMB_PRINT_FAILURE(); }

	dumb_string_free(&str_a);
	dumb_string_free(&str_b);

	if (passed) { printf("\033[1;32mPASSED\033[0m\n"); }
	else        { printf("\033[1;31mFAILED\033[0m\n"); }
}
