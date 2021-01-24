#include "environment.h"

extern inline void init_env(struct env *env, uint init_cap);
extern inline void increase_env(struct env *env);
extern inline void push_env(struct env *env, struct env_index index);
