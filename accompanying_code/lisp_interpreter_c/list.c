#include "list.h"

extern inline void init_list(struct list *list, uint init_cap);
extern inline void increase_list(struct list *list);
extern inline void push_list(struct list *list, struct list_item item);

struct list list_from_token_stack(struct token_stack stack, uint offset, uint *new_index)
{
	struct list list;

	init_list(&list, 16);

	for (uint i = offset; i < stack.n; i++) {
		struct list_item new_item;
		switch (stack.ts[i].type) {
		case OPENING_PAREN_TOKEN:
			new_item.type = LIST;
			new_item.data.list = list_from_token_stack(stack, i + 1, &i);
			break;
		case CLOSING_PAREN_TOKEN:
			*new_index = i;
			return list;
		case ID_TOKEN:
			new_item.type = ID;
			new_item.data.id.str = (char *)allocate(strlen(stack.ts[i].str) + 1, "identifier string");
			strncpy(new_item.data.id.str, stack.ts[i].str, strlen(stack.ts[i].str) + 1);
			break;
		default:
			fprintf(stderr, "Error: unknown token type: %d.\n", stack.ts[i].type);
		}
		push_list(&list, new_item);
	}

	fprintf(stderr, "Error parsing program: unclosed list\n");
	exit(EXIT_FAILURE);
}

void print_list(struct list list, uint indent)
{
	for (uint i = 0; i < list.n; i++) {
		for (uint j = 0; j < indent; j++)
			putchar('\t');
		switch (list.items[i].type) {
		case LIST:
			puts("list: ");
			print_list(list.items[i].data.list, indent + 1);
			break;
		case ID:
			printf("id: \"%s\"\n", list.items[i].data.id.str);
			break;
		default:
			fprintf(stderr, "Error printing list: unknown list item type: %d.\n", list.items[i].type);
		}
	}
}

void free_list(struct list list)
{
	for (uint i = 0; i < list.n; i++) {
		switch (list.items[i].type) {
		case LIST:
			free_list(list.items[i].data.list);
			break;
		case ID:
			free(list.items[i].data.id.str);
			break;
		default:
			fprintf(stderr, "Error freeing list: unknown list item type: %d.\n", list.items[i].type);
		}
	}

	free(list.items);
}

void eval_list(struct list list, struct env env)
{
	bool in_env;

	switch (list.items[0].type) {
	case ID:
		in_env = false;
		for (uint i = 0; i < env.n; i++) {
			if (list.items[0].data.id.str == env.indeces[i].id) {
				in_env = true;
				switch (env.indeces[0].type) {
				case ENV_FUNC:
					printf("%s: function\n", list.items[i].data.id.str);
					break;
				default:
					fprintf(stderr, "Error: unknown environment index type: %d.\n", env.indeces[0].type);
				}
			}
		}
		if (!in_env)
			fprintf(stderr, "Error: unknown identifier: %s.\n", list.items[0].data.id.str);
		break;
	case LIST:
		eval_list(list.items[0].data.list, env);
		//list.items[0] = eval_list(list.items[0]);
		//eval_list(list.items);
		break;
	default:
		fprintf(stderr, "Error: unknown list item type: %d.\n", list.items[0].type);
	}
}
