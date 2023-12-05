
#include "../minishell.h"

int ft_isalnum(char c) 
{
    return (('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'));
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



char	*del_substr(char *input, int start, int len)
{
    char	*res;
    int		i;

    i = 0;
    if (start < 0)
        start = 0;
    if (len < 0)
        len = 0;
    if (start > ft_strlen(input))
        return (NULL);
    if (len > ft_strlen(input) - start)
        len = ft_strlen(input) - start;
    res = (char *)malloc(len + 1 * sizeof(char));
    if (res == NULL)
        return (NULL);
    while (i < len)
    {
        res[i] = input[start];
        i++;
        start++;
    }
    res[i] = '\0';
    return (res);
}
