#include "../minishell.h"

int error_all_check(t_node *head, char *input)
{
    int error;

    // Check for errors related to operators
    error = check_operator_errors(head);
    if (error)
        return (-1);

    // Check for errors related to redirection operators
    error = check_redirection_errors(head);
    if (error)
        return (-1);

    // Check for errors related to filenames
    error = check_filename_errors(head);
    if (error)
        return (-1);

    // Check for errors related to pipe operator
    error = check_pipe_errors(head);
    if (error)
        return (-1);

    // Check if the last character of the input is an operator
    char last_char = input[strlen(input) - 1];
    if (is_operator(last_char))
    {
        printf("Error: Operator '%c' cannot be at the end of a command.\n", last_char);
        return (-1);
    }

    // If no errors were found, return 0
    return (0);
}

int	check_operator_errors(t_node *head)
{
	t_node	*current;
	t_node	*previous;
	t_node	*next;
	int		error;

	current = head;
	previous = NULL;
	while (current != NULL)
	{
		next = current->next;
		error = check_operator_start_end(current, previous, next);
		if (error)
			return (-1);
		error = check_operator_no_cmd(current, previous, next);
		if (error)
			return (-1);
		error = check_redirect_pipe_error(current, previous);
		if (error)
			return (-1);
		previous = current;
		current = next;
	}
	return (0);
}

int	check_operator_start_end(t_node *current, t_node *previous, t_node *next)
{
	if (current->type == OPERATOR && (previous == NULL || next == NULL))
	{
		if ((previous == NULL &&
		(ft_strcmp(current->data, ">") == 0 || ft_strcmp(current->data, "<") == 0 ||
		ft_strcmp(current->data, ">>") == 0)) ||
		(next == NULL && previous && ft_strcmp(previous->data, "|") == 0))
			return (0);
		printf("Error: Operator '%s' cannot be at the start or end of a command.\n",
		current->data);
		return (-1);
	}
	return (0);
}


int	check_operator_no_cmd(t_node *current, t_node *previous, t_node *next)
{
	if (current->type == OPERATOR) 
	{
        // First, we check if both 'previous' and 'next' nodes exist
        if (previous && next)
        {
            // If 'previous' and 'next' are both operators, then we have three operators in a row
            if (previous->type == OPERATOR && next->type == OPERATOR)
            {
                printf("Error: Operator '%s' cannot be next to two other operators.\n",
                current->data);
                return (-1);
            }
        }
	}
	return (0);
}

int	check_redirect_pipe_error(t_node *current, t_node *previous)
{
	if (current->type == OPERATOR && previous != NULL && previous->type == OPERATOR &&
	(ft_strcmp(current->data, "|") == 0 && (ft_strcmp(previous->data, ">") == 0 ||
	ft_strcmp(previous->data, ">>") == 0)))
	{
		printf("Error: Pipe operator and a redirection operator for the same output.\n");
		return (-1);
	}
	return (0);
}

int check_redirection_errors(t_node *head)
{
	t_node *current = head;
	while (current != NULL)
	{
		// Check if current node's data is '<<' or '>>'
		if (current->redirect == 1 && current->next->redirect == 1 && current->next->redirect == 1)
		{
			printf("3 redirections in a row a sahbi\n");
			return -1;
		}

		current = current->next;
	}
	return (0);
}

int check_filename_errors(t_node *head) 
{
    t_node *current = head;
    t_node *previous = NULL;
    char invalid_chars[] = "/:*?\"<>|";

    while (current != NULL) {
        // Check if previous node is a redirection operator
        if (previous != NULL && 
            (previous->type == OPERATOR) && 
            ((strcmp(previous->data, "<") == 0) || (strcmp(previous->data, ">") == 0) || (strcmp(previous->data, ">>") == 0) || (strcmp(previous->data, "<<") == 0)))
        {
            if (current->type != ARG) 
			{
				//printf("current->data: %s\n", current->data);
				if (current->previous == NULL)
					printf("NULL\n");
				// printf("current->prev->data: %s\n", current->next->data);

                printf("check_filename Error: Redirection operator '%s' must be followed by a filename.\n", previous->data);
                return -1;
            }
            else
            {
                if (check_invalid_chars(current->data, invalid_chars) == -1)
                {
                    return -1;
                }
            }
        }
        previous = current;
        current = current->next;
    }
    return 0;
}

int check_pipe_errors(t_node *head)
{
    t_node *current = head;
    t_node *previous = NULL;

    while (current != NULL)
    {
        if (current->type == OPERATOR && strcmp(current->data, "|") == 0)
        {
            if (previous == NULL || current->next == NULL || current->next->type == OPERATOR)
            {
                printf("Error: Pipe operator '|' cannot be at the start or end of a command, or next to another operator.\n");
                return -1;
            }
        }
        previous = current;
        current = current->next;
    }
    return 0;
}

int	check_invalid_chars(char *data, char *invalid_chars)
{
	int	i;

	i = 0;
	while (invalid_chars[i])
	{
		if (strchr(data, invalid_chars[i]) != NULL)
		{
			printf("halllo Error: Filename '%s' contains invalid character '%c'.\n",
			data, invalid_chars[i]);
			return (-1);
		}
		i++;
	}
	return (0);
}


