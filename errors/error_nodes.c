#include "../minishell.h"

void     set_redirect_in_nodes(t_node *head) 
{
    t_node *current;

    current = head;
    while (current != NULL)
    {
            if ((ft_strcmp(current->data, "<<") == 0 ||
                ft_strcmp(current->data, ">>") == 0 ||
                ft_strcmp(current->data, "<") == 0 ||
                ft_strcmp(current->data, ">") == 0) && (current->type != QUOTE_ARG))
                    current->redirect = 1;
            else 
                current->redirect = 0;
        current = current->next;
    }
}
