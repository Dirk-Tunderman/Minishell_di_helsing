#include "../minishell.h"

void split_env_var(char *env_var, char **key, char **value) 
{
    char *pos;

    pos = ft_strchr(env_var, '=');
    if (!pos)
    {
        *key = ft_strdup(env_var);
        *value = NULL;
        return;
    }
    *key = ft_strndup(env_var, pos - env_var);
    *value = ft_strdup(pos + 1); // might be null
}

t_env *new_env_node(char *env_var) 
{
    t_env *node;

    node = ft_calloc(sizeof(t_env), 1);
    split_env_var(env_var, &node->key, &node->value);
    node->next = NULL;
    return node;
}

void append_env_node(t_env **head, char *env_var) 
{
    t_env *newNode;

    newNode = new_env_node(env_var);
    if (!*head)
    {
        *head = newNode;
        return;
    }
    t_env *curr = *head;
    while (curr->next) curr = curr->next;
    curr->next = newNode;
}


t_env *envp_to_linked_list(char **envp) 
{
    t_env *head;
    int i;

    head = NULL;
    i = 0;
    while (envp[i]) 
    {
        append_env_node(&head, envp[i]);
        i++;
    }
    return head;
}