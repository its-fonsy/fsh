#ifndef __FSH_FUNC_
#define __FSH_FUNC_

#include "common.h"

#define	FSH_LB_SIZE	128
#define	FSH_AB_SIZE	128

int fsh_exec(char **args);
char **fsh_split_input(char *line);
char *fsh_read_input();

int fsh_cd(char **args);
int fsh_exit();
int fsh_num_builtins();

#endif
