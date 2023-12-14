/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 13:45:30 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/14 16:20:55 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int heredoc(char *delimiter, int *fds, int type, t_comparsed *cmds)
{
	char *result;
	char **intheenv;

	result = 0;
	if (pipe(fds))
		return (SYSCALLFAIL);
	result = readline("> ");
	if (!result)
	{
		close(fds[1]);
		return (0);
	}
	while (_ft_strcmp(result, delimiter)) // still issue with quotes expanding interpret inside
	{
		intheenv = get_quoted_word(result, &(int){-1}, cmds->environment, '\"', 1); // '' is same as ""
		if (type != QUOTE_ARG)
		{
		 	if (write(fds[1], intheenv[1], ft_strlen(intheenv[1])) == -1)
				return (printf("Error writing to pipe!\n"), close(fds[1]), free(result), SYSCALLFAIL);
		}
		else 
		{
			if	(write(fds[1], result, ft_strlen(result)) == -1)
				return (printf("Error writing to pipe!\n"), close(fds[1]), free(result), SYSCALLFAIL);
		}
		if (write(fds[1], "\n", 1) == -1)
			return (printf("Error writing to pipe!\n"), close(fds[1]), free(result), SYSCALLFAIL);
		free(result);
		result = readline("> ");
		if (!result)
		{
			close(fds[1]);
			return (0);
		}
	}
	//printf("pipe %d\n", fds[1]);
	free(result);
	close(fds[1]); //????
	return (0);
}
// implement history for heredoc

int set_redirects(char **redirects, int *fds)
{
	if (redirects[0])
	{
		fds[0] = open(redirects[0], O_RDONLY, NULL);
		if (fds[0] == -1)
		{
			printf("Error opening file %s\n", redirects[0]); // SET ERROR CODE!
			// these should be reset() instead
			return (ABORTCURRENTCMD);
		}	
	}
	if (redirects[2])
	{
		fds[3] = open(redirects[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
		if (fds[3] == -1)
		{
			printf("Error opening file %s\n", redirects[2]); // SET ERROR CODE!
			return (ABORTCURRENTCMD);
		}
	}
	if (redirects[3])
	{
		fds[4] = open(redirects[3], O_WRONLY | O_CREAT, S_IRWXU);
		if (fds[4] == -1)
		{
			printf("Error opening file %s\n", redirects[3]); // SET ERROR CODE!
			return (ABORTCURRENTCMD);
		}
	}
	return (0);
}

int * set_invalid(int *fds)
{
	int x;

	x = 0;
	while (x < 5)
	{
		fds[x] = -1;
		x++;
	}
	return (fds);
}

int fake_heredoc(char *delimiter)
{
	char *result;
	int fds[2];

	result = 0;
	if (pipe(fds))
		return (WHOCARES);
	result = readline("> ");
	if (!result)
	{
		close(fds[0]);
		close(fds[1]);
		return (0);
	}
	while (_ft_strcmp(result, delimiter))
	{
		if (write(fds[1], result, ft_strlen(result)) == -1)
			return (printf("Error writing to pipe!\n"), close(fds[0]), close(fds[1]), free(result), WHOCARES);
		if (write(fds[1], "\n", 1) == -1)
			return (printf("Error writing to pipe!\n"), close(fds[0]), close(fds[1]), free(result), WHOCARES);
		free(result);
		result = readline("> ");
		if (!result)
		{
			close(fds[0]);
			close(fds[1]);
			return (0);
		}
	}
	free(result);
	close(fds[0]);
	close(fds[1]); //????
	return (0);
}

int detnxttype(t_node *ll, int x) // DEC 13, VERIFY LATER!!!
{
	t_node* prev;
	int c;

	c = 0;
	prev = ll;
	while (ll && (c != x || ll->type != HERDOC))
	{
		if (ll->type == HERDOC)
			c++;
		ll = ll->next;
	}
	if (ll)
		ll = ll->next;
	while (ll && ll->type != HERDOC && (skip_expand_split(&ll) || 1) && ll->space_after == 0)
	{
		if (ll->type == QUOTE_ARG)
			return (QUOTE_ARG);
		ll = ll->next;
	}
	return (ARG);
	//while (ll && (addr != ll->data && addr != ll->before_env))
	//	ll = ll->next;
	//return (ll->type);
}

int call_heredocs(t_comparsed *cmds, int **fds)
{
	int x;
	int c;

	c = 0;
	x = 0;
	while (x < cmds->cmd_count)
	{
		//printf("HERDOC %s\n", cmds->heredocs[c]);
		fds[x] = set_invalid(ft_calloc(sizeof(int), 5));
		//if (cmds->real_redirects[x][1])
		//{
		// HEREDOC YOU CLOSED FILE DESCRIPTOR SOMEWHERE/DIDN'T OPEN!
		// cmp real redir with current before env!
		// ONLY COMPARE BY ADDRESS!
		if (cmds->real_redirects[x][1] && cmds->heredocs[c] == cmds->real_redirects[x][1]) 
		{
			if (heredoc(cmds->real_redirects[x][1], &fds[x][1],
				detnxttype(cmds->all_thestuff, x)
				, cmds) == SYSCALLFAIL)
				return (SYSCALLFAIL);
		}
		else
		{
			while (cmds->heredocs[c] && cmds->heredocs[c] != cmds->real_redirects[x][1])
			{			
			//	printf("%s %s\n", cmds->heredocs[c], cmds->real_redirects[x][1]);
				fake_heredoc(cmds->heredocs[c]);
				c++;
			}
			if (cmds->heredocs[c])
				if (heredoc(cmds->real_redirects[x][1], &fds[x][1],
					detnxttype(cmds->all_thestuff, x)
					, cmds) == SYSCALLFAIL)
					return (SYSCALLFAIL);
		}
		if (cmds->heredocs[c])
			c++;
		x++;
	}
	return (0);
}

void end_command(t_comparsed *cmds)
{
	static t_comparsed *save;
	int x;

	
	if (cmds)
		save = cmds;
	if (!cmds)
		close(STDIN_FILENO);
	if (!cmds && save)
	{
		x = 0;
		while (x < save->cmd_count && save->fds[x])
		{
			if (save->fds[x][1] != -1)
			{
				close(save->fds[x][1]);
				save->fds[x][1] = -1;
			}
			x++;
		}
		save->cmd_count = 0;
	}
}

void herestop(int x)
{
	(void) x;
	end_command(0);
	exit_status(130);
}

int set_fds(t_comparsed *cmds, int ***fds)
{
	sigset_t type;

	sigemptyset(&type);
	*fds = ft_calloc(sizeof(int *), cmds->cmd_count);
 	end_command(cmds);
	sigaction(SIGINT, &(struct sigaction){.__sigaction_u.__sa_handler = herestop, .sa_mask = type, .sa_flags = 0}, NULL);
	return (call_heredocs(cmds, *fds)); // fds are allocated here
}