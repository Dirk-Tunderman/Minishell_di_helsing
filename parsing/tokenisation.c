/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenisation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtunderm <dtunderm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 16:08:03 by dtunderm          #+#    #+#             */
/*   Updated: 2023/12/15 19:42:26 by dtunderm         ###   ########.fr       */
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

char	**get_result(char *input, int *i, t_env *l_env, t_p_t_p *par)
{
	char	*env_variable;
	char	*before_env;
	char	**result;
	int		start;

	result = ft_calloc(sizeof(char *), 2);
	result[0] = ft_calloc(sizeof(char), 1);
	result[1] = ft_calloc(sizeof(char), 1);
	if ((par->quote_type == '\"' || par->different) && input[*i] == '$')
	{
		before_env = get_env_var_ex(input, *i);
		env_variable = get_env_var(input, i, l_env);
		result[0] = ft_strjoin(result[0], before_env);
		result[1] = ft_strjoin(result[1], env_variable);
		start = *i;
	}
	else
	{
		result[0] = ft_strjoin(result[0], ft_strndup(&input[*i], 1));
		result[1] = ft_strjoin(result[1], ft_strndup(&input[*i], 1));
		(*i)++;
	}
	return (result);
}

char	**get_quoted_word(char *input, int *i, t_env *l_env, t_p_t_p *par)
{
	char	**result;
	(*i)++;
	while (input[*i] && (input[*i] != par->quote_type || par->different))
		result = get_result(input, i, l_env, par);
	return (result);
}

char **get_quoted_word2(char *input, int *i, t_env *l_env, char quote_type, int different)
{
    char **result;
	char *env_variable;
	char *before_env;

	(*i)++;
	int start = *i;
	result = ft_calloc(sizeof(char *), 2);
	result[0] = ft_calloc(sizeof(char), 1);
	result[1] = ft_calloc(sizeof(char), 1);
	while(input[*i] && (input[*i] != quote_type || different))
    {
		if ((quote_type == '\"' || different) && input[*i] == '$')
		{
			before_env = get_env_var_ex(input, *i);
        	env_variable = get_env_var(input, i, l_env);
			result[0] = ft_strjoin(result[0], before_env);
			result[1] = ft_strjoin(result[1], env_variable);
			start = *i;
		}
		else
		{
			result[0] = ft_strjoin(result[0], ft_strndup(&input[*i], 1));
			result[1] = ft_strjoin(result[1], ft_strndup(&input[*i], 1));
			(*i)++;
		}
    }
    return (result);
}


char	*get_word(char *input, int *i)
{
	int		start;
	char	*word;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i]) && !is_operator(input[*i])
		&& input[*i] != '\"' && input[*i] != '\'' && input[*i] != '$')
		(*i)++;
	word = ft_strndup(&input[start], *i - start);
	return (word);
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

t_make_env	*create_m_e(void)
{
	t_make_env	*m_e;

	m_e = (t_make_env *)malloc(sizeof(t_make_env));
	if (m_e == NULL)
		return (NULL);
	m_e->dollar = 0;
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
		m_e->dollar++;
	}
	m_e->start = *i;
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
	if (m_e->end == m_e->start - m_e->dollar)
		return (-1);
	m_e->delim = ft_substr(&input[m_e->non_delim_index],
			0, m_e->end - m_e->non_delim_index);
	m_e->var_name = ft_strndup(&input[m_e->start],
			m_e->non_delim_index - m_e->start);
	m_e->var_value = find_env_var(m_e->var_name, l_env);
	return (1);
}

char	*no_var_value(char *input, t_make_env *m_e)
{
	if (m_e->non_delim_index - m_e->start)
	{
		if ((!m_e->end - m_e->non_delim_index))
			return (ft_strndup(&input[m_e->start - m_e->dollar], m_e->dollar));
		else
			return (ft_strjoin(ft_strndup(&input[m_e->start - m_e->dollar],
						m_e->dollar), m_e->delim));
	}
	else
		return (ft_strjoin(ft_strndup(&input[m_e->start - m_e->dollar - 1],
					m_e->dollar + 1), m_e->delim));
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
			return (ft_strndup(&input[m_e->start - m_e->dollar - 1],
					m_e->dollar + 1));
	}
	if (m_e->var_value == NULL)
		return (no_var_value(input, m_e));
	m_e->result = ft_calloc(sizeof(char),
			ft_strlen(m_e->var_value) + 1 + ft_strlen(m_e->delim) + m_e->dollar);
	ft_strcat(m_e->result,
		ft_strndup(&input[m_e->start - m_e->dollar], m_e->dollar));
	ft_strcat(m_e->result, m_e->var_value);
	ft_strcat(m_e->result, m_e->delim);
	result = ft_strdup(m_e->result);
	return (m_e->result);
}

char	*find_env_var(const char *var_name, t_env *l_env)
{
	size_t	len;
	t_env	*current;

	len = ft_strlen(var_name);
	current = l_env;
	while (current)
		current = current->next;
	while (l_env)
	{
		if (ft_strncmp(l_env->key, var_name, len)
			== 0 && l_env->key[len] == '\0')
			return (l_env->value);
		l_env = l_env->next;
	}
	return (NULL);
}
