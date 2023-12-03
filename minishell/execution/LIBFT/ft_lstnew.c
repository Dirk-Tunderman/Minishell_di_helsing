/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 14:33:35 by eamrati           #+#    #+#             */
/*   Updated: 2022/10/07 15:52:10 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*structure;

	structure = malloc(sizeof(t_list));
	if (structure == 0)
		return (0);
	structure->content = content;
	structure->next = 0;
	return (structure);
}
