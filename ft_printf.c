/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkoh <stkoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/15 20:49:33 by stkoh             #+#    #+#             */
/*   Updated: 2026/10/20 03:56:36 by stkoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_format(const char *str, va_list *args, t_parsed *inpt, int *i)
{
	t_handler	func;

	(*i)++;
	func = NULL;
	while (str[*i] && !func)
	{
		func = get_handler(str[*i]);
		if (!func)
		{
			parse_format_spec(str, args, inpt, i);
			(*i)++;
		}
	}
	if (str[*i])
		(*i)++;
	if (!func)
		return (0);
	return (func(args, inpt));
}

static int	print_literal(const char *str, int *i)
{
	int	count;
	int	j;

	j = 0;
	while (str[*i + j] && str[*i + j] != '%')
		j++;
	count = write(1, &str[*i], j);
	if (count >= 0)
		*i += j;
	return (count);
}

static t_parsed	*reset_parsed(t_parsed *inpt)
{
	inpt->flags = 0;
	inpt->has_width = 0;
	inpt->width = 0;
	inpt->has_precision = 0;
	inpt->precision = 0;
	return (inpt);
}

int	ft_printf(const char *str, ...)
{
	va_list		args;
	int			i;
	int			count;
	t_parsed	parsed_inpt;
	ssize_t		ret;

	if (!str)
		return (-1);
	i = 0;
	count = 0;
	va_start(args, str);
	while (str && str[i])
	{
		if (str[i] == '%')
		{
			ret = print_format(str, &args, reset_parsed(&parsed_inpt), &i);
		}
		else
			ret = print_literal(str, &i);
		if (ret < 0)
			return (va_end(args), -1);
		count += ret;
	}
	return (va_end(args), count);
}
