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


t_env	*ft_lstlast(t_env *lst)
{
	while (lst)
	{
		if (!lst->next)
			return (lst);
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
