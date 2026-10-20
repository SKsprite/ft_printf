/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_num.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkoh <stkoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 17:50:53 by stkoh             #+#    #+#             */
/*   Updated: 2026/10/20 18:45:55 by stkoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static t_ul	fetch_number(int rule, va_list arg, int *neg)
{
	long	n;

	*neg = 0;
	if (rule & K_PTR)
		return ((t_ul) va_arg(arg, void *));
	if (!(rule & K_INT))
		return (va_arg(arg, t_uint));
	n = va_arg(arg, int);
	if (n >= 0)
		return (n);
	*neg = 1;
	return (-n);
}

static void	make_digits(t_field *f, t_ul n, int rule)
{
	const char	*base;
	t_ul		radix;
	int			i;

	base = DEC;
	if (rule & K_UPPER)
		base = HEX_UP;
	else if (rule & K_HEX)
		base = HEX_LO;
	radix = ft_strlen(base);
	i = DIGITS_MAX;
	f->buf[--i] = base[n % radix];
	while (n >= radix)
	{
		n /= radix;
		f->buf[--i] = base[n % radix];
	}
	f->body = f->buf + i;
	f->len = DIGITS_MAX - i;
}

static void	set_prefix(t_field *f, int rule, t_ul n, int neg)
{
	if (neg)
		f->sign = "-";
	else if (rule & FLAG_PLUS)
		f->sign = "+";
	else if (rule & FLAG_SPACE)
		f->sign = " ";
	if (!(rule & FLAG_HASH) || n == 0)
		return ;
	f->prefix = "0x";
	if (rule & K_UPPER)
		f->prefix = "0X";
}

void	format_number(const t_specs *s, int rule, va_list arg, t_field *f)
{
	t_ul	n;
	int		neg;

	n = fetch_number(rule, arg, &neg);
	if ((rule & K_PTR) && n == 0)
	{
		f->body = "(nil)";
		f->len = 5;
		if (f->align == ALIGN_ZEROS)
			f->align = ALIGN_RIGHT;
		return ;
	}
	set_prefix(f, rule, n, neg);
	make_digits(f, n, rule);
	if (s->precision == 0 && n == 0)
		f->len = 0;
	if (s->precision > f->len)
		f->zeros = s->precision - f->len;
}
