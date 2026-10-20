/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signed.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkoh <stkoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 01:23:51 by stkoh             #+#    #+#             */
/*   Updated: 2026/10/20 05:18:25 by stkoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_d(va_list *arg, t_parsed *parsed)
{
	int		num;
	int		count;
	char	*itoaed;

	num = va_arg(*arg, int);
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

int	print_i(va_list *arg, t_parsed *parsed)
{
	int		num;
	int		count;
	char	*itoaed;

	num = va_arg(*arg, int);
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

int	print_s(va_list *arg, t_parsed *parsed)
{
	char	*string;
	int		count;

	string = va_arg(*arg, char *);
	if (!string)
		count = write(1, "(null)", 6);
	else if (!parsed->has_precision)
		count = write(1, string, ft_strlen(string));
	else
		count = write(1, string, parsed->precision);
	return (count);
}
