/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkoh <stkoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 19:35:09 by stkoh             #+#    #+#             */
/*   Updated: 2026/10/04 20:13:59 by stkoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*applied;
	unsigned int	i;

	i = 0;
	applied = malloc((ft_strlen(s) + 1) * sizeof(char));
	while (s[i])
	{
		applied[i] = f(i, s[i]);
		i++;
	}
	applied[i] = '\0';
	return (applied);
}
