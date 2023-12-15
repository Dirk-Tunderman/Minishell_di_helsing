/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fail.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtunderm <dtunderm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 21:10:44 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/15 19:36:03 by dtunderm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void *alloc_wrapper(void *allocation, int mode, void *additional)
{
	static t_alloc *nodes;
	static t_env	*sv_additional;
	t_alloc *tmp;
	if (additional)
		sv_additional = (t_env *)additional;
	if (mode == 2)
		return (sv_additional);
	if (mode)
	{
		if (nodes)
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
	}
	if (!allocation)
	{
		exit(exit_status(YIELD));
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

void *fail(void *ret, int smth)
{
	(void) smth;
	printf("fail\n");
	if (!ret)
		alloc_wrapper(0, 1, 0);
	printf("fail 2\n");
	return (ret);
}

void *wrap_wrap(void *allocation)
{
	if (!allocation)
		alloc_wrap(0);
	return (allocation);
}

void fail_exit()
{
	alloc_wrap(0);
}
