#include "../minishell.h"


int	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s && s[i])
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

int ft_strcmp(const char *str1, const char *str2) 
{
    int i;

	i = 0;
    while (str1[i] != '\0' && str1[i] == str2[i])
        i++;
    return (unsigned char)str1[i] - (unsigned char)str2[i];
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	count;

	count = 0;
	while (src[count])
		count++;
	if (size == 0)
		return (count);
	else
	{	
		i = 0;
		while (src[i] && i < (size -1))
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (count);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while ((s1[i] || s2[i]) && i < n)
	{
		if (s1[i] != s2[i])
			return (((unsigned char)s1[i]) - ((unsigned char) s2[i]));
		if (s1[i] == s2[i])
			i++;
	}
	return (0);
}

char *ft_strtok_c(char *str, char delim) 
{
	static char *last;
    static char *last_character;

    last_character = last;
	if (str)
    {
		last = str;
        last_character = str;
    }
    if (last == NULL)
		return NULL;
	last = ft_strchr(last, delim);
	if (last == NULL)
		return (last_character);
	else
	{
		*last = '\0';
		last++;
	}
	return (last_character);
}
