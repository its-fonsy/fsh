#include <signal.h>
#include "fsh_func.h"
#include "fsh.h"
#include "proc.h"


static void handler(int signo, siginfo_t *si, void *data) {
	(void) signo;
	(void) data;
	printf("Process %d has finished", si->si_pid);
}

int main(void)
{
	char *user_line;
	char **args;
	int pid;
	Proc *bg_proc_head = init_list();

	struct sigaction sa;
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = handler;
	sigaction(SIGCHLD, &sa, 0);

	while(1) {
		printf("fsh> ");
		user_line = fsh_read_input();
		args = fsh_split_input(user_line);
		pid = fsh_exec(args);

		if (pid)
			continue;

		free (user_line);
		free (args);
	}

	return 0;
}
