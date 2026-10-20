/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkoh <stkoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/15 20:49:33 by stkoh             #+#    #+#             */
/*   Updated: 2026/10/20 18:25:42 by stkoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_convert(const char *str, va_list args, int *i)
{
	t_specs	spec;
	t_field	field;
	int		start;

	(*i)++;
	start = *i;
	if (!parse_spec(str, i, &spec, args))
	{
		if (!str[*i])
			return (-1);
		*i = start;
		return (write(1, "%", 1));
	}
	format_arg(&spec, args, &field);
	return (print_field(&field));
}

static int	print_literal(const char *str, int *i)
{
	int	len;

	len = 0;
	while (str[*i + len] && str[*i + len] != '%')
		len++;
	if (write(1, &str[*i], len) != len)
		return (-1);
	*i += len;
	return (len);
}

int	ft_printf(const char *str, ...)
{
	va_list		args;
	int			i;
	int			count;
	ssize_t		ret;

	if (!str)
		return (-1);
	i = 0;
	count = 0;
	va_start(args, str);
	while (str[i])
	{
		if (str[i] == '%')
		{
			ret = print_convert(str, args, &i);
		}
		else
			ret = print_literal(str, &i);
		if (ret < 0)
			return (va_end(args), -1);
		count += ret;
	}
	return (va_end(args), count);
}
