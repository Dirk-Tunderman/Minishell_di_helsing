/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handeling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtunderm <dtunderm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 15:37:46 by dtunderm          #+#    #+#             */
/*   Updated: 2023/12/15 15:56:32 by dtunderm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	error_all_check(t_node *head)
{
	int	error;

	if (!head)
	{
		exit_status(0);
		return (-1);
	}
	error = check_operator_errors(head);
	if (error)
	{
		exit_status(255);
		return (-1);
	}
	error = check_redirection_errors(head);
	if (error)
	{
		exit_status(255);
		return (-1);
	}
	return (0);
}

int	check_operator_errors(t_node *head)
{
	t_node	*current;
	t_node	*previous;
	int		error;

	current = head;
	previous = NULL;
	while (current != NULL)
	{
		error = check_operator_start_end(current, previous);
		if (error)
			return (-1);
		previous = current;
		current = current->next;
	}
	return (0);
}

int	check_operator_start_end(t_node *current, t_node *previous)
{
	if (current->type == OPERATOR && ((!previous || (previous->type != ARG
					&& previous->type != QUOTE_ARG && previous->type != CMD))
			|| (current->next == NULL)))
	{
		printf("Pipe err: Operator '|' can't be at start or end of command.\n");
		return (-1);
	}
	return (0);
}

int	check_redirection_errors(t_node *head)
{
	t_node	*current;

	current = head;
	while (current != NULL)
	{
		if (current->redirect == 1 && ((current->next
					&& (current->next->redirect == 1
						|| current->next->type == OPERATOR))
				|| current->next == NULL))
		{
			printf("Syntax err, a redirector can't"
				"be after another or by itself\n");
			return (-1);
		}
		current = current->next;
	}
	return (0);
}

void	set_redirect_in_nodes(t_node *head)
{
	t_node	*current;

	current = head;
	while (current != NULL)
	{
		if ((ft_strcmp(current->data, "<<") == 0
				|| ft_strcmp(current->data, ">>") == 0
				|| ft_strcmp(current->data, "<") == 0
				|| ft_strcmp(current->data, ">") == 0)
			&& current->type != QUOTE_ARG
			&& current->type != ARG && current->type != CMD)
			current->redirect = 1;
		else
			current->redirect = 0;
		current = current->next;
	}
}
