/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   final_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 20:39:26 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/03 14:26:54 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void append(char **exec_ready, char *data)
{
	int x;
	int c;
	char **newexecready;

	x = 0;
	while (exec_ready && exec_ready[x])
		x++;
	newexecready = alloc_wrapper(ft_calloc(sizeof(char *), x + 1 + 1)); // one for new, one for termination
	//if (!newexecready)
//		fail();
	c = 0;
	while (c < x)
	{
		newexecready[c] = exec_ready[c];
		c++;
	}
	newexecready[c] = data;
	if (exec_ready)
		free(exec_ready);
	return (newexecready);
}

char *get_exec_rdy(t_node **linked_list)
{
	char **single_execrdy;
	
	single_execrdy = 0;
	while ((*linked_list) && (*linked_list)->type != OPERATOR) // filter mingled args
	{
		if ((*linked_list) && ((*linked_list)->type == STDIN_RD
		|| (*linked_list)->type == STDOUT_RD
		|| (*linked_list)->type == HERDOC || (*linked_list)->type == APPEND))
			(*linked_list) == (*linked_list)->next;
		else if ((*linked_list)->type == QUOTE_ARG || (*linked_list)->type == ARG
				|| (*linked_list)->type == CMD)
			append(single_execrdy,(*linked_list)->data);
		(*linked_list) = (*linked_list)->next;
	}
	if ((*linked_list))
		(*linked_list) = (*linked_list)->next;
	return (single_execrdy);
}

char **get_real_redirs(t_node *rn_linked_list)
{
	char **redirs;

	redirs =  alloc_wrapper(ft_calloc (sizeof(char *), 4));
//	if (!redirs)
//		fail();
	while (rn_linked_list && rn_linked_list->type != OPERATOR) // filter mingled args
	{
		if (rn_linked_list && rn_linked_list->type == STDIN_RD)
		{
			redirs[0] = rn_linked_list->next->data; // copy capture
			redirs[1] = 0;
		}
		if (rn_linked_list && rn_linked_list->type == HERDOC)
		{
			redirs[1] = rn_linked_list->next->data; // copy capture
			redirs[0] = 0;
		}
		if (rn_linked_list && rn_linked_list->type == STDOUT_RD)
		{
			redirs[2] = rn_linked_list->next->data; // copy capture
			redirs[3] = 0;
		}
		if (rn_linked_list && rn_linked_list->type == APPEND)
		{
			redirs[3] = rn_linked_list->next->data; // copy capture
			redirs[2] = 0;
		}
	}
	// if something isn't copied, the ptr will remain null
	return (redirs);
}

void get_them_forreal(t_node *rn_linked_list, char **dont_be_this, char ***g_redirs)
{
	int x;
	int app;

	x = 0;
	app = 0;
	while (rn_linked_list && rn_linked_list->type != OPERATOR) // filter mingled args
	{
		if (rn_linked_list && (rn_linked_list->type == STDOUT_RD 
			|| rn_linked_list->type == APPEND))
		{
			if (ft_strcmp(rn_linked_list->next->data,
				dont_be_this[rn_linked_list->type - STDIN_RD]))
			{
				if (rn_linked_list->type == STDOUT_RD)
					g_redirs[0][x++] = rn_linked_list->next->data;
				else
					g_redirs[1][app++] = rn_linked_list->next->data;
			}
		}
		rn_linked_list = rn_linked_list->next;
	}
}

char **get_garbage_redirs(t_node *rn_linked_list, char **dont_be_this)
{
	t_node *sv;
	char ***g_redirs;
	int count;
	int count_app;

	sv = rn_linked_list;
	count = 0;
	count_app = 0;
	while (rn_linked_list && rn_linked_list->type != OPERATOR) // filter mingled args
	{
		if (rn_linked_list && (rn_linked_list->type == STDOUT_RD 
			|| rn_linked_list->type == APPEND))
		{
			if (ft_strcmp(rn_linked_list->next->data,
				dont_be_this[2]))
				count++;
			else if (ft_strcmp(rn_linked_list->next->data,
				dont_be_this[3]))
				count_app++;
		}
		rn_linked_list = rn_linked_list->next;
	}
	g_redirs = alloc_wrapper(ft_calloc(sizeof (char **), 2));
//	if (!g_redirs)
///		fail();
	g_redirs[0] = alloc_wrapper(ft_calloc(sizeof (char *), count + 1));
	g_redirs[1] = alloc_wrapper(ft_calloc(sizeof (char *), count_app + 1));
//	if (!g_redirs[0] || !g_redirs[1])
//		fail();
	get_them_forreal(rn_linked_list, dont_be_this, g_redirs);
	return (g_redirs);
}

// always skip operator, since it's considered in number of arguments
t_comparsed *parsed_single_cmd(t_node *linked_list, int cmd_count, int exit_stat, char **envp)
{
	int x;
	t_comparsed *parsed;
	t_node		*rn_linked_list;
	
	x = 0;
	parsed = alloc_wrapper(ft_calloc(sizeof(t_comparsed), 1));
	parsed->exit_status = exit_stat;
	parsed->minishell_env = 0;
	parsed->cmd_count = cmd_count;
	parsed->exec_ready = alloc_wrapper(ft_calloc (sizeof(char **), cmd_count));
	parsed->real_redirects = alloc_wrapper(ft_calloc (sizeof(char **), cmd_count));
	parsed->garbage_redirects_arr = alloc_wrapper(ft_calloc (sizeof(char **), cmd_count));
	while (x < cmd_count && linked_list) // this has to be really really the number of commands
	{
		rn_linked_list = linked_list;
		parsed->exec_ready[x] = get_exec_rdy(&linked_list);
		parsed->real_redirects[x] = get_real_redirs(rn_linked_list);
		parsed->garbage_redirects_arr[x] = get_garbage_redirs(rn_linked_list, parsed->real_redirects[x]);
		x++;
	}
	return (parsed);
}

int node_count(t_env *env)
{
	int x;

	x = 0;
	while (env)
	{
		env = env->next;
		x++;
	}
	return (env);
}

char **env_toarray(t_env *env, t_comparsed* cmds)
{
	char **envp;
	int		x;
	char *tmp;

	x = 0;
	if (cmds->minishell_env)
		free(cmds->minishell_env);
	cmds->minishell_env = 0;
	envp = alloc_wrapper(ft_calloc (sizeof(char *), node_count(env) + 1 + 1));
//	if (!envp)
//		fail();
	while (env)
	{
		tmp = alloc_wrapper(ft_strjoin(env->key, "="));
	//	if (!tmp)
	//		fail();
		envp[x] = alloc_wrapper(ft_strjoin(tmp, env->value));
//		if (!envp[x])
//			fail(); // has to record tmp;
		free(tmp);
		x++;
	}
	return (envp);
}