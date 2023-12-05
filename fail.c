/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fail.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 21:10:44 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/05 10:41:53 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void *alloc_wrapper(void *allocation, int mode) // This function should not be called on temporary allocations that will be released in the same scope, otherwise double free!!!
{ // DO NOT USE THIS FUNCTION IF YOU INTEND TO FREE BEFORE CALLING RESET()/FAIL(0)
	static t_alloc *nodes;
	t_alloc *tmp;
	
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
		nodes = fail(ft_lstnew(allocation));
	else
		ft_lstadd_back(&nodes, fail(ft_lstnew(allocation)));
	return (allocation);
}

void *fail(void *ret) // doesn't matter to set error code, since it's out minishell, not programs inside
{
	if (!ret)
	{
		alloc_wrapper(0, 1);
		main_loop(0, 0);
	}
	return (ret);
}

void fail_exit()
{
	alloc_wrap(0);
}
