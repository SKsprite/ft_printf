/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkoh <stkoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 18:34:31 by stkoh             #+#    #+#             */
/*   Updated: 2026/10/06 17:35:19 by stkoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*result;
	size_t	str_len;
	size_t	remaining;
	size_t	i;

	str_len = ft_strlen(s);
	if (start >= str_len)
		remaining = 0;
	else
	{
		remaining = str_len - start;
		if (remaining > len)
			remaining = len;
	}
	result = malloc((remaining + 1) * sizeof(char));
	if (!result)
		return (NULL);
	i = 0;
	while (i < remaining && s[start + i])
	{
		result[i] = s[start + i];
		i++;
	}
	result[i] = '\0';
	return (result);
}
