/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkoh <stkoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 18:05:39 by stkoh             #+#    #+#             */
/*   Updated: 2026/10/20 18:57:31 by stkoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	emit(int *count, const char *s, int n)
{
	if (*count < 0 || n <= 0)
		return ;
	if (write(1, s, n) != n)
		*count = -1;
	else
		*count += n;
}

static void	emit_repeat(int *count, char c, int n)
{
	while (n > 0)
	{
		emit(count, &c, 1);
		n--;
	}
}

int	print_field(const t_field *f)
{
	int	count;
	int	pad;

	pad = f->width - f->zeros - f->len;
	pad -= ft_strlen(f->sign) + ft_strlen(f->prefix);
	count = 0;
	if (f->align == ALIGN_RIGHT)
		emit_repeat(&count, ' ', pad);
	emit(&count, f->sign, ft_strlen(f->sign));
	emit(&count, f->prefix, ft_strlen(f->prefix));
	if (f->align == ALIGN_ZEROS)
		emit_repeat(&count, '0', f->zeros);
	emit(&count, f->body, f->len);
	if (f->align == ALIGN_LEFT)
		emit_repeat(&count, ' ', pad);
	return (count);
}
