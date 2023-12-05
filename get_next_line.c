/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 23:26:40 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/04 20:06:22 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	check_linepos(char *buffer)
{
	int	a;

	a = 0;
	if (!buffer)
		return (0);
	while (buffer[a] != '\n' && buffer[a] != '\0')
		a++;
	if (buffer[a] == '\n')
		return (1);
	return (0);
}

int	read_it(char **buffer, int fd, t_tools *tools)
{
	tools->x = 0;
	tools->temp = ft_calloc(BUFFER_SIZE + 1, 1);
	if (tools->temp == 0)
		return (-1);
	while (!check_linepos(buffer[fd]))
	{
				printf("%s\n", tools->temp);

		tools->ret = read(fd, tools->temp, BUFFER_SIZE);
		if (tools->ret == 0)
			break ;
		if (tools->ret == -1)
			return (-1);
		tools->temp[tools->ret] = 0;
		buffer[fd] = _ft_strjoin(buffer, tools->temp, tools->ret, fd);
		if (buffer[fd] == 0)
			return (-1);
	}
	return (1);
}

char	*extractnl(char **buffer, int fd, t_tools *tools)
{
	int	a;

	a = 0;
	while (buffer[fd][a] != 0 && buffer[fd][a] != '\n')
		a++;
	tools->newlinepos = a;
	if (buffer[fd][a] == '\n')
		tools->newlinepos = a + 1;
	return (_ft_strdup(buffer, tools->newlinepos, fd));
}

char	*reallign(char **buffer, t_tools *tools, int fd)
{
	int		a;
	char	*res;

	a = 0;
	res = ft_calloc(_ft_strlen(&buffer[fd][tools->newlinepos]) + 1, 1);
	if (res == 0)
		return (0);
	while (buffer[fd][tools->newlinepos + a])
	{
		res[a] = buffer[fd][tools->newlinepos + a];
		a++;
	}
	free(buffer[fd]);
	buffer[fd] = 0;
	return (res);
}

char	*get_next_line(int fd)
{
	static char	*buffer[OPEN_MAX];
	t_tools		tools;

	tools.freeallsignal = 0;
	tools.newlinepos = 0;
	tools.result = 0;
	tools.ret = 0;
	tools.temp = 0;
	if (!buffer[fd])
		buffer[fd] = ft_calloc(1, 1);
	if (buffer[fd] == 0)
		return (freeall(buffer, &tools, -1, fd));
	if (read_it(buffer, fd, &tools) == -1)
		return (freeall(buffer, &tools, -1, fd));
	tools.result = extractnl(buffer, fd, &tools);
	if (tools.result == 0)
		return (freeall(buffer, &tools, -1, fd));
	buffer[fd] = reallign(buffer, &tools, fd);
	if (buffer[fd] == 0)
		return (freeall(buffer, &tools, -1, fd));
	freeall(buffer, &tools, 0, fd);
	if (tools.result == 0)
		return (0);
	return (tools.result);
}
