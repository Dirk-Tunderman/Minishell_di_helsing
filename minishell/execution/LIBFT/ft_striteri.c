/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 23:39:17 by eamrati           #+#    #+#             */
/*   Updated: 2022/10/16 15:55:41 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	size_t	a;

	if (s == 0 || f == 0)
		return ;
	a = 0;
	while (a < ft_strlen(s))
	{
		f(a, &s[a]);
		a++;
	}
}
