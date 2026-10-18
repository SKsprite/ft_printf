/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkoh <stkoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/16 20:07:19 by stkoh             #+#    #+#             */
/*   Updated: 2026/10/18 19:10:46 by stkoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

typedef unsigned char	t_u8;

typedef enum	e_flags
{
	FLAG_HASH = 1 << 0;
	FLAG_MINUS = 1 << 1;
	FLAG_PLUS = 1 << 2;
	FLAG_ZERO = 1 << 3;
	FLAG_SPACE = 1 << 4;
};

typedef enum	e_types
{
	TYPE_C = 'c';
	TYPE_S = 's';
	TYPE_P = 'p';
	TYPE_D = 'd';
	TYPE_I = 'i';
	TYPE_U = 'u';
	TYPE_X = 'x';
	TYPE_U_X = 'X';
	TYPE_PERC = '%';
};

typedef struct	s_parsed
{
	e_flags	flags;
	int		has_width;
	int		width;
	int		precision;
	e_types	type;
	int		advanced;
} t_parsed;

int		ft_printf(const char *str, ...);
void	parse_type(const char c, va_list *arg);

#endif
