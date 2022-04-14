#ifndef __FSH_FSH_
#define __FSH_FSH_

typedef struct Proc {
	int pid;
	char **args;
	struct Proc *next;
	struct Proc *prev;
} Proc;


#endif
