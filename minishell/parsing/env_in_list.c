#include "../minishell.h"

// Split environment variable into key and value
void split_env_var(char *env_var, char **key, char **value) 
{
    char *pos = strchr(env_var, '=');
    if (!pos) {
        *key = strdup(env_var);
        *value = NULL;
        return;
    }

    *key = strndup(env_var, pos - env_var);
    *value = strdup(pos + 1);
}

// Create a new node for the linked list
t_env *new_env_node(char *env_var) 
{
    t_env *node = malloc(sizeof(t_env));
    if (!node) exit(1);  // Bail out if no memory

    split_env_var(env_var, &node->key, &node->value);
    node->next = NULL;
    return node;
}

// Append a new node to the end of the linked list
void append_env_node(t_env **head, char *env_var) 
{
    t_env *newNode = new_env_node(env_var);
    if (!*head) {
        *head = newNode;
        return;
    }

    t_env *curr = *head;
    while (curr->next) curr = curr->next;
    curr->next = newNode;
}

// Convert environment variables to linked list
t_env *envp_to_linked_list(char **envp) 
{
    t_env *head = NULL;
    int i = 0;
    while (envp[i]) {
        append_env_node(&head, envp[i]);
        i++;
    }
    return head;
}