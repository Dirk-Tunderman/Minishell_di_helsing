/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   final_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 20:39:26 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/14 00:19:54 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **append(char **exec_ready, char *data)
{
	int x;
	int c;
	char **newexecready;

	x = 0;
	while (exec_ready && exec_ready[x])
		x++;
	newexecready = ft_calloc(sizeof(char *), x + 1 + 1); // one for new, one for termination
	c = 0;
	while (c < x)
	{
		newexecready[c] = exec_ready[c];
		c++;
	}
	newexecready[c] = data;
	return (newexecready);
}

int	_ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s && s[i])
		i++;
	return (i);
}

char *_join(char *acc, char *a)
{
	int x;
	int c;
	char *res;

	x = 0;
	res = ft_calloc(sizeof(char), _ft_strlen(acc) + _ft_strlen(a) + 1); 
	while (acc && acc[x])
	{
		res[x] = acc[x];
		x++;
	}
	c = 0;
	while (a && a[c])
	{
		res[x] = a[c];
		x++;
		c++;
	}
	res[x] = 0;
	return res;
}

char *skip_expand_split(t_node **linked_list) // do not call this function if you are not sure if this is an env
{
	char *exp_nosplit;

	exp_nosplit = 0;
	if ((*linked_list)->space_after >= 2)
		exp_nosplit = (*linked_list)->exp_nosplit;
	while ((*linked_list)->next && (*linked_list)->next->space_after >= 2) // WARNING, do not put this in all functions unless you assure the next
		(*linked_list) = (*linked_list)->next;
	if ((*linked_list)->next && (*linked_list)->space_after >= 2)// && ((*linked_list)->exp_nosplit == (*linked_list)->next->exp_nosplit))
		(*linked_list) = (*linked_list)->next;
	return (exp_nosplit);
}

char **get_exec_rdy(t_node **linked_list)
{
	char **single_execrdy;
	char *acc;

	single_execrdy = 0;
	acc = 0;
	while ((*linked_list) && (*linked_list)->type != OPERATOR) // filter mingled args
	{/////------
		printf("OUT %s\n", (*linked_list)->data); // skip_expand, mind space_after=2;
		if ((*linked_list) && ((*linked_list)->type == STDIN_RD
		|| (*linked_list)->type == STDOUT_RD
		|| (*linked_list)->type == HERDOC || (*linked_list)->type == APPEND))
		{
			(*linked_list) = (*linked_list)->next;
			skip_expand_split(linked_list); // skip to tail if exp splited
			while ((*linked_list) && (*linked_list)->next && !(*linked_list)->space_after && !((*linked_list)->next->type == STDIN_RD
				|| (*linked_list)->next->type == STDOUT_RD
				|| (*linked_list)->next->type == HERDOC || (*linked_list)->next->type == APPEND || (*linked_list)->next->type == OPERATOR))
			{
				(*linked_list) = (*linked_list)->next;
				skip_expand_split(linked_list);
			}
		}
		else if ((*linked_list))
		{
				printf("IN %s\n", (*linked_list)->data);
				acc = _join(acc, (*linked_list)->data); // this is the command xd doesn't matter if it's exp or not, we need a single node bro// only set it if not existing // other stuff has to be investigated for expansion
				while ((*linked_list) && ((*linked_list)->type == QUOTE_ARG || (*linked_list)->type == ARG
					|| (*linked_list)->type == CMD)
					&& (*linked_list)->next && ((*linked_list)->next->type == QUOTE_ARG || (*linked_list)->next->type == ARG
					|| (*linked_list)->next->type == CMD) && !(*linked_list)->space_after)
				{
						acc = _join(acc, (*linked_list)->next->data); // correct with expansion
						(*linked_list) = (*linked_list)->next;
						printf("%s", acc);
				}// this while is for no spaces
				printf("%s", acc);
				single_execrdy = append(single_execrdy, acc);
				acc = 0;
		} // this only appends each argument that is not seperated by space!
		(*linked_list) = (*linked_list)->next;
	}/////------
	if ((*linked_list))
		(*linked_list) = (*linked_list)->next;
	return (single_execrdy);
}

//char **get_exec_rdy_old(t_node **linked_list)
//{
//	char *acc;
//
//	acc = 0;
//	while ((*linked_list) && (*linked_list)->type != OPERATOR)
//	{
//		if ((*linked_list) && ((*linked_list)->type == STDIN_RD
//		|| (*linked_list)->type == STDOUT_RD
//		|| (*linked_list)->type == HERDOC || (*linked_list)->type == APPEND))
//			(*linked_list) = (*linked_list)->next;
//		else
//		{
//			acc = _join(acc, (*linked_list)->data);
//			while ((*linked_list) && ((*linked_list)->type == QUOTE_ARG || (*linked_list)->type == ARG
//				|| (*linked_list)->type == CMD)
//				&& (*linked_list)->next && ((*linked_list)->next->type == QUOTE_ARG || (*linked_list)->next->type == ARG
//				|| (*linked_list)->next->type == CMD) &&  (!(*linked_list)->space_after)) // there are two cases of spaces, the end of a expansion and the end of a simple argument
//			{
//				if (type_from == HERDOC && (*linked_list)->before_env) // doesn't expand heredoc stuff no matter if it's an ARG or QUOTED ARG and whether they appear mingled with nonseperated different args!
//				{
//					fill[0] = _join(fill[0], (*linked_list)->before_env);
//					if ((*linked_list)->exp_nosplit)
//						skip_expand_split(linked_list);
//				}
//				else
//				{
//					if (type_from != HERDOC && (*linked_list)->before_env)
//					{
//						fill[0] = _join(fill[0], (*linked_list)->exp_nosplit);
//						skip_expand_split(linked_list); // this has to always work!
//					}
//					else
//						fill[0] = _join(fill[0], (*linked_list)->data);
//				}
//				(*linked_list) = (*linked_list)->next;
//			}
//		}
//	}
// }

void jma3_kolchi(t_node **linked_list, char **fill, char **empty, int type_from)
{
	//if ((*linked_list)->space_after >= 2)
	//{
	//	if (type_from == HERDOC)
	//		fill[0] = _join(fill[0], (*linked_list)->before_env); // do not even expand
	//	else
	//		fill[0] = _join(fill[0], (*linked_list)->exp_nosplit); // expand ! but this is not for operator so no split!
	//	skip_expand_split(linked_list);
	//}
	//else
	//{
	int skipped;
	
	skipped = 0;
	fill[0] = 0;
	if (type_from == HERDOC && (*linked_list)->before_env)
		fill[0] = _join(fill[0], (*linked_list)->before_env);
	else
	{
		if (type_from != HERDOC && (*linked_list)->exp_nosplit)
		{
			printf("ENTERED \n");
			fill[0] = _join(fill[0], (*linked_list)->exp_nosplit);
			skip_expand_split(linked_list); // this has to always work!
		}
		else
			fill[0] = _join(fill[0], (*linked_list)->data);
	}
	while ((*linked_list) && ((*linked_list)->type == QUOTE_ARG || (*linked_list)->type == ARG
		|| (*linked_list)->type == CMD)
		&& (*linked_list)->next && ((*linked_list)->next->type == QUOTE_ARG || (*linked_list)->next->type == ARG
		|| (*linked_list)->next->type == CMD) &&  (!(*linked_list)->space_after)) // there are two cases of spaces, the end of a expansion and the end of a simple argument
	{
		if (type_from == HERDOC && (*linked_list)->next->before_env) // doesn't expand heredoc stuff no matter if it's an ARG or QUOTED ARG and whether they appear mingled with nonseperated different args!
		{
			fill[0] = _join(fill[0], (*linked_list)->next->before_env);
			if ((*linked_list)->next->exp_nosplit)
			{
				(*linked_list) = (*linked_list)->next;
				skip_expand_split(linked_list);
			}
		}
		else
		{
			if (type_from != HERDOC && (*linked_list)->next->exp_nosplit)
			{
				fill[0] = _join(fill[0], (*linked_list)->next->exp_nosplit);
				(*linked_list) = (*linked_list)->next;
				skipped = 1;
				skip_expand_split(linked_list);
			}
			else
				fill[0] = _join(fill[0], (*linked_list)->next->data);
		}
		if (skipped)
			skipped = 0;
		else
			(*linked_list) = (*linked_list)->next;
	}
	empty[0] = 0;
}

char **get_real_redirs(t_node *rn_linked_list, char **heredocs)
{
	char **redirs;
	int x;
	int	f;

	f = 0;
	x = 0;
	redirs =  ft_calloc(sizeof(char *), 4);
	while (rn_linked_list && rn_linked_list->type != OPERATOR) // filter mingled args
	{
		if (rn_linked_list && rn_linked_list->type == STDIN_RD)
		{
			rn_linked_list = rn_linked_list->next;
			//if (redirs[1])
			//{
			//	fake[f] = redirs[1];
			//	f++;
			//}
			jma3_kolchi(&rn_linked_list, &redirs[0], &redirs[1], STDIN_RD);
			//redirs[0] = rn_linked_list->next->data; // copy capture
			//redirs[1] = 0;
		}
		else if (rn_linked_list && rn_linked_list->type == HERDOC)
		{
			rn_linked_list =  rn_linked_list->next;
			//if (redirs[1])
			//{
			//	fake[f] = redirs[1];
			//	f++;
			//}
			jma3_kolchi(&rn_linked_list, &redirs[1], &redirs[0], HERDOC);
			heredocs[x] = redirs[1];
			x++;
			//if (rn_linked_list->next->before_env)
			//{
			//	redirs[1] = rn_linked_list->next->before_env; // copy capture, similar to before env // if there is a single damn quote you make everything into before env
			//}
			//else
			//	redirs[1] = rn_linked_list->next->data; // copy capture , You expand here
			//redirs[0] = 0;
		}
		else if (rn_linked_list && rn_linked_list->type == STDOUT_RD)
		{
			rn_linked_list =  rn_linked_list->next;
			jma3_kolchi(&rn_linked_list, &redirs[2], &redirs[3], STDOUT_RD);
			//redirs[2] = rn_linked_list->next->data; // copy capture
			//redirs[3] = 0;
		}
		else if (rn_linked_list && rn_linked_list->type == APPEND)
		{
			rn_linked_list =  rn_linked_list->next;
			jma3_kolchi(&rn_linked_list, &redirs[3], &redirs[2], APPEND);
			//redirs[3] = rn_linked_list->next->data; // copy capture
			//redirs[2] = 0;
		}
		if (rn_linked_list && (rn_linked_list->type == ARG || rn_linked_list->type == QUOTE_ARG || rn_linked_list->type == CMD))
			rn_linked_list = rn_linked_list->next; // this should work
	}
	// if something isn't copied, the ptr will remain null
	return (redirs);
}

unsigned char _ft_strcmp(char *str1, char *str2)
{
	int x;
	unsigned char *str1u;
	unsigned char *str2u;

	x = 0;
	str1u = (unsigned char *) str1;
	str2u = (unsigned char *) str2;
	while (str1u[x] && str2u[x] && str1u[x] == str2u[x])
		x++;
	return (str1u[x] - str2u[x]);
}

char	*assemble_garbage(t_node **linked_list, int type_from)
{
	char *fill;
	int skipped;
	//if ((*linked_list)->space_after >= 2)
	//{
	//	if (type_from == HERDOC)
	//		fill[0] = _join(fill[0], (*linked_list)->before_env); // do not even expand
	//	else
	//		fill[0] = _join(fill[0], (*linked_list)->exp_nosplit); // expand ! but this is not for operator so no split!
	//	skip_expand_split(linked_list);
	//}
	//else
	//{
	skipped = 0;
	fill = 0;
	if (type_from == HERDOC && (*linked_list)->before_env)
		fill = _join(fill, (*linked_list)->before_env);
	else
	{
		if (type_from != HERDOC && (*linked_list)->exp_nosplit)
		{
			printf("ENTERED \n");
			fill = _join(fill, (*linked_list)->exp_nosplit);
			skip_expand_split(linked_list); // this has to always work!
		}
		else
			fill = _join(fill, (*linked_list)->data);
	}
	while ((*linked_list) && ((*linked_list)->type == QUOTE_ARG || (*linked_list)->type == ARG
		|| (*linked_list)->type == CMD)
		&& (*linked_list)->next && ((*linked_list)->next->type == QUOTE_ARG || (*linked_list)->next->type == ARG
		|| (*linked_list)->next->type == CMD) &&  (!(*linked_list)->space_after)) // there are two cases of spaces, the end of a expansion and the end of a simple argument
	{
		if (type_from == HERDOC && (*linked_list)->next->before_env) // doesn't expand heredoc stuff no matter if it's an ARG or QUOTED ARG and whether they appear mingled with nonseperated different args!
		{
			fill = _join(fill, (*linked_list)->next->before_env);
			if ((*linked_list)->next->exp_nosplit)
			{
				(*linked_list) = (*linked_list)->next;
				skip_expand_split(linked_list);
			}
		}
		else
		{
			if (type_from != HERDOC && (*linked_list)->next->exp_nosplit)
			{
				fill = _join(fill, (*linked_list)->next->exp_nosplit);
				(*linked_list) = (*linked_list)->next;
				skipped = 1;
				skip_expand_split(linked_list);
			}
			else
				fill = _join(fill, (*linked_list)->next->data);
		}
		if (skipped)
			skipped = 0;
		else
			(*linked_list) = (*linked_list)->next;
	}
	return (fill);
}

void get_them_forreal(t_node *rn_linked_list, char **dont_be_this, char ***g_redirs)
{
	char *sv;
	int x;
	int app;
	int in;

	sv = 0;
	x = 0;
	app = 0;
	in = 0;
	while (rn_linked_list && rn_linked_list->type != OPERATOR) // filter mingled args
	{
		if (rn_linked_list && rn_linked_list->type == STDOUT_RD)
		{
			rn_linked_list =  rn_linked_list->next;
			sv = assemble_garbage(&rn_linked_list, STDOUT_RD);
			if (!dont_be_this[2] || _ft_strcmp(sv, dont_be_this[2])) // operate on the same file, don't take address
				g_redirs[0][x++] = sv;
		}
		if (rn_linked_list && rn_linked_list->type == APPEND)
		{
			rn_linked_list =  rn_linked_list->next;
			sv = assemble_garbage(&rn_linked_list, APPEND);
			if (!dont_be_this[3] || _ft_strcmp(sv, dont_be_this[3]))
				g_redirs[1][app++] = sv;
		}
		if (rn_linked_list && rn_linked_list->type == STDIN_RD)
		{
			rn_linked_list =  rn_linked_list->next;
			sv = assemble_garbage(&rn_linked_list, STDIN_RD);
			if (!dont_be_this[0] || _ft_strcmp(sv, dont_be_this[0]))
				g_redirs[2][in++] = sv;
		}
		printf("stuck two\n");
		if (rn_linked_list && (rn_linked_list->type == ARG || rn_linked_list->type == QUOTE_ARG || rn_linked_list->type == CMD ||  rn_linked_list->type == HERDOC))
			rn_linked_list = rn_linked_list->next;
	}
}

char ***get_garbage_redirs(t_node *rn_linked_list, char **dont_be_this)
{
	t_node *sv;
	char ***g_redirs;
	int count;
	int count_app;
	int count_abort;

	sv = rn_linked_list;
	count = 0;
	count_app = 0;
	count_abort = 0;
	while (rn_linked_list && rn_linked_list->type != OPERATOR) // filter mingled args
	{
		if (rn_linked_list && rn_linked_list->type == STDOUT_RD)
		{
			rn_linked_list = rn_linked_list->next;
			if ((!dont_be_this[2] && assemble_garbage(&rn_linked_list, STDOUT_RD))
				||	_ft_strcmp(assemble_garbage(&rn_linked_list, STDOUT_RD), // !dont_be_this[3] is because different type of ops make the opposite useless as well
				dont_be_this[2])) // operate on the same file, don't take address
				count++;
		}
		if (rn_linked_list && rn_linked_list->type == APPEND)
		{
			rn_linked_list = rn_linked_list->next;
			if ((!dont_be_this[3] && assemble_garbage(&rn_linked_list, APPEND))
				|| _ft_strcmp(assemble_garbage(&rn_linked_list, APPEND),
				dont_be_this[3]))
				count_app++;
		}
		if (rn_linked_list && rn_linked_list->type == STDIN_RD)
		{
			rn_linked_list =  rn_linked_list->next;
			if ((!dont_be_this[0] && assemble_garbage(&rn_linked_list, STDIN_RD)) 
				|| _ft_strcmp(assemble_garbage(&rn_linked_list, STDIN_RD),
				dont_be_this[0]))
				count_abort++;
		}
		printf("stuck one, %s\n", rn_linked_list->data);
		if (rn_linked_list && (rn_linked_list->type == ARG || rn_linked_list->type == QUOTE_ARG || rn_linked_list->type == CMD || rn_linked_list->type == HERDOC))
			rn_linked_list = rn_linked_list->next;
	}
	g_redirs = ft_calloc(sizeof (char **), 3);
	g_redirs[0] = ft_calloc(sizeof (char *), count + 1);
	g_redirs[1] = ft_calloc(sizeof (char *), count_app + 1);
	g_redirs[2] = ft_calloc(sizeof (char *), count_abort + 1);
	get_them_forreal(sv, dont_be_this, g_redirs);
	return (g_redirs);
}

char **alloc_heredocs(t_node *linked_list)
{
	int sz;
	char ** result;
	t_node *sv;

	sv = linked_list;
	sz = 0;
	while (linked_list)
	{
		if (linked_list->type == HERDOC && linked_list->next)
			sz++;
		linked_list = linked_list->next;
	}
	result = ft_calloc(sizeof(char *), sz + 1);
//sz = 0;
//	linked_list = sv;
	//while (linked_list)
	//{
	//	if (linked_list->type == HERDOC && linked_list->next)
	//	{
	//		//if (linked_list->next->before_env)
	//		//	result[sz] = linked_list->next->before_env;
	//		//else
	//		//	result[sz] = linked_list->next->data;
	//		//if (linked_list->next->exp_nosplit) // this must be last! next->*
	//		//	skip_expand_split(&linked_list);
	//		linked_list = linked_list->next;
	//		result[sz] = assemble_garbage(&linked_list, HERDOC);
	//		printf("res %s\n", result[sz]);
	//		sz++;
	//	}
	//	linked_list = linked_list->next;
	//}
	return (result);
}

// always skip operator, since it's considered in number of arguments
t_comparsed *parsed_single_cmd(t_node *linked_list, int cmd_count, int exit_stat, char **envp)
{
	int x;
	t_comparsed *parsed;
	t_node		*rn_linked_list;
	
	x = 0;
	parsed = ft_calloc(sizeof(t_comparsed), 1);
	parsed->exit_status = exit_stat;
	parsed->uptodate_env = envp;
	parsed->all_thestuff = linked_list;
	parsed->cmd_count = cmd_count;
	parsed->exec_ready = ft_calloc (sizeof(char **), cmd_count + 1);
	parsed->real_redirects = ft_calloc (sizeof(char **), cmd_count + 1);
	parsed->garbage_redirects_arr = ft_calloc (sizeof(char **), cmd_count + 1);
	//parsed->heredocs = find_heredocs(linked_list);
	parsed->heredocs = alloc_heredocs(linked_list);
	while (x < cmd_count && linked_list) // this has to be really really the number of commands
	{	// command arg parsing doesn't care about number of spaces, however you need to save the real number of spaces for the redirects since it's an ambiguous redirect!
		rn_linked_list = linked_list;
	//	printf("2\n");
		parsed->exec_ready[x] = get_exec_rdy(&linked_list);
		//printf("3 %s %s %s %s\n", parsed->exec_ready[x][0], parsed->exec_ready[x][1], parsed->exec_ready[x][2], parsed->exec_ready[x][3]);
		parsed->real_redirects[x] = get_real_redirs(rn_linked_list, parsed->heredocs);
	//	printf("rl %s %s\n", parsed->real_redirects[x][0], parsed->real_redirects[x][2]);
		parsed->garbage_redirects_arr[x] = get_garbage_redirs(rn_linked_list, parsed->real_redirects[x]); // redo
		//printf("trash %s %s %s\n", parsed->garbage_redirects_arr[x][0][0],
	//			parsed->garbage_redirects_arr[x][1][0], parsed->garbage_redirects_arr[x][2][0]);
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
	return (x);
}

char **env_toarray(t_env *env)
{
	char **envp;
	int		x;
	char *tmp;

	x = 0;
	envp = ft_calloc(sizeof(char *), node_count(env) + 1); //+ 1)); <---- what was this for?
	while (env)
	{
		if (!env->only_export)
		{
			tmp = ft_strjoin(env->key, "="); // This will be freed in fail()
			envp[x] = ft_strjoin(tmp, env->value);
			x++;
		}
		env = env->next;
	}
	return (envp);
}