/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fail.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 21:10:44 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/05 14:29:39 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void *alloc_wrapper(void *allocation, int mode, void *additional) // This function should not be called on temporary allocations that will be released in the same scope, otherwise double free!!!
{ // DO NOT USE THIS FUNCTION IF YOU INTEND TO FREE BEFORE CALLING RESET()/FAIL(0)
	static t_alloc *nodes;
	static void		*sv_additional;
	t_alloc *tmp;
	
	if (additional)
		sv_additional = additional;
	if (mode == 2)
		return (additional);
	if (mode)
	{
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
		exit(OUT_OF_MEMORY);
	if (!nodes)
		nodes = fail(ft_lstnew(allocation), 0);
	else
		ft_lstadd_back(&nodes, fail(ft_lstnew(allocation), 0));
	return (allocation);
}

void *fail(void *ret, int smth) // doesn't matter to set error code, since it's out minishell, not programs inside
{
	(void) smth;
	if (!ret)
	{
		alloc_wrapper(0, 1, 0); // call with mode set when you want to reset, call with additional set if you have some allocation done that shouldn't be lost if a malloc fails!
		main_loop(0, 0);
	}
	return (ret);
}

void *alloc_wrap(void *arg) 
{
	return (alloc_wrapper(arg, 0, 0));
}

void save_alloc(void *sv)
{
	alloc_wrapper(0, 2, sv);
}

void fail_exit()
{
	alloc_wrap(0);
}
