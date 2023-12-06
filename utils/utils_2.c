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

int ft_strcmp(const char *str1, const char *str2) 
{
    int i;

	i = 0;
    while (str1[i] != '\0' && str1[i] == str2[i])
        i++;
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

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	j;
	size_t	dlen;
	size_t	slen;
	size_t	res;

	slen = ft_strlen(src);
	dlen = ft_strlen(dst);
	if (dstsize == 0)
		res = dlen;
	if (dlen < dstsize)
		res = slen + dlen;
	else
		res = dstsize + slen;
	j = 0;
	while (src[j] && (dlen + 1) < dstsize)
	{
		dst[dlen] = src[j];
		dlen++;
		j++;
	}
	dst[dlen] = '\0';
	return (res);
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
