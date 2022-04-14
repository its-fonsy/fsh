#include "fsh_func.h"
#include <fcntl.h>

int fsh_exec(char **args) {
	int error;
	int argc;
	bool bg = false;

	/* get the number of arguments */
	for(argc = 0; args[argc] != NULL; argc++)
		;

	/* if the last char is a '&' then execute command in background */
	if (args[argc-1][0] == '&') {
		bg = true;
		args[argc-1] = NULL;
	}

	pid_t pid = fork();

	if( pid == 0 ) {

		if(bg) {
			int fdout = open("/dev/null",O_WRONLY | O_CREAT, 0600);
			dup2(fdout, STDOUT_FILENO);
			close(fdout);
		}

		error = execvp(args[0], args);
		if(error)
			fprintf(stderr, "fsh: command %s not found\n", args[0]);

		return 0;

	} else {

		if(bg) {
			waitpid(pid, NULL, WNOHANG);
			return pid;
		}
		else
			waitpid(pid, NULL, 0);
	}
	return 0;
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

		if (c == '\0') {
			args[j] = NULL;
			return args;
		}

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
