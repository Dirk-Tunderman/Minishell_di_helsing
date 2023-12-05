#include "../minishell.h"

int ft_isspace(int c)
{
	return (c == ' ' || (c >= '\t' && c <= '\r'));
}
int is_operator(char c)
{
	return (c == '|' || c == '>' || c == '<');
}

char *char_to_str(char c)
{
	char *str;

	str = malloc(2 * sizeof(char));
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

t_node	*get_last_node(t_node **head)
{
	t_node	*last;

	last = *head;
	while (last->next != NULL)
		last = last->next;
	return (last);
}

// void	add_node(t_node **head, char *data, t_token type) 
// {
// 	if (*head == NULL)
// 	{
// 		t_node *new_node = create_node(data, type);
// 		if (new_node != NULL) 
// 			*head = new_node;
// 	}
// 	else
// 		append_node(head, data, type, 0);
// }

int	check_if_present(char c, char *str) 
{
	while (*str) 
	{
		if (*str == c)
			return (1);
		str++;
	}
	return (0);
}

int	incl_char(char c, char *str) 
{
	return (check_if_present(c, str));
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

char *ft_strjoin(char const *s1, char const *s2)
{
	char *str;
	size_t len;

	if (!s1 || !s2)
		return NULL;
	len = ft_strlen(s1) + ft_strlen(s2);
	str = malloc(len + 1); 
	if (!str)
		return NULL;
	strcpy(str, s1); 
	strcat(str, s2);
	return str;
}

char *ft_strndup(const char *str, size_t n) 
{
	size_t len;
	
	len = n;// ft_strnlen(str, n);
	char *dup = (char *)malloc((len + 1) * sizeof(char));
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

char *ft_strltok(char *str, const char *delim) 
{
	static char *last;
	char *start;

	last = NULL;
	if (str)
		last = str;
	if (last == NULL || *last == '\0')
		return NULL;
	start = last;
	while (*last && ft_strchr(delim, *last) == NULL)
		last++;
	if (*last) 
	{
		*last = '\0';
		last++;
	}
	return start;
}

char *ft_strcpy(char *dest, const char *src) 
{
	char *original_dest;

	original_dest = dest;
	while (*src) 
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

void	*ft_memset(void *b, int c, size_t len)
{
	size_t			i;
	unsigned char	*p;

	p = (unsigned char *)b;
	i = 0;
	while (i < len)
	{
		p[i] = (unsigned char)c;
		i++;
	}
	return (b);
}

size_t ft_strnlen(const char *s, size_t maxlen) 
{
    size_t len;

	len = 0;
    while (len < maxlen && s[len])
        len++;
    return len;
}