#include "../minishell.h"

int ft_isspace(int c)
{
	return (c == ' ' || (c >= '\t' && c <= '\r'));
}
int is_operator(char c)
{
	return (c == '|' || c == '>' || c == '<');
}

char *ft_strjoin(char const *s1, char const *s2)
{
	char *str;
	size_t len;

	if (!s1 || !s2)
		return NULL;
	len = ft_strlen(s1) + ft_strlen(s2);
	str = ft_calloc(sizeof(char), len + 1); 
	if (!str)
		return NULL;
	ft_strcpy(str, s1); 
	ft_strcat(str, s2);
	return str;
}

char *ft_strndup(const char *str, size_t n) 
{
	size_t len;
	
	len = n;
	char *dup = (char *)ft_calloc(sizeof(char), len + 1);
	if (dup == NULL)
		return NULL;
	ft_strncpy(dup, str, len);
	dup[len] = '\0';
	return dup;
}

char *ft_strncpy(char *dest, const char *src, size_t n) 
{
    size_t i;

    i = 0;
    while (i < n && src[i] != '\0') 
	{
        dest[i] = src[i];
        i++;
    }
    while (i < n) {
        dest[i] = '\0';
        i++;
    }
    return dest;
}

char *ft_strcpy(char *dest, const char *src) 
{
	char *original_dest;

	original_dest = dest;
	while (src && *src)
	{
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';
	return original_dest;
}

char *ft_strcat(char *dest, const char *src) 
{
	char *original_dest;

	original_dest = dest;
	while (*dest) 
	{
		dest++;
	}
	while (*src) 
	{
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';
	return original_dest;
}

char *ft_strtok(char *str, const char *delim) 
{
	static char *last;
	char *start;

	last = NULL;
	if (str)
		last = str;
	if (last == NULL) 
		return NULL;
	while (*last && ft_strchr(delim, *last)) 
		last++;
	if (*last == '\0')
		return NULL;
	start = last;
	while (*last && !ft_strchr(delim, *last)) 
		last++;
	if (*last)
	{
		*last = '\0';
		last++;
	}
	return start;
}

char	*m_ft_strdup(const char *s1)
{
	int		a;
	char		*str;

	a = 0;
	str = malloc (ft_strlen(s1) + 1 + 1);
	if (str == 0)
		return (0);
	ft_bzero(str, ft_strlen(s1) + 1 + 1);
	while (a < ft_strlen(s1))
	{
		str[a] = s1[a];
		a++;
	}
	str[a] = 0;
	return (str);
}
