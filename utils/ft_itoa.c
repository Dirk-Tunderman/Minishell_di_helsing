/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 21:40:50 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/06 17:50:22 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	reverse(int sign, char *a, int counter)
{
	int		i;
	int		j;
	int		swap;

	i = 0;
	if (sign == -1)
		i = 1;
	while (i < ft_strlen(a))
	{
		j = 0;
		if (sign == -1)
			j = 1;
		counter--;
		while (j < counter)
		{
			swap = a[j];
			a[j] = a[j + 1];
			a[j + 1] = swap;
			j++;
		}
		i++;
	}
}

static void	after_init(long long n2, char *a, int *sign, int *counter)
{
	if (n2 < 0)
	{
		a[0] = '-';
		(*counter)++;
		*sign = -1;
		n2 *= -1;
	}
	while (n2 != 0)
	{
		a[(*counter)++] = (n2 % 10) + '0';
		n2 = n2 / 10;
	}
}

void	*ft_calloc(size_t count, size_t size)
{
	unsigned long long		a;
	char					*ptr;

	a = 0;
	ptr = malloc(count * size);
	if (ptr == 0)
		return (0);
	while (a < count * size)
		ptr[a++] = 0;
	return (ptr);
}

char	*ft_itoa(int n)
{
	char		*a;
	int			counter;
	long long	n2;
	int			sign;

	a = ft_calloc(15, 1);
	counter = 0;
	n2 = n;
	sign = 0;
	if (a == 0)
		return (0);
	a[0] = '0';
	after_init(n2, a, &sign, &counter);
	reverse(sign, a, counter);
	return (a);
}
