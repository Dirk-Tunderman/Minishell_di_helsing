#include "../minishell.h"

int	checker_quotes(char *line)
{
	int	i;
    int complete;

	i = 0;
    complete = 1;
	while (line[i])
    {
        if (line[i] == '\'')
        {
            i++;
            while (line[i] && line[i] != '\'')
                i++;
            if (!line[i])
                complete = 0;
        }
        else if (line[i] == '"')
        {
            i++;
            while (line[i] && line[i] != '"')
                i++;
            if (!line[i])
                complete = 0;
        }
        if (line[i])
            i++;
    }
    if (!complete)
        return (printf("Quoting error!\n"), 0);
    return (1);
}

void    display_list(t_node *head)
{
    t_node *current = head;
    while (current != NULL)
    {
        printf("Data: %s, Type: %d, redirect: %d, path: %s\n space_after: %d\n, before_env: %s\n", current->data, current->type, current->redirect, current->path, current->space_after, current->before_env);
        current = current->next;
    }
    printf("--------------------------------------------------------------------------------\n");
}

char *ft_substr(char *s, int start, int len)
{
    char *res;
    int i;
    i = 0;
    if (start < 0)
        start = 0;
    if (len < 0)
        len = 0;
    if (start > ft_strlen(s))
        return (NULL);
    if (len > ft_strlen(s) - start)
        len = ft_strlen(s) - start;
    res = (char *)malloc(len + 1 * sizeof(char));
    if (res == NULL)
        return (NULL);
    while (i < len)
    {
        res[i] = s[start];
        i++;
        start++;
    }
    res[i] = '\0';
    return (res);
}

char *char_to_str(char c)
{
	char *str;

	str = ft_calloc(2 * sizeof(char), 1);
	if (!str)
		return NULL;
	str[0] = c;
	str[1] = '\0';

	return str;
}

int ft_isalnum(char c) 
{
	return (('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'));
}
