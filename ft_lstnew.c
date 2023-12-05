/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 14:33:35 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/03 10:48:09 by eamrati          ###   ########.fr       */
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


