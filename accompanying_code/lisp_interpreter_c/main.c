#include "util.h"
#include "token.h"
#include "list.h"
#include "program.h"
#include "environment.h"

int add(int x, int y)
{
	return x + y;
}

int main(int argc, char *argv[])
{
	struct env env;
	struct env_index env_add = { .id = "+", .type = ENV_FUNC, .data = &add };

	init_env(&env, 64);
	push_env(&env, env_add);

	puts("Lispy Version 0.0.1\n");

	/* Main loop */
	for (;;) {
		struct token_stack token_stack;
		char input[2048];

		init_token_stack(&token_stack, 1024); // initialise new token stack

		/* Get input from user */
		fputs("Lispy> ", stdout);
		try(fgets(input, 2048, stdin), "Error reading input from stdin: %s.\n", strerror(errno));
		input[strcspn(input, "\n")] = '\0';
		input[strcspn(input, "\t")] = ' ';

		/* Split input up into tokens */
		for (uint i = 0; i < strlen(input);) {
			uint token_len = 1;
			struct token new_token;

			switch (input[i]) {
			case '(':
				new_token.type = OPENING_PAREN_TOKEN;
				new_token.str = (char *)allocate(2, "token string"); // two including null byte
				strcpy(new_token.str, "(");
				push_token_stack(&token_stack, new_token);
				break;
			case ')':
				new_token.type = CLOSING_PAREN_TOKEN;
				new_token.str = (char *)allocate(2, "token string"); // two including null byte
				strcpy(new_token.str, ")");
				push_token_stack(&token_stack, new_token);
				break;
			case ' ':
				break;
			default:
				new_token.type = ID_TOKEN;
				for (bool breakl = false; i + token_len < strlen(input) && !breakl; token_len++) {
					if (input[i + token_len] == ' ')
						break;
					for (uint j = 0; j < sizeof(tokens) / sizeof(tokens[0]) && !breakl; j++)
						if (input[i + token_len] == tokens[j][0])
							breakl = true;
					if (breakl)
						break;
				}
				new_token.str = (char *)allocate(token_len + 1, "token string");
				strncpy(new_token.str, input+i, token_len);
				push_token_stack(&token_stack, new_token);
			}

			i += token_len;
		}

		/* Create lists from the token stack and add them to a program */
		struct program program;
		init_program(&program, 1);
		for (uint i = 0; i < token_stack.n; i++) {
			struct list new_list;
			switch (token_stack.ts[i].type) {
			case OPENING_PAREN_TOKEN:
				new_list = list_from_token_stack(token_stack, i + 1, &i);
				push_program(&program, new_list);
				break;
			case CLOSING_PAREN_TOKEN:
				fputs("Error parsing program: unexpected end of list.\n", stderr);
			default:
				fputs("Error parsing program: not a list.\n", stderr);
			}
		}

		/* Print out program */
		for (uint i = 0; i < program.n; i++) {
			print_list(program.lists[i], 0);
		}

		/* Run program */
		for (uint i = 0; i < program.n; i++) {
			eval_list(program.lists[i], env);
		}

		/* Free memory */
		free(token_stack.ts);
		for (uint i = 0; i < program.n; i++) {
			free_list(program.lists[i]);
		}
		free(program.lists);
	}

	return EXIT_SUCCESS;
}
