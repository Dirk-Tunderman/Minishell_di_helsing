#include "minishell.h"

void call_respective(char **cmd, int *exit_stat, t_env *env)
{
	if (!_ft_strcmp(cmd[0], "echo"))
		echo(&cmd[1], env, exit_stat);
	if (!_ft_strcmp(cmd[0], "cd"))
		change_dir(&cmd[1], env, exit_stat);
	if (!_ft_strcmp(cmd[0], "pwd"))
		print_wd(&cmd[1], env, exit_stat);
	if (!_ft_strcmp(cmd[0], "export"))
		export(&cmd[1], env, exit_stat);
	if (!_ft_strcmp(cmd[0], "unset"))
		unset(&cmd[1], env, exit_stat);
	if (!_ft_strcmp(cmd[0], "env"))
		print_env(&cmd[1], env, exit_stat);
		//env_builtin(&cmd[1], env, exit_stat); // for dirk
	if (!_ft_strcmp(cmd[0], "exit"))
		;
		//exit_builtin(&cmd[1], env, exit_stat); // for dirk
}