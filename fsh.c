#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define	FSH_LB_SIZE	128
#define	FSH_AB_SIZE	128

int fsh_exec(char **args) {
	int error;
	error = execvp(args[0], args);

	if(error)
      fprintf(stderr, "fsh: command %s not found\n", args[0]);
}

char **fsh_split_input(char *line) {
	int buff_size = FSH_AB_SIZE;
	int i = 0, j= 0;
	char c;
	bool quoted = false;
	char **args = (char **) malloc( buff_size * sizeof(char*) );

	args[j++] = &line[i];
	while(1) {
		c = line[i];

		if (c == '\0')
			return args;

		/* Increase memory allocation for the arguments */
		if( j >  buff_size) {
			buff_size += FSH_AB_SIZE;
			args = realloc(args, buff_size * sizeof(char*));
		}

		/* Check for quoted arguments */
		if( c == '"') {
			line[i] = '\0';
			quoted = !quoted;
			if(quoted)
				args[j-1] = &line[i+1];
		}

		/* Separate argument with spaces */
		if( c == ' ' && !quoted) {
			line[i] = '\0';
			args[j] = &line[i+1];
			j++;
		}

		i++;
	}
}

char *fsh_read_input() {
	char *user_line;
	char c;
	int i = 0;
	int buff_size = FSH_LB_SIZE;

	user_line = (char*) malloc(buff_size * sizeof(char));

	if(!user_line)
		fprintf(stderr, "fsh: allocation error");
	
	while(1) {
		c = getchar();
		
		if(c == '\n' || c == EOF) {
			user_line[i] = '\0';
			return user_line;
		}

		if (i > buff_size) {
			buff_size += FSH_LB_SIZE;
			user_line = realloc(user_line, buff_size * sizeof(char));
		}

		user_line[i] = c;
		i++;
	}
}

int main(void)
{
	char *user_line;
	char **args;

	printf("fsh> ");
	user_line = fsh_read_input();
	args = fsh_split_input(user_line);
	fsh_exec(args);

	return 0;
}
