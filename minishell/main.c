#include "minishell.h"

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
        printf("Key: %s, Value: %s\n", current->key, current->value ? current->value : "NULL");
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


void parse_cmd_list(t_cmd_full *cmd_full, t_node *head , t_env *l_env, t_command *cmd_arr, char *input)
{
    //first malloc cmd_full
    //then malloc cmd_arr

    cmd_full = create_cmd_full();
    cmd_full->cmd_count = count_cmd(head);
    printf("cmdcount: %d\n", cmd_full->cmd_count);
    cmd_full->cmd_arr = (t_command *)malloc(sizeof(t_command) * cmd_full->cmd_count);
    if (!cmd_full->cmd_arr)
        return ;
    cmd_full->env = l_env;
    //malloc cmd_arr
    cmd_arr = (t_command *)malloc(sizeof(t_command) * cmd_full->cmd_count);
    if (!cmd_arr)
        return ;

    parse_cmd_arr(cmd_full, input);

    printf("CMD_FULL->cmd_arr[0]: %s\n", cmd_full->cmd_arr->single_cmd[0]);
    printf("CMD_FULL->cmd_count: %d\n", cmd_full->cmd_count);



}

int main_loop(t_node *head, char **envp)
{
    char    *input;
    int     error;
    int exit_status;
    t_env *l_env = NULL;
    t_cmd_full *cmd_full = NULL;
    t_command *cmd_arr = NULL;

    l_env = envp_to_linked_list(envp);
    print_env_list(l_env);
    exit_status = 0;
    while (1)
    {
        input = readline("minishell$: ");
        if (input && strlen(input)) // add history for readline
            add_history(input);
        if (!checker_quotes(input)) //error_handeling.c
        {
            printf("error\n");
            free(input);
            continue; // go to next iteration instead of terminating the program
        }

        lexer(input, &head, envp); // pass address of head and envp

        error = error_all_check(head, input);
        if (error)
        {
            printf("error\n");
            free_list(head);
            head = NULL; // Reset head
            free(input);
            continue;
        }
        set_redirect_in_nodes(head);
        check_redirect(head);
        check_redirect_pipe(head);

        resolve_commands(head, envp);
        resolve_path(head, envp);

  

        // Initialize the t_cmd list and transfer the data
        // parse_cmd_list(cmd_full, head, l_env, cmd_arr);
        // transfer_node_to_cmd(head,  cmd_head);

        // Continue processing the commands using cmd_head

        printf("\n");
        display_list(head);
        printf("\n");
        // cmd_full = init_cmd_full();
        //parse_cmd_list(cmd_head, cmd_full);

        parse_cmd_list(cmd_full, head, l_env, cmd_arr, input);
        printf("input; %s\n", input);

        //display_cmd_list(cmd_head);
        t_comparsed *parsed = parsed_single_cmd(head, cmd_full->cmd_count, cmd_full->exit_status, envp);
        execute_list(parsed, head, l_env);
        exit_status = parsed->exit_status;
        free(input);
        free_list(head); // You need to implement this function to free the linked list
        head = NULL; // Reset head
        // free_cmd_list(cmd_head); // You should implement this function to free the t_cmd linked list
    }
    return (0);
}


int     main(int argc, char **argv, char **envp)
{
    t_node *head = NULL;
    (void)argc;
    (void)argv;
    
    return (main_loop(head, envp)); // Add envp to main_loop
}

