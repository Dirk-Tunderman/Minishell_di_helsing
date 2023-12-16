/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtunderm <dtunderm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 11:49:47 by dtunderm          #+#    #+#             */
/*   Updated: 2023/12/16 11:50:14 by dtunderm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	resolve_path(t_node *head, t_env *l_env)
{
	t_node	*current;
	char	*executable_path;

	current = head;
	while (current != NULL)
	{
		if ((current->type == ARG || current->type == QUOTE_ARG)
			&& current->data)
		{
			executable_path = find_executable(current->data, l_env);
			if (executable_path != NULL)
				update_node_type_and_path(current, executable_path);
		}
		current = current->next;
	}
}
