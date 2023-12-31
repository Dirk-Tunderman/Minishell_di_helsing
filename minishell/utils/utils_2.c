#include "../minishell.h"


int	ft_strlen(const char *s)
{
	size_t	i;

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

int ft_strcmp(const char *str1, const char *str2) {
    int i = 0;

    while (str1[i] != '\0' && str1[i] == str2[i]) {
        i++;
    }

    return (unsigned char)str1[i] - (unsigned char)str2[i];
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
