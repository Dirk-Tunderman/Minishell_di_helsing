#include "../minishell.h"

char *find_executable(char *command, t_env *l_env)
{
    char *path_env;
    char *path;
    //printf("command: %s\n", command);

    if (is_in(command, '/') || !_ft_strcmp(command, "..") || !_ft_strcmp(command, ".") || !_ft_strcmp(command, ""))// no search if already path component
        return (NULL);
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
        char *full_path = ft_calloc(ft_strlen(path) + ft_strlen(command) + 2, 1);
        if (full_path == NULL)
            return (NULL);

        // Construct possible full path
        ft_strcpy(full_path, path);

        ft_strcat(full_path, "/");

        ft_strcat(full_path, command);

        //printf("two full_path: %s\n", full_path);
        if (access(full_path, F_OK) == 0)
            return (full_path);
        path = ft_strtok_c(NULL, ':'); // recode
    }
    return (NULL);
}

int is_command(char *cmd)
{
    int x;

    x = 0;
    if (!cmd)
        return (0);
    while (cmd[x] && cmd[x] != '/')
        x++;
    if (cmd[x])
        return (0);
    if ((!cmd[0] || (cmd[0] ==  '.' && !cmd[1]) || (cmd[0] == '.' && cmd[1] == '.' && !cmd[2])))
        return (0);
    return (1);
}

void resolve_path(t_node *head, t_env *l_env)
{
    char *executable_path;

    t_node *current = head;
    while (current != NULL)
    {
        if ((current->type == ARG || current->type == QUOTE_ARG) && current->data)
        {
            executable_path = find_executable(current->data, l_env);
            if (executable_path != NULL && is_command(current->data))
            {
                current->type = CMD;
                current->path = executable_path;
            }
        }
        current = current->next;
    }
}
