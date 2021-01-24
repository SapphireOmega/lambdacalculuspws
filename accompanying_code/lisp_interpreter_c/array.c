#include "array.h"

void init_array(void *array, uint init_cap, size_t index_size, const char *what)
{
	uint *info = (uint *)array;
	void **data = array + 2 * sizeof(uint);

	info[0] = 0; // n = 0
	info[1] = init_cap; // cap = init_cap
	*data = allocate(init_cap * index_size, what);
}

void increase_array(void *array, size_t index_size, const char *what)
{
	uint *info = (uint *)array;
	void **data = array + 2 * sizeof(uint);

	info[1] *= 2; // cap *= 2
	*data = reallocate(*data, info[1] * index_size, what);
}

void push_array(void *array, size_t index_size, const byte *val, const char *what)
{
	uint *info = (uint *)array;
	void **data = array + 2 * sizeof(uint);
	byte *val_index = *data + info[0] * index_size; // data[n]

	memcpy(val_index, val, index_size); // data[n] = val
	info[0]++; // n++
	if (info[0] == info[1]) // n == cap
		increase_array(array, index_size, what);
}
