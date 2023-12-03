/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 20:50:55 by eamrati           #+#    #+#             */
/*   Updated: 2022/10/20 13:45:43 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <stdio.h>
// es == end_to_start

static void	es(int *beginning, int *end, char const *s1, char const *set)
{
	size_t	b;

	while (*end > *beginning)
	{
		b = 0;
		while (b < ft_strlen(set))
		{
			if (s1[*end] == set[b])
				break ;
			b++;
		}
		if (b == ft_strlen(set))
			break ;
		(*end)--;
	}
}

static void	start_to_end(int *beginning, char const *s1, char const *set)
{
	size_t	b;

	*beginning = 0;
	while (*beginning < (int) ft_strlen(s1))
	{
		b = 0;
		while (b < ft_strlen(set))
		{
			if (s1[*beginning] == set[b])
				break ;
			b++;
		}
		if (b == ft_strlen(set))
			break ;
		(*beginning)++;
	}
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		beginning;
	int		end;
	char	*trimmed;

	if (s1 == 0 || set == 0)
		return (0);
	end = ft_strlen(s1) - 1;
	start_to_end(&beginning, s1, set);
	es(&beginning, &end, s1, set);
	trimmed = malloc(end + 1 - beginning + 1);
	if (trimmed == 0)
		return (0);
	ft_strlcpy(trimmed, &s1[beginning], end + 1 - beginning + 1);
	return (trimmed);
}
