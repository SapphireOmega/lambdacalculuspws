#include "program.h"

extern inline void init_program(struct program *program, uint init_cap);
extern inline void increase_program(struct program *program);
extern inline void push_program(struct program *program, struct list list);
