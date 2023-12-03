/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 23:44:32 by eamrati           #+#    #+#             */
/*   Updated: 2022/10/19 16:30:17 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <stdio.h>

// that srclen is bigger than dstsize - dstlen 
// doesn't mean it overflows, it merely stops somewhere.. 

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t		dstlen;
	size_t		a;

	if (dst == 0 && dstsize == 0)
		return (ft_strlen(src));
	dstlen = ft_strlen(dst);
	a = 0;
	while (a < ft_strlen(src) && a + dstlen + 1 < dstsize)
	{
		dst[dstlen + a] = src[a];
		a++;
	}
	if (dstsize - dstlen > 0)
		dst[dstlen + a] = '\x00';
	if (dstsize > dstlen)
		return (dstlen + ft_strlen(src));
	return (dstsize + ft_strlen(src));
}
