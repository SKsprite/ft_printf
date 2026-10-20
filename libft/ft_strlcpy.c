/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkoh <stkoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 17:20:18 by stkoh             #+#    #+#             */
/*   Updated: 2026/10/04 17:26:56 by stkoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	copied;

	if (size == 0)
		return (ft_strlen(src));
	copied = 0;
	while (copied < size - 1 && src[copied])
	{
		dst[copied] = src[copied];
		copied++;
	}
	dst[copied] = '\0';
	return (ft_strlen(src));
}
