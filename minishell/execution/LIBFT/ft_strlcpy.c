/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 23:49:31 by eamrati           #+#    #+#             */
/*   Updated: 2022/10/20 13:54:00 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	a;

	a = 0;
	while (a < ft_strlen(src) && a + 1 < dstsize)
	{
		dst[a] = src[a];
		a++;
	}
	if (dstsize > 0)
		dst[a] = '\x00';
	return (ft_strlen(src));
}
