#include "common.h"
#include "proc.h"

Proc * create_proc(int p_pid, int p_id, char ** p_args) {
	Proc *proc;
	proc = (Proc*) malloc(sizeof(Proc));

	proc->pid = p_pid;
	proc->id = p_id;
	proc->args = p_args;
	proc->next = NULL;
	proc->next = NULL;

	return proc;
}

void append_proc(Proc *head, Proc *proc) {

	Proc *last = head;

	/* resetting proc links */
	proc->next = NULL;
	proc->prev = NULL;

	/* get the last element of the list */
	while(last->next != NULL)
		last = last->next;

	/* make proc the new last element of the list */
	last->next = proc;
	proc->prev = last;
}

void pop_proc(Proc *proc) {

	Proc *left_link = proc->prev;
	Proc *right_link = proc->next;

	if(right_link != NULL) {
		/*
		 * Current state of the list
		 *       -> left_link -> proc -> right_link
		 * when proc is popped it becomes
		 *       -> left_link -> right_link
		 */
		left_link->next = right_link;
		right_link->prev = left_link;
	} else {
		/*
		 * Current state of the list
		 *       -> left_link -> proc
		 * when proc is popped it becomes
		 *       -> left_link
		 */
		left_link->next = NULL;
	}

	free(proc->args);
	free(proc);
}

void update_proc_ids(Proc *head) {
	int id = 0;

	for(Proc *p = head->next; p != NULL; p=p->next)
		p->id = id++;
}

Proc * proc_in_list(Proc *head, int p_pid) {

	for(Proc *p = head->next; p != NULL; p=p->next)
		if(p->pid == p_pid)
			return p;

	return NULL;
}
