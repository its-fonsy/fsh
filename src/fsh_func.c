#include <fcntl.h>
#include <string.h>

#include "fsh_func.h"
#include "proc.h"
#include "fsh.h"

/* List of builtin commands, followed by their corresponding functions. */
char *builtin_str[] = {
  "cd",
  "jobs",
  "exit"
};

int (*builtin_func[]) (char **) = {
  &fsh_cd,
  &fsh_jobs,
  &fsh_exit
};

int fsh_exec(char **args) {
	int error;
	int argc;
	int i;
	bool bg = false;

	/* get the number of arguments */
	for(argc = 0; args[argc] != NULL; argc++)
		;

	/* if the last char is a '&' then execute command in background */
	if (args[argc-1][0] == '&') {
		bg = true;
		args[argc-1] = NULL;
	}

	/* An empty command was entered. */
	if (args[0][0] == '\0') {
		return 0;
	}

	/* Executing a builtin function */
	for (i = 0; i < fsh_num_builtins(); i++) {
		if (strcmp(args[0], builtin_str[i]) == 0) {
			return (*builtin_func[i])(args);
		}
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

int fsh_num_builtins() {
	return sizeof(builtin_str) / sizeof(char *);
}

int fsh_cd(char **args) {
	if (args[1] == NULL) {
		fprintf(stderr, "fsh: expected argument to \"cd\"\n");
	} else {
		if (chdir(args[1]) != 0) {
			perror("fsh");
		}
	}
	return 1;
}

/*
 * Builtin function that lists the current process in the background
 */
int fsh_jobs(char **args) {

	for(Proc *p = bg_proc_head->next; p != NULL; p=p->next)
		printf("[%d] %d %s\n", p->id, p->pid, p->args[0]);

	return 1;
}

int fsh_exit() {
	exit(0);
}
