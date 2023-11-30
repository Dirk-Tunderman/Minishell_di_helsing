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
    char *str = malloc(2 * sizeof(char));
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

// /* New function: get_last_node */
t_node	*get_last_node(t_node **head)
{
	t_node	*last;

	last = *head;
	while (last->next != NULL)
		last = last->next;
	return (last);
}

void	add_node(t_node **head, char *data, t_token type) 
{
	if (*head == NULL)
	{
		t_node *new_node = create_node(data, type);
		if (new_node != NULL) 
		{
			*head = new_node;
		}
	}
	else
	{
		append_node(head, data, type);
	}
}

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
    size_t len = strnlen(str, n); // Get the length of the string up to 'n' characters
    
    char *dup = (char *)malloc((len + 1) * sizeof(char)); // Allocate memory for the new string
    if (dup == NULL) {
        return NULL; // Memory allocation failed
    }
    
    strncpy(dup, str, len); // Copy the characters to the new string
    dup[len] = '\0'; // Add null terminator
    
    return dup;
}

