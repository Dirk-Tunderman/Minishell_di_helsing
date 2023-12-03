/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 20:14:18 by eamrati           #+#    #+#             */
/*   Updated: 2022/10/19 16:32:37 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*result;
	size_t	a;

	if (s == 0 || f == 0)
		return (0);
	result = malloc(ft_strlen(s) + 1);
	if (result == 0)
		return (0);
	a = 0;
	while (a < ft_strlen(s))
	{
		result[a] = f(a, s[a]);
		a++;
	}
	result[a] = 0;
	return (result);
}
