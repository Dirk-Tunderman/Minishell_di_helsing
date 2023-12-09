/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 21:54:31 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/09 15:08:47 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void update_wd(char *wd, t_env *env)
{
	char *sv;

	if(!wd)
		return ;
	alloc_wrap(wd);
	sv = ft_strjoin("PWD=", wd); // leak
	while (env && _ft_strcmp(env->key, "PWD")) // env->key should never equal null otherwise it's a crash
			env = env->next;
	if (!env)
		append_env_node(&env, sv);
	else
		env->value = wd;
}

void update_oldwd(char *old, t_env *env)
{
	char *sv;

	if(!old)
		return ;
	sv = ft_strjoin("OLDPWD=", old); // leak
	while (env && _ft_strcmp(env->key, "OLDPWD")) // env->key should never equal null otherwise it's a crash
			env = env->next;
	if (!env)
		append_env_node(&env, sv);
	else
		env->value = old;
}

int change_dir(char **args, t_env *env, int *exit_stat)
{
	char *old;
	
	old = getcwd(NULL, 0);
	if (old)
		alloc_wrap(old);
	if (!args[0]) // always the case that this might be null, since no data is copied even though the allocation is done (in the parsing)
	{
		while (env && _ft_strcmp(env->key, "HOME")) // env->key should never equal null otherwise it's a crash
			env = env->next;		
		if (!env || !env->value)
			return (printf("Failure to change directory!\n"), 1);
		if (chdir(env->value) == -1)
		{
			*exit_stat = 1;
			return (printf("Failure to change directory!\n"), 1);
		}
		update_wd(getcwd(NULL, 0), env);
		update_oldwd(old, env);
		return (0);
	}
	if (chdir(args[0]) == -1)
	{
		*exit_stat = 1;
		return (printf("Failure to change directory!\n"), 1); // can't handle everything, need errno which is forbidden
	}
	update_wd(getcwd(NULL, 0), env);
	update_oldwd(old, env);
	*exit_stat = 0;
	return (0);
}

int echo(char **args, t_env *env, int *exit_stat)
{
	int x;
	char **sv;

	(void)(env);
	sv = args;
	x = 0;
	if (!args[0])
	{
		*exit_stat = 0;
		return (printf("\n"));
	}
	if (!_ft_strcmp("-n", args[0]))
		args = &args[1];
	while (args[x])
	{
		if (x > 0)
		{
			if (printf(" ") < 0)
			{
				*exit_stat = 1;
				return (1);// set retval!!
			}
		}
		if (printf("%s", args[x++]) < 0)
		{
			*exit_stat = 1;
			return (1);// set retval!!
		}
	}
	if (_ft_strcmp("-n", sv[0]))
		if (printf("\n") < 0)
		{
			*exit_stat = 1;
			return (1);// set retval!!
		}
	*exit_stat = 0;
	return (0);
}

int print_wd(char **args, t_env *env, int *exit_stat)
{
	char *wd;

	(void)(args);
	(void)(env);
	wd = getcwd(NULL, 0);
	if (!wd)
	{
		*exit_stat = 0;
		return (printf(".\n"), 0);
	}
	if (printf("%s\n", wd) < 0)
	{
		*exit_stat = 1;
		return (free(wd), 1); // set retval
	}
	free(wd);
	*exit_stat = 0;
	return (0);
}

int valid_key(char *key)
{
	int x;

	x = 0;
	if (key[0] >= '0' && key[0] <= '9')
		return (0);
	while (key[x] && ((key[x] >= 'a' && key[x] <= 'z') || (key[x] >= 'A' && key[x] <= 'Z')
		|| (key[x] >= '0' && key[x] <= '9') || key[x] == '_'))
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
	//(free(key), -1); // NOT VALUE THEN add key, but do not append it to env! (maybe using a flag or smth)
	if (!valid_key(key))
		return (-1);
	if (!value)
		return (2);
	while (env && _ft_strcmp(env->key, key))
		env = env->next;
	if (env)
	{
	//	free(env->value); This will be garbage collected later!!!
		env->value = value;
	}
	else
		return (0);//(free(value), free(key), 0);
	return (1);//(free(key), 1);
}

int is_there(t_env *env, char *key)
{
	while (env && _ft_strcmp(env->key, key))
		env = env->next;
	if (env)
		return (1);
	return (0);
}

void set_exp(t_env *env, char *key)
{
	while (env && _ft_strcmp(env->key, key))
		env = env->next;
	if (env)
		env->only_export = 1;
}

int export(char **args, t_env *env, int *exit_stat)
{
	int x;
	int c;
	int opti;

	*exit_stat = 1;
	c = 0;
	x = 0;
	while (args && args[x])
		x++;
	if (x == 0)
	{
		while (env)
		{
			if (printf("declare -x %s=%s\n", env->key, env->value) < 0)
				return (1);
			env = env->next;
		}
	}
	else
		while (c < x)
		{
			opti = chck_node(env, args[c]);
			if (opti == 0)
				append_env_node(&env, args[c++]);
			else if (opti == 1)
				c++;
			else if (opti == -1)
				return (printf("Invalid identifier!\n"), 1);
			else if (opti == 2)
			{
				if(!is_there(env, args[c]))
					append_env_node(&env, args[c]);
				set_exp(env, args[c++]);
			}
		}
	*exit_stat = 0;
	return (0);
}

int unset(char **args, t_env *env, int *exit_stat)
{
	int		x;
	t_env	*prev;
	t_env	*sv;

	x = 0;
	sv = env;
	while (args[x])
	{
		if(!valid_key(args[x]))
		{
			*exit_stat = 1;
			return (printf("Invalid identifier!\n"), 1);
		}
		while (env && _ft_strcmp(env->key, args[x]))
		{
			prev = env;
			env = env->next;
		}
		if (env)
			prev->next = env->next; // do not free (double free)!
		env = sv;
		x++;
	}
	*exit_stat = 0;
	return (0);
}

void print_env(char **args, t_env *head, int *exit_stat) 
{
	(void) args;
    while (head) 
	{
		if (!head->only_export)
		{
			if (head->value)
        		printf("%s=%s\n", head->key, head->value);
			else
				printf("%s=\n", head->key);
		}
		head = head->next;
    }
	*exit_stat = 0;
}