/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 15:59:01 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/04 23:37:24 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	*ft_memset(void *b, int c, size_t len)
{
	size_t			a;
	unsigned char	*bc;

	a = 0;
	bc = (unsigned char *)b;
	while (a < len)
	{
		bc[a] = (unsigned char)c;
		a++;
	}
	return (b);
}

// The value of this symbolic constant is the total number of signals defined. 
//Since the signal numbers are allocated consecutively, NSIG is also one greater 
//than the largest defined signal number.

void	handler(int sig, siginfo_t *info, void *ucontext)
{
	fail(0);
}

int restarter_setter()
{
	int x;
	struct sigaction act;

	ft_memset(&act, 0, sizeof(struct sigaction));
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGQUIT);
	act.sa_flags = SA_SIGINFO;
	act.__sigaction_u.__sa_sigaction = handler;
	sigaction(x, &act, NULL);
}