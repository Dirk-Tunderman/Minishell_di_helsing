#include "minishell.h"

void	resolve_path_and_commands(t_node *head, t_env *l_env)
{
    resolve_path(head, l_env);
}

int exit_status(int value)
{
    static int status;

    if (value > 1000)
        return (status);
    else
        status = value;
    return (0);
}

void print_envp(char **envp) {
    int i = 0;
    while (envp[i]) {
        printf("%s\n", envp[i]);
        i++;
    }
}

void print_env_list(t_env *head) {
    t_env *current = head;
    while (current) {
        //printf("%s, Value: %s\n", current->key, current->value ? current->value : "NULL");
        current = current->next;
    }
}

int count_cmd(t_node *head)
{
    int count = 1;
    while (head)
    {
        if (head->type == OPERATOR)
            count++;
        head = head->next;
    }
    return (count);
}


void    check_here_doc_before_exp(t_node *head)
{
    t_node *current = head;
    while (current)
    {
        if (!ft_strcmp(current->data, "<<") && current->next->before_env != NULL)
            current->next->data = current->next->before_env;
        current = current->next;
    }
}



//--------------------------------------------------------------------------------------//





int main_loop(t_node *head, char **envp)
{
	char    *input;
	static t_env *l_env;
    int     exit_status;

    (void) exit_status; 
    input = "";
    if (!l_env)
    	l_env = envp_to_linked_list(envp);
    
    //print_env_list(l_env);
	while (1)
	{
        if(!input)
            printf("\n");
        input = readline("minishell$: ");
		if (input && ft_strlen(input) && (add_history(input) || 1) && checker_quotes(input))
        {
            printf("input: %s\n", input);
		    lexer(input, &head, l_env);
        	set_redirect_in_nodes(head);
		    if (error_all_check(head))
		    {
		    	head = NULL; // leak !!!!
		    	free(input);
		    	continue;
		    }
		    resolve_path_and_commands(head, l_env);

            check_here_doc_before_exp(head);
            printf("\ndisplaylist:\n\n");
		    display_list(head);
		    printf("\n");
		    printf("input; %s\n", input);
            ///--------------------------------------//
          //  t_comparsed *parsed = parsed_single_cmd(head, count_cmd(head), exit_status, envp);
          //  execute_list(parsed, l_env, envp, head);
          //  exit_status = parsed->exit_status;
		    ///--------------------------------------//
            free(input);
		    free_list(head);
		    head = NULL;
        }
	}
	return (0);
}


int     main(int argc, char **argv, char **envp)
{
	t_node *head;
	(void)argc;
	(void)argv;
	
    head = NULL;
   // sig_init();
	return (main_loop(head, envp)); // Add envp to main_loop
}

