#include "minishell.h"

int change_dir(char **args, int is_subshell, t_env *env)
{
	char *wd;

	wd = getcwd(NULL, 0);
	if (chdir(args[0]) == -1)
		return (printf("Failure to change directory!"), free(wd), 1); // can't handle everything, need errno which is forbidden
	if (is_subshell)
		chdir(wd);
	free(wd);
	return (0);
}

int echo(char **args, int is_subshell, t_env *env)
{
	int x;

	x = 0;
	if (!ft_strcmp("-n", args[0]))
		x++;
	while (args[x])
		if (printf("%s", args[x++]) < 0)
			return (1);// set retval!!
	if (ft_strcmp("-n", args[0]))
		if (printf("\n") < 0)
			return (1);// set retval!!
	return (0);
}

int print_wd(char **args, int is_subshell, t_env *env)
{
	char *wd;

	wd = getcwd(NULL, 0);
	if (!wd)
		return (printf("Failure to get directory!"), -1);
	if (printf("%s", wd) < 0)
		return (free(wd), 1); // set retval
	free(wd);
}

int valid_key(char *key)
{
	int x;

	x = 0;
	if (key[0] >= '0' && key[0] <= '9')
		return (0);
	while (key[x] && (key[x] >= 'a' && key[x] <= 'z') || (key[x] >= 'A' && key[x] <= 'Z')
		|| (key[x] >= '0' && key[x] <= '9') || key[x] == '_')
		x++;
	if (x == ft_strlen(key) && x != 0)
		return (1);
	return (0);
}

int chck_node(t_env *env, char *arg)
{
	char *key;
	char *value;

	split_env_var(arg, &key, &value); // check for malloc fail
	if (!value) // this does not check for malloc, instead it checks if there is no equal sign!
		return (free(key), -1);
	while (env && env->key != key)
		env = env->next;
	if (!valid_key(key))
		return (-1);
	if (env)
	{
		if (env->value)
			free(env->value);
		env->value = value;
	}
	else
		return (free(value), free(key), 0);
	return (free(key), 1);
}

int export(char **args, int is_subshell, t_env *env)
{
	int x;
	int c;
	int opti;

	c = 0;
	x = 0;
	while (args && args[x])
		x++;
	if (x == 0)
	{
		while (env)
		{
			if (printf("declare -x %s=%s\n", env->key, env->value) < 0)
				return (1); // set retval
			env = env->next;
		}
	}
	else
		while (c < x)
		{
			opti = chck_node(env, args[c]);
			if (opti == 0 && !is_subshell)
				append_env_node(env, args[c++]);
			else if (opti == -1)
				return (printf("Invalid identified!\n"), 1);
		}
	return (0);
}

int unset(char **args, int is_subshell, t_env *env)
{
	int		x;
	t_env	*prev;
	t_env	*sv;

	x = 0;
	sv = env;
	while (args[x])
	{
		if(!valid_key(args[x]))
			return (printf("Invalid identified!\n"), 1);
		while (env && env->key != args[x])
		{
			prev = env;
			env = env->next;
		}
		if (env && !is_subshell)
		{
			free(env->key);
			free(env->value);
			prev->next = env->next;
			free(env);
		}
		env = sv;
		x++;
	}
}