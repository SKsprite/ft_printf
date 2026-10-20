/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkoh <stkoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 01:12:08 by stkoh             #+#    #+#             */
/*   Updated: 2026/10/20 03:59:49 by stkoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_perc(va_list *arg, t_parsed *parsed)
{
	int	printed;

	(void) arg;
	(void) parsed;
	printed = write(1, "%", 1);
	return (printed);
}

static void	init_table(t_dispatch_table *table)
{
	table[0] = (t_dispatch_table){TYPE_C, print_c};
	table[1] = (t_dispatch_table){TYPE_S, print_s};
	table[2] = (t_dispatch_table){TYPE_P, print_p};
	table[3] = (t_dispatch_table){TYPE_D, print_d};
	table[4] = (t_dispatch_table){TYPE_I, print_i};
	table[5] = (t_dispatch_table){TYPE_U, print_u};
	table[6] = (t_dispatch_table){TYPE_X, print_x};
	table[7] = (t_dispatch_table){TYPE_U_X, print_u_x};
	table[8] = (t_dispatch_table){TYPE_PERC, print_perc};
	table[9] = (t_dispatch_table){0, 0};
}

t_handler	get_handler(char c)
{
	t_dispatch_table	table[10];
	int					i;

	init_table(table);
	i = 0;
	while (table[i].type)
	{
		if ((char)table[i].type == c)
			return (table[i].func);
		i++;
	}
	return (0);
}
