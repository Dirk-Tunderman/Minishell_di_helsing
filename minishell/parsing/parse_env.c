#include "../minishell.h"

static char	**count_word(const char *str, char set)
{
	int		i;
	int		count;
	char	**c;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] != set && (str[i + 1] == set || str[i + 1] == '\0'))
			count++;
		i++;
	}
	c = (char **)malloc ((count + 1) * sizeof(char *));
	if (!c)
		return (NULL);
	return (c);
}

static int	ft_lenght_str(const char *str, char c, int h)
{
	int	len;

	len = 0;
	while (str[h] && str[h] != c)
	{
		h++;
		len++;
	}
	return (len);
}

static char	**ft_free(char **str, int i)
{
	while (i)
	{
		free (str[i]);
		i--;
	}
	free (str);
	return (str);
}

char	**ft_split(char *str, char c)
{
	int		i;
	int		j;
	char	**splitted;

	j = 0;
	i = 0;
	if (!str)
		return (NULL);
	splitted = count_word (str, c);
	if (!splitted)
		return (NULL);
	if (str[0] == '\0')
		return (splitted[0] = NULL, splitted);
	while (str[i])
	{
		while (str[i] == c)
			i++;
		if (str[i] == '\0')
			break ;
		splitted[j++] = ft_substr(str, i, ft_lenght_str (str, c, i));
		if (!splitted[j - 1])
			return (ft_free(splitted, j));
		i = i + ft_lenght_str (str, c, i);
	}
	return (splitted[j] = NULL, splitted);
}

t_env	*ft_lstlast(t_env *lst)
{
	while (lst)
	{
		if (!lst->next)
		{
			return (lst);
		}
		lst = lst->next;
	}
	return (lst);
}

t_env *create_nodee(char *key, char *val)
{
    t_env *tmp;

    tmp = malloc(sizeof (t_env) * 1);
    tmp->key = key;
    tmp->value = val;
    tmp->next = NULL;
    return (tmp);
}


void	ft_lstadd_back(t_env **lst, t_env *new)
{
	t_env	*tmp;

	if (lst)
	{
		if (*lst == NULL)
			*lst = new;
		else
		{
			tmp = ft_lstlast(*(lst));
			tmp->next = new;
		}
	}
}

int	ft_lstsize(t_env *lst)
{
	int	i;

	i = 0;
	while (lst != NULL)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

int parse_env(char **str, t_data **data)
{
    int i;
    char **tmp;

    i = 0;
    while (str[i])
    {
        tmp = ft_split(str[i], '=');
        if (i == 0)
            (*data)->env = create_nodee(tmp[0],tmp[1]);
        ft_lstadd_back(&(*data)->env,create_nodee(tmp[0], tmp[1]));
        i++;
    }
	while ((*data)->env != NULL)
    {
        printf("%s=%s\n", (*data)->env->key, (*data)->env->value);
        (*data)->env = (*data)->env->next;
    }
    return (1);
}