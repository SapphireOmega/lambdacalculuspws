#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

typedef unsigned int uint;
typedef char byte;

void *try(void *ptr, const char *err, ...);

inline void *allocate(size_t size, const char *what)
{
	return try(malloc(size), "Error during %s heap allocation: %s.\n", what, strerror(errno));
}

inline void *reallocate(void *ptr, size_t size, const char *what)
{
	return try(realloc(ptr, size), "Error during %s heap reallocation: %s.\n", what, strerror(errno));
}

#endif
