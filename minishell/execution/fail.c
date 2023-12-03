/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fail.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 21:10:44 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/03 14:20:40 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void *alloc_wrapper(void *allocation)
{
	static t_alloc *nodes;
	
	if (!allocation)
		exit(OUT_OF_MEMORY);
	if (!nodes)
		nodes = fail(ft_lstnew(allocation));
	else
		ft_lstadd_back(&nodes, fail(ft_lst_new(allocation)));
	return (allocation);
}

void *fail(void *ret) // doesn't matter to set error code, since it's out minishell, not programs inside
{
	if (!ret)
		alloc_wrapper(ret);
	return (ret);
}