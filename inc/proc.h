#ifndef __FSH_PROC_
#define __FSH_PROC_

typedef struct Proc {
	int pid;
	int id;
	char **args;
	struct Proc *next;
	struct Proc *prev;
} Proc;

Proc * create_proc(int p_pid, int p_id, char ** p_args);
void append_proc(Proc *head, Proc *proc);
void pop_proc(Proc *proc);
void update_proc_ids(Proc *head);
Proc * proc_in_list(Proc *head, int p_pid);

#endif
