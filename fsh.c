#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define	FSH_LB_SIZE	128
#define	FSH_AB_SIZE	128

char **fsh_split_input(char *line) {
	int i, j = 0;
	int buff_size = FSH_AB_SIZE;
	bool quoted = false;
	char **args = (char **) malloc( buff_size * sizeof(char*) );

	args[j] = &line[0];
	j++;
	for(i=0; line[i] != '\0'; i++) {

		/* Reallocate memory if necessary */
		if( i > buff_size ) {
			buff_size += FSH_AB_SIZE;
			args = realloc(args, buff_size * sizeof(char));
		}

		/* Tokenize argument separated by space */
		if(line[i] == ' ' && !quoted) {
			line[i] = '\0';
			args[j] = &line[i+1];
			j++;
		}

		/* Tokenize arguments between quotes */
		if(line[i] == '"') {
			if(quoted) {
				/* second quotation mark */
				line[i] = '\0';
				args[j] = &line[i+2];
				j++;
				i++;
				quoted = false;
			} else {
				/* first quotation mark */
				args[j-1] = &line[i+1];
				quoted = true;
			}
		}
	}

	args[j] = NULL;

	return args;
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

	return 0;
}
