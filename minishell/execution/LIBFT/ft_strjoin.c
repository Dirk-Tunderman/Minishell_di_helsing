/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 23:41:25 by eamrati           #+#    #+#             */
/*   Updated: 2022/10/13 17:18:59 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*concated;

	if (s1 == 0 || s2 == 0)
		return (0);
	concated = malloc (ft_strlen(s1) + ft_strlen(s2) + 1);
	if (concated == 0)
		return (0);
	ft_strlcpy(concated, s1, ft_strlen(s1) + 1);
	ft_strlcat(concated, s2, ft_strlen(s1) + ft_strlen(s2) + 1);
	return (concated);
}
