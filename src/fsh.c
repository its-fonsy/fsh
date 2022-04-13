
#include "fsh_func.h"

int main(void)
{
	char *user_line;
	char **args;

	while(1) {
		printf("fsh> ");
		user_line = fsh_read_input();
		args = fsh_split_input(user_line);
		fsh_exec(args);

		free (user_line);
		free (args);
	}

	return 0;
}
