/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   formatter.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkoh <stkoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 17:30:49 by stkoh             #+#    #+#             */
/*   Updated: 2026/10/20 18:33:47 by stkoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static t_align	align_of(int rule)
{
	if (rule & FLAG_MINUS)
		return (ALIGN_LEFT);
	if (rule & FLAG_ZERO)
		return (ALIGN_ZEROS);
	return (ALIGN_RIGHT);
}

static void	format_text(const t_specs *s, int rule, va_list arg, t_field *f)
{
	f->buf[0] = s->conv;
	f->body = f->buf;
	f->len = 1;
	if (rule & K_PCT)
		f->width = 0;
	if (rule & K_CHAR)
		f->buf[0] = (char) va_arg(arg, int);
	if (rule & K_STR)
	{
		f->body = va_arg(arg, const char *);
		if (!f->body && s->precision >= 0 && s->precision < 6)
			f->body = "";
		else if (!f->body)
			f->body = "(null)";
		f->len = 0;
		while (f->body[f->len] && (s->precision < 0 || f->len < s->precision))
			f->len++;
	}
}

void	format_arg(const t_specs *s, va_list arg, t_field *f)
{
	int	rule;

	rule = get_rule(s);
	f->sign = "";
	f->prefix = "";
	f->zeros = 0;
	f->width = s->width;
	f->align = align_of(rule);
	if (rule & K_TEXT)
		format_text(s, rule, arg, f);
	else
		format_number(s, rule, arg, f);
}
