/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkoh <stkoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/15 20:49:33 by stkoh             #+#    #+#             */
/*   Updated: 2026/10/16 20:55:35 by stkoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	parse_type(const char c, va_list *args)
{
	char	o;

	if ((t_u8) c == (t_u8) 'c')
		o = va_arg(&args, char);
}

void	format_str(const char *str, va_list *args)
{
	size_t	i;

	s = (t_u8 *) str;
	while (s[i])
	{
		if (s[i] == (t_u8) '%')
		{
			while (1)
				i++;
		}
		parse_type(s[i], args);
	}
}

int	ft_printf(const char *str, ...)
{
	va_list	args;

	va_start(args, str);
	format_str(str, &args);
	va_end(args);
	return ((void *)0);
}
