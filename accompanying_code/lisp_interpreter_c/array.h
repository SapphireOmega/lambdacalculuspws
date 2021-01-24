/*
 * These functions operate on structs with dynamic arrays. These structs must
 * have the current number of items as the first variable, the amount of items
 * that currently fit into the array as the second, and a pointer to the actual
 * array as the third.
 */

#ifndef ARRAY_H
#define ARRAY_H

#include "util.h"

void init_array(void *array, uint init_cap, size_t index_size, const char *what);
void increase_array(void *array, size_t index_size, const char *what);
void push_array(void *array, size_t index_size, const byte *val, const char *what);

#endif
