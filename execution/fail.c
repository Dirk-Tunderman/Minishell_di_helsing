/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fail.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 21:10:44 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/08 23:19:38 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void *alloc_wrapper(void *allocation, int mode, void *additional) // This function should not be called on temporary allocations that will be released in the same scope, otherwise double free!!!
{ // DO NOT USE THIS FUNCTION IF YOU INTEND TO FREE BEFORE CALLING RESET()/FAIL(0)
	static t_alloc *nodes;
	static t_env	*sv_additional;
	t_alloc *tmp;
	
	if (additional)
		sv_additional = (t_env *)additional;
	if (mode == 2)
		return (sv_additional);
	if (mode)
	{
		sv_additional = duplicate_env(sv_additional);
		while (nodes)
		{
			free(nodes->allocation);
			tmp = nodes;
			nodes = nodes->next;
			free(tmp);
		}
		nodes = 0;
		return (0);
	}
	if (!allocation)
	{
		exit(exit_status(256));
	}
	if (!nodes)
		nodes = ft_lstnew(allocation);
	else
		ft_lstadd_back(&nodes, ft_lstnew(allocation));
	return (allocation);
}

void *alloc_wrap(void *arg) 
{
	return (alloc_wrapper(arg, 0, 0));
}

void save_alloc(void *sv)
{
	alloc_wrapper(0, 2, sv);
}

void *fail(void *ret, int smth) // doesn't matter to set error code, since it's out minishell, not programs inside
{
	(void) smth;
	if (!ret)
		alloc_wrapper(0, 1, 0); // call with mode set when you want to reset, call with additional set if you have some allocation done that shouldn't be lost if a malloc fails!
	return (ret);
}

void fail_exit()
{
	//printf("PID: %d| File %s:\nLine %d: Function %s\n", getpid(), __FILE__, __LINE__, __func__);
	alloc_wrap(0);
}
