/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 09:51:25 by eamrati           #+#    #+#             */
/*   Updated: 2022/10/16 16:03:18 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		a;
	char	converted;

	converted = (char) c;
	a = ft_strlen(s);
	if (converted == 0)
		return ((char *)(s + ft_strlen(s)));
	while (a >= 0)
	{
		if (s[a] == converted)
			return ((char *)(s + a));
		a--;
	}
	return (0);
}
