/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolute.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtunderm <dtunderm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 16:05:04 by dtunderm          #+#    #+#             */
/*   Updated: 2023/12/16 11:49:39 by dtunderm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_path_env(t_env *l_env)
{
	while (l_env)
	{
		if (ft_strcmp(l_env->key, "PATH") == 0)
			return (l_env->value);
		l_env = l_env->next;
	}
	return (NULL);
}

char	*duplicate_path_env(char *path_env)
{
	if (path_env != NULL)
		return (ft_strdup(path_env));
	else
		return (NULL);
}

char	*search_executable_in_path(char *path_env_copy, char *command)
{
	char	*path;
	char	*full_path;

	path = ft_strtok_c(path_env_copy, ':');
	while (path != NULL)
	{
		full_path = ft_calloc(ft_strlen(path) + ft_strlen(command) + 2, 1);
		if (full_path == NULL)
			return (NULL);
		ft_strcpy(full_path, path);
		ft_strcat(full_path, "/");
		ft_strcat(full_path, command);
		if (access(full_path, F_OK) == 0)
			return (full_path);
		path = ft_strtok_c(NULL, ':');
	}
	return (NULL);
}

char	*find_executable(char *command, t_env *l_env)
{
	char	*path_env_copy;
	char	*path_env;

	if (is_in(command, '/') || !_ft_strcmp(command, "..")
		|| !_ft_strcmp(command, ".") || !_ft_strcmp(command, ""))
		return (NULL);
	path_env = get_path_env(l_env);
	if (path_env == NULL)
		return (NULL);
	path_env_copy = duplicate_path_env(path_env);
	if (path_env_copy == NULL)
		return (NULL);
	return (search_executable_in_path(path_env_copy, command));
}

void	update_node_type_and_path(t_node *current, char *executable_path)
{
	current->type = CMD;
	current->path = executable_path;
}
