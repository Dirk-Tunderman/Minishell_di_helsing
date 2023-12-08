#include "../minishell.h"


int count_quotes(char *line)
{
    int i = 0;
    int count = 0;
    while (line[i])
    {
        if ((line[i] == '\"' || line[i] == '\''))
            count++;
        i++;
    }
    return count;
}

bool check_quotes_correctness(const char *line)
{
    char quote_stack[100];
    int stack_index;
    int i;

    stack_index = -1;
    i = 0;
    while(line[i])
    {
        if (line[i] == '\"' || line[i] == '\'')
        {
            if (stack_index >= 0 && quote_stack[stack_index] == line[i])
                stack_index--;
            else if (stack_index >= 0 && quote_stack[stack_index] != line[i])
            {
                stack_index++;
                quote_stack[stack_index] = line[i];
            }
            else
            {
                stack_index++;
                quote_stack[stack_index] = line[i];
            }
        }
        i++;
    }
    return stack_index == -1;
}

int	checker_quotes(char *line)
{
	int	i;
    int complete;

	i = 0;
    complete = 1;
	while (line[i])
    {
        if (line[i] == '\'')
        {
            i++;
            while (line[i] && line[i] != '\'')
                i++;
            if (!line[i])
                complete = 0;
        }
        else if (line[i] == '"')
        {
            i++;
            while (line[i] && line[i] != '"')
                i++;
            if (!line[i])
                complete = 0;
        }
        if (line[i])
            i++;
    }
    if (!complete)
        return (printf("Quoting error!\n"), 0);
    return (1);
}

void    display_list(t_node *head)
{
    t_node *current = head;
    while (current != NULL)
    {
        printf("Data: %s, Type: %d, redirect: %d, path: %s\n space_after: %d\n, before_env: %s\n", current->data, current->type, current->redirect, current->path, current->space_after, current->before_env);
        current = current->next;
    }
}

void    free_node(t_node *node)
{
    if (node == NULL)
        return ;
    free(node->data);
    free(node);
}

void    free_list(t_node *head)
{
    t_node *next;

    while (head != NULL)
    {
        next = head->next;
        free_node(head);
        head = next;
    }
}
