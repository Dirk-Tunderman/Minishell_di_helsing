#include "minishell.h"

int	set_and_check_redirect(t_node *head)
{

	set_redirect_in_nodes(head);

	if (check_redirect(head) == -1)
        return (-1);
	if (check_redirect_pipe(head) == -1)
        return (-1);
    return (1);
}

void	resolve_path_and_commands(t_node *head, t_env *l_env)
{
    resolve_path(head, l_env);
    resolve_commands(head, l_env);
}


//--------------------------------------------------------------------------------------//




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


t_cmd_full *create_cmd_full() {
    // Allocate memory for t_cmd_full
    t_cmd_full *cmd_full = (t_cmd_full *)malloc(sizeof(t_cmd_full));
    if (!cmd_full) {
        // If malloc fails, return NULL
        return NULL;
    }

    // Initialize fields
    cmd_full->cmd_arr = NULL; // Assuming cmd_arr should be initialized to NULL
    cmd_full->env = NULL;   // Assuming env should be initialized to NULL
    cmd_full->cmd_count = 0; // Assuming cmd_count should be initialized to 0
    // You might want to allocate memory for cmd_arr, head, and env here
    // depending on what they are and how they should be initialized

    return cmd_full;
}

void parse_cmd_arr(t_cmd_full *cmd_full, char *input)
{
    int i = 0, j = 0;
    char *token;
    const char delim[2] = "|";

    // Count the number of commands
    int cmd_count = 1;
    while (input[i]) {
        if (input[i] == '|') {
            cmd_count++;
        }
        i++;
    }

    // Allocate memory for cmd_arr including one extra for the null termination
    cmd_full->cmd_arr->single_cmd = (char **)malloc(sizeof(char *) * (cmd_count + 1));
    if (!cmd_full->cmd_arr->single_cmd) {
        perror("Failed to allocate memory for single_cmd");
        return;
    }

    // Tokenize and store the commands
    i = 0;
    token = strtok(input, delim);
    while (token != NULL) {
        cmd_full->cmd_arr->single_cmd[i] = malloc(strlen(token) + 1);
        if (!cmd_full->cmd_arr->single_cmd[i]) {
            perror("Failed to allocate memory for a single command");
            // Free the already allocated memory
            while (i > 0) {
                free(cmd_full->cmd_arr->single_cmd[--i]);
            }
            free(cmd_full->cmd_arr->single_cmd);
            return;
        }
        strcpy(cmd_full->cmd_arr->single_cmd[i], token);
        token = strtok(NULL, delim);
        i++;
    }

    // Null-terminate the array of commands
    cmd_full->cmd_arr->single_cmd[i] = NULL;

    // Print the commands
    j = 0;
    while (cmd_full->cmd_arr->single_cmd[j]) {
        printf("cmd_arr[%d]: %s\n", j, cmd_full->cmd_arr->single_cmd[j]);
        j++;
    }
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


void parse_cmd_list(t_cmd_full **cmd_full, t_node *head , t_env *l_env, t_command *cmd_arr, char *input)
{
    //first malloc cmd_full
    //then malloc cmd_arr

    (*cmd_full) = create_cmd_full();
     (*cmd_full)->cmd_count = count_cmd(head);
    printf("cmdcount: %d\n",  (*cmd_full)->cmd_count);
     (*cmd_full)->cmd_arr = (t_command *)malloc(sizeof(t_command) *  (*cmd_full)->cmd_count);
    if (! (*cmd_full)->cmd_arr)
        return ;
     (*cmd_full)->env = l_env;
    //malloc cmd_arr
    cmd_arr = (t_command *)malloc(sizeof(t_command) *  (*cmd_full)->cmd_count);
    if (!cmd_arr)
        return ;

    parse_cmd_arr(*cmd_full, input);

    printf("CMD_FULL->cmd_arr[0]: %s\n",  (*cmd_full)->cmd_arr->single_cmd[0]);
    printf("CMD_FULL->cmd_count: %d\n",  (*cmd_full)->cmd_count);

}











//--------------------------------------------------------------------------------------//


int main_loop(t_node *head, char **envp)
{
	char    *input;
	int     error;
	t_env *l_env;
    
    
    ///--------------------------------------//
    // int exit_status;
    // //int i = 0;

    // // while (envp[i])
    // // {
    // //     printf("envp[%d]: %s\n", i, envp[i]);
    // //     i++;
    // // }
    // t_cmd_full *cmd_full = NULL;
    // t_command *cmd_arr = NULL;
    // exit_status = 0;
	///--------------------------------------//
    
    input = "";
    l_env = NULL;
	l_env = envp_to_linked_list(envp);
    //print_env_list(l_env);
	while (1)
	{
        sig_init();
        if(!input)
            printf("\n");
        input = readline("minishell$: ");
		if (input && ft_strlen(input))
        {
			add_history(input);
		    if (!checker_quotes(input))
		    {
			    printf("error\n");	
		    	free(input);
		    	continue;
		    }
            printf("input: %s\n", input);
		    lexer(input, &head, l_env);
            error = error_all_check(head, input);
		    if (error)
		    {
		    	printf("error\n");
		    	free_list(head);
		    	head = NULL;
		    	free(input);
		    	continue;
		    }
		    if (set_and_check_redirect(head) == -1)
            {
                printf("error\n");
                free_list(head);
                head = NULL;
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
            // parse_cmd_list(&cmd_full, head, l_env, cmd_arr, input);
            // t_comparsed *parsed = parsed_single_cmd(head, cmd_full->cmd_count, cmd_full->exit_status, envp);
            // execute_list(parsed, l_env, envp, head);
            // exit_status = parsed->exit_status;
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
	return (main_loop(head, envp)); // Add envp to main_loop
}

