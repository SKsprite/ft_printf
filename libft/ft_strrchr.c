/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkoh <stkoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 14:38:44 by stkoh             #+#    #+#             */
/*   Updated: 2026/10/04 17:44:50 by stkoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int				str_len;
	unsigned char	target;

	target = (unsigned char) c;
	str_len = (int)ft_strlen(s);
	while (&s[str_len] >= s)
	{
		if ((unsigned char) s[str_len] == target)
			return ((char *) &s[str_len]);
		str_len--;
	}
	if ((unsigned char) s[str_len] == target)
		return ((char *) &s[str_len]);
	return (NULL);
}
