/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 13:45:30 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/03 14:28:30 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void heredoc(char *delimiter, int *fds)
{
	char *result;

	if (pipe(fds))
		fail(0);
	printf(">");
	result = get_next_line(0);
	while (ft_strcmp(result, delimiter))
	{
		if (write(fds[1], result, ft_strlen(result)))
			printf("Error writing to pipe!\n");
		printf(">");
		result = get_next_line(0);
	}
}
// implement history for heredoc

void set_redirects(char **redirects, int *fds)
{
	if (redirects[0])
	{
		fds[0] = open(redirects[0], O_RDONLY, NULL);
		if (fds[0] == -1)
			printf("Error opening file %s\n", redirects[0]); // SET ERROR CODE!
	}
	if (redirects[2])
	{
		fds[3] = open(redirects[2], O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR);
		if (fds[3] == -1)
			printf("Error opening file %s\n", redirects[2]); // SET ERROR CODE!
	}
	if (redirects[3])
	{
		fds[4] = open(redirects[3], O_WRONLY | O_CREAT, S_IWUSR);
		if (fds[4] == -1)
			printf("Error opening file %s\n", redirects[3]); // SET ERROR CODE!
	}
}

void set_invalid(int *fds, int x)
{
	while (x >= 0)
	{
		fds[x] = -1;
		x--;
	}
}

void call_heredocs(t_comparsed *cmds, int **fds)
{
	int x;

	x = 0;
	while (x < cmds->cmd_count)
	{
		fds[x] = alloc_wrapper(ft_calloc(sizeof(int), 5));
	//	if (!fds)
	//		fail();
		set_invalid(fds, x);
		if (cmds->real_redirects[x][1])
			heredoc(cmds->real_redirects[x][1], &fds[x][1]);
		x++;
	}
}

void set_fds(t_comparsed *cmds, int **fds)
{
	int x;

	fds = alloc_wrapper(ft_calloc(sizeof(int *), cmds->cmd_count));
//	if (!fds)
//		fail();
	x = 0;
	call_heredocs(cmds, fds); // fds are allocarted here
}