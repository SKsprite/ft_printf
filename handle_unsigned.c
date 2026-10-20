/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_unsigned.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkoh <stkoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 01:21:09 by stkoh             #+#    #+#             */
/*   Updated: 2026/10/20 06:07:38 by stkoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_u(va_list *arg, t_parsed *parsed)
{
	t_ul	num;
	int		count;
	char	*itoaed;

	num = va_arg(*arg, t_ul);
	if (!num)
		count = write(1, &"0", 1);
	else
	{
		itoaed = ft_itoa(num);
		if (!itoaed)
			return (-1);
		if (!parsed->has_precision)
			count = write(1, itoaed, ft_strlen(itoaed));
		else
			count = num_with_precision(itoaed, parsed->precision);
		free(itoaed);
	}
	return (count);
}

int	print_x(va_list *arg, t_parsed *parsed)
{
	(void) arg;
	(void) parsed;
	return (0);
}

int	print_u_x(va_list *arg, t_parsed *parsed)
{
	(void) arg;
	(void) parsed;
	return (0);
}

int	print_p(va_list *arg, t_parsed *parsed)
{
	t_ul	*addr;
	int		count;

	num = va_arg(*arg, t_ul *);
	if (!num)
		count = write(1, "(nil)", 5);
	else
	{
		count = write(1, &c, 1);
	}
	(void) parsed;
	(void) arg;
	(void) parsed;
	return (0);
}

int	print_c(va_list *arg, t_parsed *parsed)
{
	t_uint	num;
	t_u8	c;
	int		count;

	num = va_arg(*arg, t_uint);
	if (!num)
		count = write(1, &"\x00", 1);
	else
	{
		c = (t_u8) num;
		count = write(1, &c, 1);
	}
	(void) parsed;
	return (count);
}
