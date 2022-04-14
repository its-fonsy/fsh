#include "common.h"
#include "proc.h"

Proc * init_list() {
	Proc *head;
	head = (Proc*) malloc(sizeof(Proc));

	head->pid = 0;
	head->id = 0;
	head->args = NULL;
	head->next = NULL;
	head->next = NULL;

	return head;
}
