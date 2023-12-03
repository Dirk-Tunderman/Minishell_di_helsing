#include "minishell.h"

int call_respective(char *cmd, int *exit_stat, int is_subshell, t_env *env)
{
	if (!ft_strcmp(cmd[0], "echo"))
		*exit_stat = echo(&cmd[1], is_subshell, env);
	if (!ft_strcmp(cmd[0], "cd"))
		*exit_stat = change_dir(&cmd[1], is_subshell, env);
	if (!ft_strcmp(cmd[0], "pwd"))
		*exit_stat = print_wd(&cmd[1], is_subshell, env);
	if (!ft_strcmp(cmd[0], "export"))
		*exit_stat = export(&cmd[1], is_subshell, env);
	if (!ft_strcmp(cmd[0], "unset"))
		*exit_stat = unset(&cmd[1], is_subshell, env);
	if (!ft_strcmp(cmd[0], "env"))
		*exit_stat = env_builtin(&cmd[1], is_subshell, env); // for dirk
	if (!ft_strcmp(cmd[0], "exit"))
		*exit_stat = exit_builtin(&cmd[1], is_subshell, env); // for dirk
}