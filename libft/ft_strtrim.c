/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkoh <stkoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 18:49:32 by stkoh             #+#    #+#             */
/*   Updated: 2026/10/05 20:05:41 by stkoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	c_in_set(char c, char const *set)
{
	while (*set)
	{
		if (c == *set)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	len;
	size_t	start;
	size_t	end;
	char	*ptr;

	len = ft_strlen(s1);
	start = 0;
	end = 0;
	while (c_in_set(s1[start], set))
	{
		start++;
	}
	while (c_in_set(s1[len - end - 1], set))
	{
		end++;
	}
	ptr = ft_substr(s1, start, (len - (end + start)));
	if (!ptr)
		return (NULL);
	return (ptr);
}
