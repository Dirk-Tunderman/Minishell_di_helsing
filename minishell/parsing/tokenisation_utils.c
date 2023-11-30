#include "../minishell.h"

int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}


char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != (char)c)
		i++;
	if (s[i] == (char) c)
		return ((char *) s + i);
	return (NULL);
}

char	*ft_strdup( const char *source)
{
	char	*p;
	int		i;

	p = (char *)malloc(ft_strlen(source) + 1 * sizeof(char));
	if (p == NULL)
		return (NULL);
	i = 0;
	while (source[i])
	{
		p[i] = source[i];
		i++;
	}
	p[i] = '\0';
	return (p);
}


// void    quote(char *str, t_node **head)
// {
//     int type;
    
//     if (str[0] == '\'')
//         type = S_QUOT;
//     else if (str[0] == '\"')
//         type = D_QUOT;
// 	else if (str[0] == '>')
// 		type = R_SIGNE;
// 	else if (str[0] == '<')
// 		type = L_SIGNE;
// 	// else if (str == '>>')
// 	// 	type = RIGHT_SHIFT;
// 	else if (str[0] == '|')
// 		type = PIPE;
// 	else
// 		type = CMD;
//     add_node(head, str, type);
// }


char	*ft_strtrim(char *s1, char *set)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (!s1 || !set)
		return (NULL);
	while (s1[i])
	{
		if (!ft_strchr(set, s1[i]))
			break ;
		i++;
	}
	j = ft_strlen(s1);
	while (j)
	{
		if (!ft_strchr(set, s1[j]))
			break ;
		j--;
	}
	return (ft_substr(s1, i, j + 1 - i));
}
