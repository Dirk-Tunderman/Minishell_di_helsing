/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtunderm <dtunderm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 15:57:11 by dtunderm          #+#    #+#             */
/*   Updated: 2023/12/16 10:31:34 by dtunderm         ###   ########.fr       */
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

void	process_operator(char *input, int *i, t_p_t_p *params)
{
	if (input[*i] == '>' && input[*i + 1] == '>')
	{
		params->data = ft_strdup(">>");
		params->type = APPEND;
		(*i)++;
	}
	else if (input[*i] == '<' && input[*i + 1] == '<')
	{
		params->data = ft_strdup("<<");
		params->type = HERDOC;
		(*i)++;
	}
	else
	{
		if (input[*i] == '<')
			params->type = STDIN_RD;
		else if (input[*i] == '>')
			params->type = STDOUT_RD;
		else
			params->type = OPERATOR;
		params->data = char_to_str(input[*i]);
	}
	(*i)++;
	params->space_flag = ft_isspace(input[*i]);
}

void	process_quotes(char *input, int *i, t_env *l_env, t_p_t_p *params)
{
	params->quoted = get_quoted_word2(input, i, l_env, input[*i], 0);
	params->data = params->quoted[1];
	params->before_env = params->quoted[0];
	if (input[*i])
		(*i)++;
	params->space_flag = ft_isspace(input[*i]);
	params->type = QUOTE_ARG;
}

void	handle_arg_type(t_node **head, t_p_t_p *params)
{
	if (params->before_env)
	{
		params->split_env = ft_split(params->before_env);
		params->x = 0;
		while (params->split_env[params->x])
		{
			if (!params->split_env[params->x + 1])
				append_node(head, params);
			else
				append_node(head, params);
			minishell_no_alzheimer(*head, params->data);
			params->x++;
		}
		if (!params->split_env[0])
		{
			append_node(head, params);
			minishell_no_alzheimer(*head, params->data);
		}
	}
	else
	{
		append_node(head, params);
		minishell_no_alzheimer(*head, params->data);
	}
}

void	regular_arg(char *input, int *i, t_p_t_p *params)
{
	params->data = get_word(input, i);
	params->space_flag = ft_isspace(input[*i]);
	params->type = ARG;
}
