#include <signal.h>
#include "fsh_func.h"
#include "fsh.h"

#define	FSH_BG_SIZE	16

int bg_procs[FSH_BG_SIZE];
int bg_proc_idx = 0;

static void handler(int signo, siginfo_t *si, void *data) {
	(void) signo;
	(void) data;
	for(int k = 0; k < FSH_BG_SIZE; k++)
		if(si->si_pid == bg_procs[k]) {
			printf("Process %d has finished", si->si_pid);
			bg_proc_idx--;
			break;
		}
}

int main(void)
{
	char *user_line;
	char **args;
	int pid;

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
			bg_procs[bg_proc_idx++] = pid;

		free (user_line);
		free (args);
	}

	return 0;
}
