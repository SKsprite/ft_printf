/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkoh <stkoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 14:27:58 by stkoh             #+#    #+#             */
/*   Updated: 2026/10/04 17:40:22 by stkoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	unsigned char	target;

	target = (unsigned char) c;
	while (*s)
	{
		if ((unsigned char) *s == target)
			return ((char *) s);
		s++;
	}
	if ((unsigned char) *s == target)
		return ((char *) s);
	return (NULL);
}
