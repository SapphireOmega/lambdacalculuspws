#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "util.h"
#include "array.h"

struct env_func {
	int (*func)(int, int);
};

enum env_index_type {
	ENV_FUNC,
};

union env_index_data {
	struct env_func func;
};

struct env_index {
	char *id;
	enum env_index_type type;
	union env_index_data data;
};

struct env {
	uint n;
	uint cap;
	struct env_index *indeces;
};

inline void init_env(struct env *env, uint init_cap)
{
	init_array(env, init_cap, sizeof(struct env_index), "environment");
}

inline void increase_env(struct env *env)
{
	increase_array(env, sizeof(struct env_index), "environment");
}

inline void push_env(struct env *env, struct env_index index)
{
	push_array(env, sizeof(struct env_index), (void *)&index, "environment");
}

#endif
