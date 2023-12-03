/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 23:30:12 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/03 14:33:10 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*_ft_strdup(char **str, int len, int fd)
{
	int		a;
	char	*res;

	a = 0;
	res = ft_calloc(len + 1, 1);
	if (res == 0)
		return (0);
	while (a < len)
	{
		res[a] = str[fd][a];
		a++;
	}
	res[a] = 0;
	return (res);
}

void	*freeall(char **buffer, t_tools *tools, int signal, int fd)
{
	if (signal == -1)
	{
		if (tools->result)
			free(tools->result);
		if (buffer[fd])
			free(buffer[fd]);
		buffer[fd] = 0;
		tools->result = 0;
	}
	else
	{
		if (tools->result[0] == 0)
		{
			if (buffer[fd])
				free(buffer[fd]);
			if (tools->result)
				free(tools->result);
			tools->result = 0;
			buffer[fd] = 0;
		}
	}
	if (tools->temp)
		free(tools->temp);
	tools->temp = 0;
	return (0);
}

long long	_ft_strlen(const char *s)
{
	int	a;

	a = 0;
	if (!s)
		return (0);
	while (s[a] != '\x00')
		a++;
	return (a);
}

char	*_ft_strjoin(char **buffer1, char *buffer2, int len, int fd)
{
	int		a;
	int		b;
	char	*res;
	int		x;

	x = _ft_strlen(buffer1[fd]);
	a = 0;
	b = 0;
	res = ft_calloc (x + len + 1, 1);
	if (res == 0)
		return (0);
	while (a < x)
	{
		res[a] = buffer1[fd][a];
		a++;
	}
	while (b < len)
	{
		res[a] = buffer2[b];
		a++;
		b++;
	}
	free(buffer1[fd]);
	buffer1[fd] = 0;
	return (res);
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
	{
		ptr[a++] = 0;
	}
	return (ptr);
}
