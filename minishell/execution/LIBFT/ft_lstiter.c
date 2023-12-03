/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 12:19:18 by eamrati           #+#    #+#             */
/*   Updated: 2022/10/14 15:43:03 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	t_list	*navigator;

	if (lst == 0 || f == 0)
		return ;
	navigator = lst;
	while (navigator != 0)
	{
		f(navigator->content);
		navigator = navigator->next;
	}
}
