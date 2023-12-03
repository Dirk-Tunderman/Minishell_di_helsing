/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 21:44:35 by eamrati           #+#    #+#             */
/*   Updated: 2022/10/19 11:58:21 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t					a;
	unsigned char			*sc;

	a = 0;
	sc = (unsigned char *) s;
	while (a < n)
	{
		if (sc[a] == (unsigned char)c)
			return ((void *)(sc + a));
		a++;
	}
	return (0);
}
