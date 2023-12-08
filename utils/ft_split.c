#include "minishell.h"

static void	*ft_calloc(size_t count, size_t size)
{
	unsigned long long		a;
	char					*ptr;

	a = 0;
	ptr = malloc(count * size);
	if (ptr == 0)
		return (0);
	while (a < count * size)
	{
		ptr[a++] = 0;
	}
	return (ptr);
}

static int	freeing(char **list, int listindex)
{
	while (listindex >= 0)
	{
		free(list[listindex]);
		listindex--;
	}
	return (0);
}

static int	spliit(char const *s, char c, char **list, int listindex)
{
	size_t	a;
	int		save;

	a = 0;
	while (a < ft_strlen(s))
	{
		if (s[a] != c)
		{
			save = a;
			while (s[a] != c && s[a] != '\x00')
				a++;
			list[listindex] = ft_calloc(a - save + 1, 1);
			if (list[listindex] == 0)
				return (freeing(list, listindex));
			ft_strlcpy(list[listindex], &s[save], a - save + 1);
			listindex++;
		}
		a++;
	}
	list[listindex] = 0;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**list;
	int		listindex;

	listindex = 0;
	if (s == 0)
		return (0);
	list = ft_calloc (ft_strlen(s) + 1, sizeof(char *));
	if (list == 0)
		return (0);
	if (spliit(s, c, list, listindex) == 0)
	{
		free(list);
		return (0);
	}
	return (list);
}