#ifndef TOKEN_H
#define TOKEN_H

#include "util.h"
#include "array.h"

extern const char *tokens[2];

enum token_type {
	OPENING_PAREN_TOKEN,
	CLOSING_PAREN_TOKEN,
	ID_TOKEN,
};

struct token {
	enum token_type type;
	char *str;
};

struct token_stack {
	uint n;
	uint cap;
	struct token *ts;
};

struct id {
	char *str;
};

inline void init_token_stack(struct token_stack *stack, uint init_cap)
{
	init_array(stack, init_cap, sizeof(struct token), "token stack");
}

inline void increase_token_stack(struct token_stack *stack)
{
	increase_array(stack, sizeof(struct token), "token stack");
}

inline void push_token_stack(struct token_stack *stack, struct token token)
{
	push_array(stack, sizeof(struct token), (byte *)&token, "token stack");
	stack->ts[stack->n - 1].str = (char *)allocate(strlen(token.str) + 1, "token stack token string");
	strncpy(stack->ts[stack->n - 1].str, token.str, strlen(token.str) + 1);
}

#endif
