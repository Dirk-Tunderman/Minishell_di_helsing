/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 13:45:30 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/08 22:25:48 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void heredoc(char *delimiter, int *fds)
{
	char *result;

	result = 0;
	if (pipe(fds))
	{
		fail(0, 0);
		return ;
	}
	if (!g_signal_rec)
		result = readline("> ");
	if (!result)
	{
		close(fds[1]);
		return;
	}
	while (_ft_strcmp(result, delimiter))
	{
		if (write(fds[1], result, ft_strlen(result)) == -1)
			return (printf("Error writing to pipe!\n"), free(result));
		if (write(fds[1], "\n", 1) == -1)
			return (printf("Error writing to pipe!\n"), free(result));
		free(result);
		result = readline("> ");
		if (!result)
		{
			close(fds[1]);
			return;
		}
	}
	free(result);
	close(fds[1]); //????
}
// implement history for heredoc

void set_redirects(char **redirects, int *fds)
{
	if (redirects[0])
	{
		fds[0] = open(redirects[0], O_RDONLY, NULL);
		if (fds[0] == -1)
		{
			printf("Error opening file %s\n", redirects[0]); // SET ERROR CODE!
			fail(0, 0);// these should be reset() instead
			return ;
		}	
	}
	if (redirects[2])
	{
		fds[3] = open(redirects[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
		if (fds[3] == -1)
		{
			printf("Error opening file %s\n", redirects[2]); // SET ERROR CODE!
			fail(0, 0);
			return ;
		}
	}
	if (redirects[3])
	{
		fds[4] = open(redirects[3], O_WRONLY | O_CREAT, S_IRWXU);
		if (fds[4] == -1)
		{
			printf("Error opening file %s\n", redirects[3]); // SET ERROR CODE!
			fail(0, 0);
			return ;
		}
	}
}

void set_invalid(int *fds)
{
	int x;

	x = 0;
	while (x < 5)
	{
		fds[x] = -1;
		x++;
	}
}

void call_heredocs(t_comparsed *cmds, int **fds)
{
	int x;

	x = 0;
	while (x < cmds->cmd_count)
	{
		fds[x] = ft_calloc(sizeof(int), 5);
		set_invalid(fds[x]);
		if (cmds->real_redirects[x][1])
		{
			heredoc(cmds->real_redirects[x][1], &fds[x][1]);
			restore_fds();
		}
		x++;
	}
}

void set_fds(t_comparsed *cmds, int ***fds)
{
	*fds = ft_calloc(sizeof(int *), cmds->cmd_count);
	call_heredocs(cmds, *fds); // fds are alloarted here
}