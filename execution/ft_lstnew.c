/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 14:33:35 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/14 21:40:53 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_alloc	*ft_lstnew(void *content)
{
	t_alloc	*structure;

	structure = malloc(sizeof(t_alloc));
	if (structure == 0)
		return (0);
	structure->allocation = content;
	structure->next = 0;
	return (structure);
}

t_alloc	*ft_lstlast(t_alloc *lst)
{
	if (lst == 0)
		return (0);
	while (lst->next != 0)
		lst = lst->next;
	return (lst);
}
void	ft_lstadd_back(t_alloc **lst, t_alloc *new)
{
	t_alloc	*end;

	if (lst == 0)
		return ;
	if (*lst == 0)
	{
		*lst = new;
		return ;
	}
	end = ft_lstlast(*lst);
	end->next = new;
}


