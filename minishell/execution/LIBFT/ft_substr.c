/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 20:16:06 by eamrati           #+#    #+#             */
/*   Updated: 2022/10/17 13:45:02 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char		*substr;
	size_t		a;

	substr = malloc (len + 1);
	a = 0;
	if (substr == 0 || s == 0)
		return (0);
	while (a < len && start + a < ft_strlen(s))
	{
		substr[a] = s[start + a];
		a++;
	}
	substr[a] = '\x00';
	return (substr);
}
