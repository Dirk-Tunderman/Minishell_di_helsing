#include "../minishell.h"

void set_redirect_in_nodes(t_node *head) {
    t_node *current = head;
    while (current != NULL) {

            if (strcmp(current->data, "<<") == 0 ||
                strcmp(current->data, ">>") == 0 ||
                strcmp(current->data, "<") == 0 ||
                strcmp(current->data, ">") == 0) 
                {
                    current->redirect = 1;
            }
            else 
                current->redirect = 0;
        
        current = current->next;
    }
}

int check_redirect(t_node *head) 
{

    while (head->next != NULL)
    {
        //printf("data: %s, path: %s, flag: %d\n\n", head->data, head->path, head->flag);
        head = head->next;
    }
    return 0;
//     t_node *current = head;
//    // t_node *prev = NULL;

//     while (current != NULL)
//     {
//         // Check if current node is a redirection
//         if (current->type == 6 || current->type == 7 || current->type == 8 || current->type == 9) {
//             // Check if previous node is also a redirection
//             if ((current->next->type == 6 || current->next->type == 7 || current->next->type == 8 || current->next->type == 9)) 
//             {
//                 // Check for invalid combinations
//                     printf("Error: Invalid combination of redirection operators.\n");
//                     return -1;
            
//             }
//             prev = current;
//         } else {
//             // Reset prev if the current node is not a redirection
//             prev = NULL;
//         }
//         current = current->next;
//     }

//     // If we made it through the whole list without returning, then the redirections are fine
//     return 0;
}

int check_redirect_pipe(t_node *head) 
{
    t_node *current = head;

    while (current != NULL)
    {
        // Check if current node is a redirection
        if ((current->redirect == 1) && (current->next->type == OPERATOR))
        {
            printf("Error: Redirection and pipe after not possible.\n");
            return -1;
        }
        else if (current->type == OPERATOR && current->next->type == OPERATOR && current->next->next->type == 1)
        {
            printf("Error: Redirection and pipe after not possible.\n");
            return -1;
            
        }
        current = current->next;
    }

    // If we made it through the whole list without returning, then the redirections are fine
    return 0;
}

void    display_excute_list(t_excute *head)
{
    t_excute *current = head;
    puts("jaaaaa");
    if (current == NULL)
        printf("je moeder");
    while (current != NULL)
    {
        printf("%s\n", current->line);
        current = current->next;
    }
}