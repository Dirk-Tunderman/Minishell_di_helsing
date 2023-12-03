#include "../minishell.h"

void resolve_commands(t_node *head, char **envp) 
{
    t_node *current;

    current = head;
    while (current != NULL) 
    {
        if (current->type == 1 && is_command(current->data)) 
        {
            char *executable_path = find_executable(current->data, envp);
            if (executable_path != NULL) 
            {
                current->type = 4;
                current->path = executable_path;
            } 
            else 
                current->path = NULL;
        }
        current = current->next;
    }
}

char *find_executable(char *command, char **envp) 
{
    char *path_env = NULL;
    char *path;

    for (int i = 0; envp[i] != NULL; i++) 
    {
        if (ft_strncmp(envp[i], "PATH=", 5) == 0) 
        {
            path_env = envp[i] + 5;
            break;
        }
    }
    if (path_env == NULL)
        return NULL;

    char *path_env_copy = ft_strdup(path_env);
    if (path_env_copy == NULL) 
        return (NULL);

    path = ft_strtok(path_env_copy, ":");
    while (path != NULL) 
    {
        char *full_path = malloc(strlen(path) + strlen(command) + 2);
        if (full_path == NULL) 
        {
            free(path_env_copy);
            return (free(path_env_copy), NULL);
        }
        ft_strcpy(full_path, path);
        ft_strcat(full_path, "/");
        ft_strcat(full_path, command);
        if (access(full_path, X_OK) == 0) 
            return (free(path_env_copy), full_path);

        free(full_path);
        path = strtok(NULL, ":");
    }
    return (free(path_env_copy), NULL);
}

void resolve_path(t_node *head, char **envp) 
{
    t_node *current = head;

    while (current != NULL) 
    {
        if (current->type == CMD && is_command(current->data)) 
        {
            char *executable_path = find_executable(current->data, envp);
            if (executable_path != NULL) 
            {
                current->path = executable_path;
            }
        }
        current = current->next;
    }
}

