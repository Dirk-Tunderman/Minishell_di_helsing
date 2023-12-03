/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 22:03:23 by eamrati           #+#    #+#             */
/*   Updated: 2022/10/19 17:20:58 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <string.h>

static void	move(char *dstc, char *srcc, int end, size_t len)
{
	if (dstc - srcc > 0)
	{	
		end = len - 1;
		while (end >= 0)
		{
			dstc[end] = srcc[end];
			end--;
		}
	}
	else
	{
		end = 0;
		while ((size_t) end < len)
		{
			dstc[end] = srcc[end];
			end++;
		}
	}
}

// end = 0 because of compiler complaining about initialization

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char	*dstc;
	char	*srcc;
	int		end;

	if (dst == src)
		return (dst);
	dstc = (char *) dst;
	srcc = (char *) src;
	end = 0;
	move(dstc, srcc, end, len);
	return (dst);
}
