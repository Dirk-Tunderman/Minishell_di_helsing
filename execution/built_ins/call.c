#include "minishell.h"

void    c_exit(char **args, int *exit_stat) 
{
    if (args[0] == NULL) 
        exit_status(0);
    else if (is_int0(args[0]) && !args[1])
    {
        if (ft_atoi(args[0]) < 0)
            exit_status(256 - ((-ft_atoi(args[0])) % 256));
        else if (ft_atoi(args[0]) > 255)
            exit_status(ft_atoi(args[0]) % 256);
       	else
			exit_status(ft_atoi(args[0]));
    }
    else if (is_int0(args[0]) && args[1])
	{
        printf("minishell: exit: too many arguments\n");
		*exit_stat = 1;
		return ;
	}
	else
        exit_status(255);
	fail_exit();
}

void call_respective(char **cmd, int *exit_stat, t_env **env)
{
	if (!_ft_strcmp(cmd[0], "echo"))
		echo(&cmd[1], *env, exit_stat);
	if (!_ft_strcmp(cmd[0], "cd"))
		change_dir(&cmd[1], *env, exit_stat);
	if (!_ft_strcmp(cmd[0], "pwd"))
		print_wd(&cmd[1], *env, exit_stat);
	if (!_ft_strcmp(cmd[0], "export"))
		export(&cmd[1], *env, exit_stat);
	if (!_ft_strcmp(cmd[0], "unset"))
		unset(&cmd[1], env, exit_stat);
	if (!_ft_strcmp(cmd[0], "env"))
		print_env(&cmd[1], *env, exit_stat);
	if (!_ft_strcmp(cmd[0], "exit"))
		c_exit(&cmd[1], exit_stat);
		//env_builtin(&cmd[1], env, exit_stat); // for dirk
		//exit_builtin(&cmd[1], env, exit_stat); // for dirk
}