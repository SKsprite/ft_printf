/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkoh <stkoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/15 20:49:33 by stkoh             #+#    #+#             */
/*   Updated: 2026/10/18 19:30:55 by stkoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

size_t	parse(const char *curr, t_parsed *inpt);
{
	size_t	advanced;
	const t_u8	*str;

	str = (const t_u8 *) curr;
	if (*str == (t_u8) TYPE_C)
		inpt->type = TYPE_C;
	else if (*str == (t_u8) TYPE_S)
		inpt->type = TYPE_S;
	else if (*str == (t_u8) TYPE_P)
		inpt->type = TYPE_P;
	else if (*str == (t_u8) TYPE_D)
		inpt->type = TYPE_D;
	else if (*str == (t_u8) TYPE_I)
		inpt->type = TYPE_I;
	else if (*str == (t_u8) TYPE_U)
		inpt->type = TYPE_U;
	else if (*str == (t_u8) TYPE_X)
		inpt->type = TYPE_X;
	else if (*str == (t_u8) TYPE_U_X)
		inpt->type = TYPE_U_X;
	else if (*str == (t_u8) TYPE_PERC)
		inpt->type = TYPE_PERC;
}

void	format_str(const char *str, t_parsed *inpt, va_list *args)
{
	
	while (*curr)
	{
		while(*curr && *curr != (t_u8) '%')
		{
			curr++;
			count++;
		}
		ft_putstr_fd(1, str, count+diff);
		parse_type(s[i], args);
	}
}

void	reset_parsed(t_parsed *inpt)
{
	inpt->flags = 0;
	inpt->has_width = 0;
	inpt->width = 0;
	inpt->precision = 0;
	inpt->type = 0;
	inpt->advanced = 0;
}

int	ft_printf(const char *str, ...)
{
	va_list		args;
	const t_u8	*curr;
	size_t		count;
	t_parsed	parsed_inpt;

	if (!str)
		return (NULL);
	count = 0;
	curr = (const t_u8 *) str;
	va_start(args, str);
	while (*curr)
	{
		reset_parsed(&parsed_inpt);
		while (*curr && *curr != (t_u8) '%')
		{

		}
		if (*curr == (t_u8) '%')
		{
			parse(curr, &parsed_inpt);
			count += format_str(curr, &parsed_inpt, &args);
			
		}
	}
	va_end(args);
	return (count);
}
