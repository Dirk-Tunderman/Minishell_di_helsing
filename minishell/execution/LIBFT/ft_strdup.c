/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 23:35:51 by eamrati           #+#    #+#             */
/*   Updated: 2022/10/16 21:15:05 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <sys/errno.h>

char	*ft_strdup(const char *s1)
{
	size_t		a;
	char		*str;

	a = 0;
	str = malloc (ft_strlen(s1) + 1);
	if (str == 0)
	{
		errno = ENOMEM;
		return (0);
	}
	while (a < ft_strlen(s1))
	{
		str[a] = s1[a];
		a++;
	}
	str[a] = 0;
	return (str);
}
