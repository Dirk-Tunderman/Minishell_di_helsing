#include "../minishell.h"

void resolve_commands(t_node *head, t_env *l_env)
{
    t_node *current = head;
    while (current != NULL)
    {
        if (current->type == CMD && is_command(current->data))
        {
            char *executable_path = find_executable(current->data, l_env);
            //printf("two executable_path: %s\n", executable_path);
            if (executable_path != NULL)
            {
                current->path = executable_path;
            }
        }
        current = current->next;
    }
}
// {
//     t_node *current;

//     current = head;
//     while (current != NULL) 
//     {
//         if (current->type == 1 && is_command(current->data)) 
//         {
//             char *executable_path = find_executable(current->data, envp);
//             if (executable_path != NULL) 
//             {
//                 current->type = 4;
//                 current->path = executable_path;
//             } 
//             else 
//                 current->path = NULL;
//         }
//         current = current->next;
//     }
// }

char *find_executable(char *command, t_env *l_env)
{
    char *path_env = NULL;
    char *path;
    //printf("command: %s\n", command);
    while (l_env)
    {
        if (ft_strcmp(l_env->key, "PATH") == 0)
        {
            //printf("path_env: %s\n", l_env->value);
            path_env = l_env->value;
            break;
        }
        l_env = l_env->next;
    }
 //   printf("%s\n", path_env);//
    if (path_env == NULL)
        return NULL;

    char *path_env_copy = ft_strdup(path_env);
    if (path_env_copy == NULL)
        return (NULL);

    path = strtok(path_env_copy, ":"); // recode
    //printf("path: %s\n", path);
    while (path != NULL)
    {
        char *full_path = malloc(ft_strlen(path) + ft_strlen(command) + 2);
        if (full_path == NULL)
            return (free(path_env_copy), NULL);

        // Construct possible full path
        strcpy(full_path, path);

        strcat(full_path, "/");

        strcat(full_path, command);

        //printf("two full_path: %s\n", full_path);
        if (access(full_path, X_OK) == 0)
        {
            printf("full_path: %s\n", full_path);
            return (free(path_env_copy), full_path);
        }
        free(full_path);
        path = strtok(NULL, ":"); // recode
    }
    return (free(path_env_copy), NULL);
}
// {
//     char *path_env = NULL;
//     char *path;

//     for (int i = 0; envp[i] != NULL; i++) 
//     {
//         if (ft_strncmp(envp[i], "PATH=", 5) == 0) 
//         {
//             path_env = envp[i] + 5;
//             break;
//         }
//     }
//     if (path_env == NULL)
//         return NULL;

//     char *path_env_copy = ft_strdup(path_env);
//     if (path_env_copy == NULL) 
//         return (NULL);

//     path = ft_strtok(path_env_copy, ":");
//     while (path != NULL) 
//     {
//         char *full_path = malloc(strlen(path) + strlen(command) + 2);
//         if (full_path == NULL) 
//         {
//             return (free(path_env_copy), NULL);
//         }
//         ft_strcpy(full_path, path);
//         ft_strcat(full_path, "/");
//         ft_strcat(full_path, command);
//         if (access(full_path, X_OK) == 0) 
//             return (free(path_env_copy), full_path);

//         free(full_path);
//         path = strtok(NULL, ":");
//     }
//     return (free(path_env_copy), NULL);
// }

void resolve_path(t_node *head, t_env *l_env)
{
    t_node *current = head;
    while (current != NULL)
    {
        if (current->type == ARG && is_command(current->data))
        {
            //printf("current->data: %s\n is a command\n", current->data);
            char *executable_path = find_executable(current->data, l_env);
            //printf("executable_path: %s\n", executable_path);
            if (executable_path != NULL)
            {
                current->type = 4;
                current->path = executable_path;
            }
        }
        current = current->next;
    }
}
// {
//     t_node *current = head;

//     while (current != NULL) 
//     {
//         if (current->type == CMD && is_command(current->data)) 
//         {
//             char *executable_path = find_executable(current->data, envp);
//             if (executable_path != NULL) 
//             {
//                 current->path = executable_path;
//             }
//         }
//         current = current->next;
//     }
// }

