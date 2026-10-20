/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_spec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkoh <stkoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 02:38:32 by stkoh             #+#    #+#             */
/*   Updated: 2026/10/20 06:00:36 by stkoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	num_with_precision(char *itoaed, t_ul precision)
{
	t_ul	len;
	int		count;

	len = ft_strlen(itoaed);
	count = 0;
	if (itoaed[0] == '-')
		len--;
	while (count + len <= precision)
		count += write(1, &"0", 1);
	count += write(1, itoaed, len);
	return (count);
}

static int	set_flag(char c, t_parsed *parsed)
{
	if (c == '#')
		parsed->flags |= FLAG_HASH;
	else if (c == ' ')
		parsed->flags |= FLAG_SPACE;
	else if (c == '0')
		parsed->flags |= FLAG_ZERO;
	else if (c == '-')
		parsed->flags |= FLAG_MINUS;
	else if (c == '+')
		parsed->flags |= FLAG_PLUS;
	else
		return (0);
	return (1);
}

t_parsed	*parse_format_spec(const char *c, va_list *arg,
				t_parsed *parsed, int *i)
{
	int	j;

	if (!parsed->has_precision && !parsed->has_width && set_flag(c[*i], parsed))
	{
		return (parsed);
	}
	else if (!parsed->has_precision)
	{
		if (c[*i] == '*')
			parsed->width = va_arg(*arg, int);
		parsed->has_width = 1;
	}
	else if (c[*i] == '.')
	{
		(*i)++;
		j = *i;
		if (c[*i] == '*')
			parsed->precision = va_arg(*arg, int);
		else
			parsed->precision = ft_atoi(&c[*i]);
		parsed->has_precision = 1;
	}
	return (parsed);
}
