#ifndef LIST_H
#define LIST_H

#include "util.h"
#include "environment.h"
#include "token.h"

struct list {
	uint n;
	uint cap;
	struct list_item *items;
};

union list_item_data {
	struct id id;
	struct list list;
};

enum list_item_type {
	ID,
	LIST,
};

struct list_item {
	union list_item_data data;
	enum list_item_type type;
};

/* Function declarations */
struct list list_from_token_stack(struct token_stack stack, uint offset, uint *new_index);
void print_list(struct list list, uint indent);
void free_list(struct list list);
void eval_list(struct list list, struct env env);

inline void init_list(struct list *list, uint init_cap)
{
	init_array(list, init_cap, sizeof(struct list_item), "list");
}

inline void increase_list(struct list *list)
{
	increase_array(list, sizeof(struct list_item), "list");
}

inline void push_list(struct list *list, struct list_item item)
{
	push_array(list, sizeof(struct list_item), (void *)&item, "list");
}

#endif
