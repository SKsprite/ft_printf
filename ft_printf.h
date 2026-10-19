/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkoh <stkoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/16 20:07:19 by stkoh             #+#    #+#             */
/*   Updated: 2026/10/19 22:49:44 by stkoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdarg.h>

typedef unsigned char	t_u8;
typedef int				(*t_handler)(va_list *);

typedef enum e_flags
{
	FLAG_HASH = 1 << 0,
	FLAG_MINUS = 1 << 1,
	FLAG_PLUS = 1 << 2,
	FLAG_ZERO = 1 << 3,
	FLAG_SPACE = 1 << 4
}	t_flags;

typedef enum e_types
{
	TYPE_C = 'c',
	TYPE_S = 's',
	TYPE_P = 'p',
	TYPE_D = 'd',
	TYPE_I = 'i',
	TYPE_U = 'u',
	TYPE_X = 'x',
	TYPE_U_X = 'X',
	TYPE_PERC = '%'
}	t_types;

typedef struct s_parsed
{
	t_flags	flags;
	int		has_width;
	int		width;
	int		has_precision;
	int		precision;
	t_types	type;
}	t_parsed;

typedef struct s_dispatch_table
{
	t_types		type;
	t_handler	func;
}	t_dispatch_table;

int		ft_printf(const char *str, ...);

#endif
