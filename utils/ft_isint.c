/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isint.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 20:37:48 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/13 13:19:29 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>

int	ft_pow(int nb, int pow)
{
	if (pow == 0)
		return (1);
	if (pow == 1)
		return (nb);
	return (nb * ft_pow(nb, --pow));
}

int	int_cmp(int int_val, char *nmb, int len, int sign)
{
	int	a;
	int	val;

	a = len;
	if (nmb[0] == '+')
		nmb++;
	while (len)
	{
		val = ((int_val / ft_pow(10, len - 1)) % 10) * sign ;
		if (val < nmb[a - len] - 48)
			return (0);
		else if (val > nmb[a - len] - 48)
			return (1);
		len--;
	}
	return (1);
}

int	int_tst(int max_min)
{
	int	nb;

	nb = 0;
	while (max_min)
	{
		max_min /= 10;
		nb++;
	}
	return (nb);
}

int	max_min_int(char *arg, int len, int sign)
{
	int	int_max;
	int	int_min;

	int_max = int_tst(INT_MIN);
	int_min = int_tst(INT_MAX);
	if (sign)
		len--;
	if ((sign == -1 && int_min < len) || (sign != -1 && int_max < len))
		return (0);
	else if (sign == -1 && int_min == len)
		return (int_cmp(INT_MIN, arg + 1, len, -1));
	else if (sign != -1 && int_max == len)
		return (int_cmp(INT_MAX, arg, len, 1));
	return (1);
}

int	is_int0(char *arg)
{
	int	a;
	int zer;
	int sign;

	sign = 0;
	a = 0;
	while (*arg == ' ')
		arg++;
	if (arg[a] == '-' || arg[a] == '+')
	{
		a++;
		sign = -1;
		if (arg[a] == '+')
			sign = 1;
	}
	if (arg[a] > 57 || arg[a] < 48)
		return (0);
	while (arg[a] == '0')
		a++;
	zer = a;
	if (arg[a] && (arg[a] > 57 || arg[a] < 48))
		return (0);
	while (!(arg[a] > 57 || arg[a] < 48))
		a++;
	if (arg[a] && (arg[a] > 57 || arg[a] < 48))
		return (0);
	return (max_min_int(&arg[zer], a - zer, sign));
}
