/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 12:21:15 by eamrati           #+#    #+#             */
/*   Updated: 2022/10/14 17:22:12 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

//	navigator for the given list, heading for the created list

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*heading;
	t_list	*header;

	if (lst == 0 || f == 0 || del == 0)
		return (0);
	heading = ft_lstnew(f(lst->content));
	header = heading;
	if (header == 0)
		return (0);
	lst = lst->next;
	while (lst)
	{
		heading->next = ft_lstnew(f(lst->content));
		if (heading->next == 0)
		{
			ft_lstclear(&header, del);
			return (header);
		}
		heading = heading->next;
		lst = lst->next;
	}
	return (header);
}
