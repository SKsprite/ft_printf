/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkoh <stkoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/15 20:49:33 by stkoh             #+#    #+#             */
/*   Updated: 2026/10/19 23:17:25 by stkoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_format(const char *str, va_list *args, t_parsed *inpt, int *count)
{
	(void) str;
	(void) args;
	(void) inpt;
	(void) count;
	return (1);
}

static int	print_literal(const char *str, int *count)
{
	int	i;
	int	suc;

	i = 0;
	while (str[i] && str[i] != '%')
		i++;
	suc = write(1, str, i);
	if (suc >= 0)
		*count += suc;
	return (suc);
}

static void	reset_parsed(t_parsed *inpt)
{
	inpt->flags = 0;
	inpt->has_width = 0;
	inpt->width = 0;
	inpt->has_precision = 0;
	inpt->precision = 0;
	inpt->type = 0;
}

int	ft_printf(const char *str, ...)
{
	va_list		args;
	size_t		i;
	int			count;
	t_parsed	parsed_inpt;
	ssize_t		ret;

	if (!str)
		return (-1);
	i = 0;
	count = -1;
	va_start(args, str);
	while (str && str[i])
	{
		if (str[i] == '%')
		{
			reset_parsed(&parsed_inpt);
			ret = print_format(&str[i], &args, &parsed_inpt, &count);
		}
		else
			ret = print_literal(&str[i], &count);
		if (ret < 0)
			return (va_end(args), count);
		i += ret;
	}
	return (va_end(args), count);
}
