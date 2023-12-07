#include "../minishell.h"

char *find_executable(char *command, t_env *l_env)
{
    char *path_env;
    char *path;
    //printf("command: %s\n", command);

    path_env = NULL;
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

    path = ft_strtok_c(path_env_copy, ':'); // recode
    //printf("path: %s\n", path);
    while (path != NULL)
    {
        printf("THIS IS : %s\n", path);
        char *full_path = malloc(ft_strlen(path) + ft_strlen(command) + 2);
        if (full_path == NULL)
            return (free(path_env_copy), NULL);

        // Construct possible full path
        ft_strcpy(full_path, path);

        ft_strcat(full_path, "/");

        ft_strcat(full_path, command);

        //printf("two full_path: %s\n", full_path);
        if (access(full_path, X_OK) == 0)
        {
            printf("full_path: %s\n", full_path);
            return (free(path_env_copy), full_path);
        }
        free(full_path);
        path = ft_strtok_c(NULL, ':'); // recode
    }
    return (free(path_env_copy), NULL);
}

void resolve_path(t_node *head, t_env *l_env)
{
    char *executable_path;

    t_node *current = head;
    while (current != NULL)
    {
        if (current->type == ARG || current->type == QUOTE_ARG)
        {
            executable_path = find_executable(current->data, l_env);
            if (executable_path != NULL && current->data[0])
            {
                current->type = CMD;
                current->path = executable_path;
            }
        }
        current = current->next;
    }
}
