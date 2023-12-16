/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtunderm <dtunderm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 11:43:25 by dtunderm          #+#    #+#             */
/*   Updated: 2023/12/16 11:53:09 by dtunderm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	process_dollar(char *input, int *i, t_env *l_env, t_p_t_p *params)
{
	params->temp = *i;
	params->before_env = get_env_var_ex(input, *i);
	params->data = get_env_var(input, i, l_env);
	params->space_flag = ft_isspace(input[*i]);
	params->type = ARG;
}

char	*get_env_var_ex(char *input, int i)
{
	int		start;
	char	*var_name;
	int		dollar;

	dollar = 0;
	start = i;
	while (input[i] == '$')
	{
		i++;
		dollar++;
	}
	while (input[i] && !ft_isspace(input[i]) && !is_operator(input[i])
		&& input[i] != '\"' && input[i] != '\'' && input[i] != '$')
		i++;
	var_name = ft_strndup(&input[start], i - start);
	return (var_name);
}

char	*get_env_var(char *input, int *i, t_env *l_env)
{
	t_make_env	*m_e;
	char		*result;

	m_e = create_m_e();
	(*i)++;
	loop_dollar(input, i, m_e);
	if (input[*i] == '?')
		get_var_es(input, i, m_e);
	else
	{
		if (get_var(input, i, m_e, l_env) == -1)
			return (ft_strndup(&input[m_e->start - m_e->dol - 1],
					m_e->dol + 1));
	}
	if (m_e->var_value == NULL)
		return (no_var_value(input, m_e));
	m_e->result = ft_calloc(sizeof(char),
			ft_strlen(m_e->var_value) + 1 + ft_strlen(m_e->delim) + m_e->dol);
	ft_strcat(m_e->result,
		ft_strndup(&input[m_e->start - m_e->dol], m_e->dol));
	ft_strcat(m_e->result, m_e->var_value);
	ft_strcat(m_e->result, m_e->delim);
	result = ft_strdup(m_e->result);
	return (m_e->result);
}

void	get_var_es(char *input, int *i, t_make_env *m_e)
{
	(*i)++;
	m_e->non_delim_index = *i;
	while (input[*i] && !ft_isspace(input[*i]) && !is_operator(input[*i])
		&& input[*i] != '\"' && input[*i] != '\'' && input[*i] != '$')
		(*i)++;
	m_e->end = *i;
	m_e->delim = ft_substr(&input[m_e->non_delim_index],
			0, m_e->end - m_e->non_delim_index);
	m_e->var_value = ft_itoa(exit_status(YIELD));
}

int	get_var(char *input, int *i, t_make_env *m_e, t_env *l_env)
{
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_' ))
		(*i)++;
	m_e->non_delim_index = *i;
	while (input[*i] && !ft_isspace(input[*i]) && !is_operator(input[*i])
		&& input[*i] != '\"' && input[*i] != '\'' && input[*i] != '$')
		(*i)++;
	m_e->end = *i;
	if (m_e->end == m_e->start - m_e->dol)
		return (-1);
	m_e->delim = ft_substr(&input[m_e->non_delim_index],
			0, m_e->end - m_e->non_delim_index);
	m_e->var_name = ft_strndup(&input[m_e->start],
			m_e->non_delim_index - m_e->start);
	m_e->var_value = find_env_var(m_e->var_name, l_env);
	return (1);
}
