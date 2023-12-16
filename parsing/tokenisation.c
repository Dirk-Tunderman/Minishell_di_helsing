/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenisation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtunderm <dtunderm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 16:08:03 by dtunderm          #+#    #+#             */
/*   Updated: 2023/12/16 11:53:59 by dtunderm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	lexer(char *input, t_node **head, t_env *l_env)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (ft_isspace(input[i]))
			i++;
		else
			process_token(input, &i, head, l_env);
	}
}

void	minishell_no_alzheimer(t_node *head, char *data)
{
	while (head->next)
		head = head->next;
	head->exp_nosplit = data;
}

t_make_env	*create_m_e(void)
{
	t_make_env	*m_e;

	m_e = (t_make_env *)malloc(sizeof(t_make_env));
	if (m_e == NULL)
		return (NULL);
	m_e->dol = 0;
	m_e->start = 0;
	m_e->end = 0;
	m_e->non_delim_index = 0;
	m_e->delim = NULL;
	m_e->var_name = NULL;
	m_e->var_value = NULL;
	m_e->result = NULL;
	return (m_e);
}

void	loop_dollar(char *input, int *i, t_make_env *m_e)
{
	while (input[*i] == '$')
	{
		(*i)++;
		m_e->dol++;
	}
	m_e->start = *i;
}

char	*no_var_value(char *input, t_make_env *m_e)
{
	if (m_e->non_delim_index - m_e->start)
	{
		if ((!m_e->end - m_e->non_delim_index))
			return (ft_strndup(&input[m_e->start - m_e->dol], m_e->dol));
		else
			return (ft_strjoin(ft_strndup(&input[m_e->start - m_e->dol],
						m_e->dol), m_e->delim));
	}
	else
		return (ft_strjoin(ft_strndup(&input[m_e->start - m_e->dol - 1],
					m_e->dol + 1), m_e->delim));
}
