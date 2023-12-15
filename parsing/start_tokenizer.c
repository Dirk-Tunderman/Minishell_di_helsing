/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_tokenizer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtunderm <dtunderm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 16:03:45 by dtunderm          #+#    #+#             */
/*   Updated: 2023/12/15 19:44:58 by dtunderm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_p_t_p	*create_process_token_params(void)
{
	t_p_t_p	*params;

	params = ft_calloc(sizeof(t_p_t_p), 1);
	if (params == NULL)
		return (NULL);
	params->data = NULL;
	params->quoted = NULL;
	params->split_env = NULL;
	params->quote_type = '\0';
	params->x = 0;
	params->type = 0;
	params->space_flag = 0;
	params->before_env = NULL;
	params->temp = 0;
	params->different = 0;
	return (params);
}

void	append_node(t_node **head, t_p_t_p *params)
{
	t_node	*new_node;
	t_node	*current;

	new_node = ft_calloc(sizeof(t_node), 1);
	new_node->data = ft_strdup(params->data);
	if (params->before_env)
		new_node->before_env = ft_strdup(params->before_env);
	else
		new_node->before_env = NULL;
	new_node->type = params->type;
	new_node->path = NULL;
	new_node->space_after = params->space_flag;
	if (*head == NULL)
		*head = new_node;
	else
	{
		current = *head;
		while (current->next != NULL)
			current = current->next;
		current->next = new_node;
	}
}

void	process_token(char *input, int *i, t_node **head, t_env *l_env)
{
	t_p_t_p	*par;

	par = create_process_token_params();
	if (input[*i] == '$')
		process_dollar(input, i, l_env, par);
	else if (is_operator(input[*i]))
		process_operator(input, i, par);
	else if (input[*i] == '\"' || input[*i] == '\'')
	{
		par->quote_type = input[*i];
		process_quotes(input, i, l_env, par);
	}
	else
		regular_arg(input, i, par);
	if (par->type == ARG && par->before_env)
	{
		handle_arg_type(head, par);
		return ;
	}
	append_node(head, par);
	free(par);
}
