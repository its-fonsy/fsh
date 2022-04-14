#ifndef __FSH_PROC_
#define __FSH_PROC_

typedef struct Proc {
	int pid;
	int id;
	char **args;
	struct Proc *next;
	struct Proc *prev;
} Proc;

Proc * init_list();

#endif
