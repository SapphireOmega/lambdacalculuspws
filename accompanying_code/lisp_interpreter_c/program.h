#ifndef PROGRAM_H
#define PROGRAM_H

#include "util.h"
#include "array.h"
#include "list.h"

struct program {
	uint n;
	uint cap;
	struct list *lists;
};

inline void init_program(struct program *program, uint init_cap)
{
	init_array(program, init_cap, sizeof(struct list), "program");
}

inline void increase_program(struct program *program)
{
	increase_array(program, sizeof(struct list), "program");
}

inline void push_program(struct program *program, struct list list)
{
	push_array(program, sizeof(struct list), (void *)&list, "program");
}

#endif
