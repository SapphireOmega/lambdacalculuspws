#include "token.h"

const char *tokens[] = {"(", ")"};

extern inline void init_token_stack(struct token_stack *stack, uint init_cap);
extern inline void increase_token_stack(struct token_stack *stack);
extern inline void push_token_stack(struct token_stack *stack, struct token token);
