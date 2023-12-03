/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 09:47:33 by eamrati           #+#    #+#             */
/*   Updated: 2022/10/19 17:04:53 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

// double while loops, fixate a character then search the needle, 
// don't overpass the limits "strlen needle\haystack" & "maxlen"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t		a;
	size_t		b;

	a = 0;
	if (ft_strlen(needle) == 0)
		return ((char *)haystack);
	while (a < len && haystack[a] != 0)
	{
		b = 0;
		while (b < ft_strlen(needle) && b < ft_strlen(haystack) - a
			&& b < len - a)
		{
			if (haystack[a + b] != needle[b])
				break ;
			b++;
		}
		if (b == ft_strlen(needle))
		{
			return ((char *)(haystack + a));
		}
		a++;
	}
	return (0);
}
