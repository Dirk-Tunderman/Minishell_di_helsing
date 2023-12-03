/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 21:52:47 by eamrati           #+#    #+#             */
/*   Updated: 2022/10/19 18:09:35 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "libft.h"
#include <stdlib.h>
#include <string.h>

// We work with unsigned char because we need to substract and get indexes,
// not values that are relevant only mathematically

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t				a;
	const unsigned char	*s1c;
	const unsigned char	*s2c;

	a = 0;
	s1c = s1;
	s2c = s2;
	while (a < n)
	{
		if (s1c[a] > s2c[a])
			return (1);
		else if (s1c[a] < s2c[a])
			return (-1);
		a++;
	}
	return (0);
}
