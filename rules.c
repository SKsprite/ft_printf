/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rules.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkoh <stkoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 17:19:59 by stkoh             #+#    #+#             */
/*   Updated: 2026/10/20 18:32:29 by stkoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	conv_rules(char c)
{
	if (c == 'c')
		return (K_CHAR | ALLOW_TEXT);
	if (c == 's')
		return (K_STR | ALLOW_TEXT);
	if (c == '%')
		return (K_PCT);
	if (c == 'd' || c == 'i')
		return (K_INT | ALLOW_INT);
	if (c == 'u')
		return (ALLOW_UINT);
	if (c == 'x')
		return (K_HEX | ALLOW_HEX);
	if (c == 'X')
		return (K_HEX | K_UPPER | ALLOW_HEX);
	if (c == 'p')
		return (K_PTR | K_HEX | ALLOW_INT);
	return (0);
}

int	get_rule(const t_specs *s)
{
	int	allowed;
	int	rule;

	allowed = conv_rules(s->conv);
	rule = (allowed & ~FLAG_ALL) | (s->flags & allowed);
	if (rule & K_PTR)
		rule |= FLAG_HASH;
	if (rule & FLAG_MINUS)
		rule &= ~FLAG_ZERO;
	if (rule & FLAG_PLUS)
		rule &= ~FLAG_SPACE;
	if (s->precision >= 0)
		rule &= ~FLAG_ZERO;
	return (rule);
}
