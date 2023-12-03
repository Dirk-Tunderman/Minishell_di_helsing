/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 23:54:17 by eamrati           #+#    #+#             */
/*   Updated: 2022/10/19 18:10:34 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t				a;
	const unsigned char	*s1c;
	const unsigned char	*s2c;

	a = 0;
	s1c = (const unsigned char *)s1;
	s2c = (const unsigned char *)s2;
	while (s1[a] != 0 && s2[a] != 0 && a < n)
	{
		if (s1c[a] > s2c[a])
			return (1);
		else if (s1c[a] < s2c[a])
			return (-1);
		a++;
	}
	if ((s1[a] == 0 || s2[a] == 0) && a < n)
	{
		if (s1c[a] > s2c[a])
			return (1);
		else if (s1c[a] < s2c[a])
			return (-1);
	}
	return (0);
}
