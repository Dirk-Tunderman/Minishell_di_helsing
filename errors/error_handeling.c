#include "../minishell.h"

int error_all_check(t_node *head)
{
    int error;

	if (!head)
		return (-1);
    error = check_operator_errors(head); /// PIPE
    if (error)
        return (-1);
    error = check_redirection_errors(head); // < > >> >>
    if (error)
        return (-1);
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
		&& previous->type != QUOTE_ARG && previous->type != CMD && previous->type != BUILT_IN)) || (current->next == NULL)))
	{
		printf("Pipe error: Operator '|' cannot be at the start or end of a command.\n");
		return (-1);
	}
	return (0);
}


int check_redirection_errors(t_node *head)
{
	t_node *current = head;
	while (current != NULL)
	{
		if (current->redirect == 1 && ((current->next
			&& (current->next->redirect == 1 || current->next->type == OPERATOR)) || current->next == NULL))
		{
			printf("Syntax error, a redirector cannot appear after another or by itself!\n");
			return (-1);
		}
		current = current->next;
	}
	return (0);
}



