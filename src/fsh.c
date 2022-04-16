#include <signal.h>

#include "fsh_func.h"
#include "fsh.h"
#include "proc.h"

/* head of the linked list that tracks
 * background processes */
Proc *bg_proc_head;

static void handler(int signo, siginfo_t *si, void *data) {
	(void) signo;
	(void) data;

	Proc *p = proc_in_list(bg_proc_head, si->si_pid);
	if( p != NULL) {
		printf("\n[%d] Process %s done\n", p->pid, p->args[0]);
		pop_proc(p);
		update_proc_ids(bg_proc_head);
	}
}

int main(void)
{
	char *user_line;
	char **args;
	int pid;
	bg_proc_head = create_proc(0, 0, NULL);

	struct sigaction sa;
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = handler;
	sigaction(SIGCHLD, &sa, 0);

	while(1) {
		printf("fsh> ");
		user_line = fsh_read_input();
		args = fsh_split_input(user_line);
		pid = fsh_exec(args);

		/* pid > 0 means process is executed in background */
		if(pid) {
			append_proc( bg_proc_head, create_proc(pid, 0, args) );
			update_proc_ids(bg_proc_head);
		} else {
			free (user_line);
			free (args);
		}

	}

	return 0;
}
