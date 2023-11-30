#include "minishell.h"

void resolve_commands(t_node *head, char **envp) 
{
    t_node *current = head;

    while (current != NULL) 
    {
        if (current->type == 1 && is_command(current->data)) 
        {
            char *executable_path = find_executable(current->data, envp);
            if (executable_path != NULL) 
            {
                current->type = 4;
                current->path = executable_path;  // Set path in node
            } else 
            {
                // Command is not found, set path to NULL
                current->path = NULL;
            }
        }

        current = current->next;
    }
}

char *find_executable(char *command, char **envp) 
{
    char *path_env = NULL;
    // Find the PATH variable in envp
    for (int i = 0; envp[i] != NULL; i++) 
    {
        if (strncmp(envp[i], "PATH=", 5) == 0) 
        {
            path_env = envp[i] + 5;  // Skip the "PATH=" part
            break;
        }
    }

    if (path_env == NULL) {
        // No PATH variable found
        return NULL;
    }

    char *path_env_copy = strdup(path_env);  // We need a copy because strtok modifies the string
    if (path_env_copy == NULL) 
    {
        // Handle strdup failure (e.g., by returning NULL or exiting)
        return NULL;
    }

    char *path = strtok(path_env_copy, ":");  // PATH entries are separated by ":"

    while (path != NULL) 
    {
        // Allocate memory for the possible full path
        char *full_path = malloc(strlen(path) + strlen(command) + 2);
        if (full_path == NULL) 
        {
            // Handle malloc failure (e.g., by returning NULL or exiting)
            free(path_env_copy);
            return NULL;
        }

        // Construct possible full path
        strcpy(full_path, path);
        strcat(full_path, "/");
        strcat(full_path, command);

        if (access(full_path, X_OK) == 0) 
        {
            // Found the command in this directory
            free(path_env_copy);
            return full_path;
        }

        // Not found in this directory, free the allocated memory and try next
        free(full_path);
        path = strtok(NULL, ":");
    }

    // Command not found in any PATH directory
    free(path_env_copy);
    return NULL;
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
                current->path = executable_path;  // Set path in node
            } else 
            {
                // Handle case where command is not found (e.g., set path to an error string)
            }
        }

        current = current->next;
    }
}

