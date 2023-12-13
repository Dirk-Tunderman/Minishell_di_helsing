#include "../minishell.h"

size_t	ft_strlen(const char *s)
{
	int	a;

	a = 0;
	while (s && *s != '\x00')
	{
		s++;
		a++;
	}
	return (a);
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

	p = (char *)ft_calloc(sizeof(char), ft_strlen(source) + 1);
	if (p == NULL)
		return (NULL);
	i = 0;
	while (source && source[i])
	{
		p[i] = source[i];
		i++;
	}
	p[i] = '\0';
	return (p);
}

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
