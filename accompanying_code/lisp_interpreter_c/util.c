#include "util.h"

#include <stdarg.h>

extern inline void *allocate(size_t size, const char *what);
extern inline void *reallocate(void *ptr, size_t size, const char *what);

void *try(void *ptr, const char *err, ...)
{
	if (ptr == NULL) {
		va_list args;
		va_start(args, err);
		vfprintf(stderr, err, args);
		va_end(args);
		exit(EXIT_FAILURE);
	}
	return ptr;
}
