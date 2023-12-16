/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtunderm <dtunderm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 21:02:14 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/16 11:39:29 by dtunderm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strdup(const char *s1)
{
	int		a;
	char	*str;

	a = 0;
	str = ft_calloc (ft_strlen(s1) + 1, 1);
	if (str == 0)
		return (0);
	while (a < ft_strlen(s1))
	{
		str[a] = s1[a];
		a++;
	}
	str[a] = 0;
	return (str);
}

int	exit_status(int value)
{
	static int	status;

	if (value > 255)
		return (status);
	else
		status = value;
	return (0);
}

int	count_cmd(t_node *head)
{
	int	count;

	count = 1;
	while (head)
	{
		if (head->type == OPERATOR)
			count++;
		head = head->next;
	}
	return (count);
}

t_env	*duplicate_env(t_env *env)
{
	t_env	*res;
	t_env	*head;

	res = 0;
	if (env)
		res = ft_calloc(1, sizeof(t_env));
	head = res;
	while (env)
	{
		res->key = m_ft_strdup(env->key);
		res->value = m_ft_strdup(env->value);
		res->only_export = env->only_export;
		env = env->next;
		if (env)
		{
			res->next = ft_calloc(1, sizeof(t_env));
			res = res->next;
		}
		else
			res->next = 0;
	}
	return (head);
}

void	alloc_wrap_env(t_env *env)
{
	while (env)
	{
		alloc_wrap(env);
		alloc_wrap(env->key);
		alloc_wrap(env->value);
		env = env->next;
	}
}

int	start_cmd(t_node *head, t_env *l_env, char **envp)
{
	t_comparsed	*parsed;

	parsed = parsed_single_cmd(head, count_cmd(head), exit_status(YIELD), envp);
	resolve_path(head, l_env);
	parsed->environment = l_env;
	if (execute_list(parsed, &l_env, envp, head) == SYSCALLFAIL)
		parsed->exit_status = 1;
	exit_status(parsed->exit_status);
	restore_fds(0);
	return 0;
}

void	main_loop(t_node *head, char **envp)
{
	char			*input;
	static t_env	*l_env;

	l_env = envp_to_linked_list(envp);
	while (1)
	{
		sig_init();
		input = readline("minishell$: ");
		signal(SIGINT, SIG_IGN);
		if (!input)
			fail_exit();
		alloc_wrap(input);
		if (input && ft_strlen(input) && (add_history(input), 1)
			&& checker_quotes(input))
		{
			lexer(input, &head, l_env);
			set_redirect_in_nodes(head);
			display_list(head);

			if (!error_all_check(head))
				start_cmd(head, l_env, envp);
			l_env = duplicate_env(l_env);
			//fail(0, 0);
			//display_list(head);
			alloc_wrap_env(l_env);
			head = NULL;
		}
	}
}

int	main(int argc, char **argv, char **envp)
{
	int		x;
	char	*key;
	char	*value;

	(void)argc;
	(void)argv;
	x = 0;
	while (envp && envp[x])
	{
		split_env_var(envp[x], &key, &value);
		if (!_ft_strcmp(key, "SHLVL"))
			envp[x] = ft_strjoin(ft_strjoin(key, "="),
					ft_itoa(ft_atoi(value) + 1));
		x++;
	}
	rl_catch_signals = 0;
	restore_fds(0);
	return (main_loop(0, envp), 0);
}
