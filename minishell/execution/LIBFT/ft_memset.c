/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 22:07:17 by eamrati           #+#    #+#             */
/*   Updated: 2022/10/19 15:13:54 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	*ft_memset(void *b, int c, size_t len)
{
	size_t			a;
	unsigned char	*bc;

	a = 0;
	bc = (unsigned char *)b;
	while (a < len)
	{
		bc[a] = (unsigned char)c;
		a++;
	}
	return (b);
}
