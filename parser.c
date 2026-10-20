/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkoh <stkoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 02:38:32 by stkoh             #+#    #+#             */
/*   Updated: 2026/10/20 19:03:20 by stkoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	parse_number(const char *str, int *i, va_list args)
{
	int	num;

	num = 0;
	if (str[*i] == '*')
		return ((*i)++, va_arg(args, int));
	while (ft_isdigit(str[*i]))
	{
		num *= 10;
		num += (str[*i] - '0');
		(*i)++;
	}
	return (num);
}

static int	set_flags(char c, t_specs *spec)
{
	const char	*flags;
	const char	*found;

	flags = FLAG_CHARS;
	found = ft_strchr(flags, c);
	if (!c || !found)
		return (0);
	spec->flags |= 1 << (found - flags);
	return (1);
}

int	parse_spec(const char *str, int *i, t_specs *spec, va_list args)
{
	spec->flags = 0;
	while (set_flags(str[*i], spec))
		(*i)++;
	spec->width = parse_number(str, i, args);
	spec->precision = -1;
	if (str[*i] == '.')
	{
		(*i)++;
		spec->precision = parse_number(str, i, args);
	}
	if (!conv_rules(str[*i]))
		return (0);
	spec->conv = str[*i];
	(*i)++;
	return (1);
}
