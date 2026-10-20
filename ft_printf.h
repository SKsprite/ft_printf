/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkoh <stkoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/16 20:07:19 by stkoh             #+#    #+#             */
/*   Updated: 2026/10/20 18:32:54 by stkoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdarg.h>
# include "./libft/libft.h"

# define DEC "0123456789"
# define HEX_LO "0123456789abcdef"
# define HEX_UP "0123456789ABCDEF"
# define DIGITS_MAX 32
# define FLAG_CHARS "#-+0 "

typedef unsigned int	t_uint;
typedef unsigned long	t_ul;

typedef enum e_rule
{
	FLAG_HASH = 1 << 0,
	FLAG_MINUS = 1 << 1,
	FLAG_PLUS = 1 << 2,
	FLAG_ZERO = 1 << 3,
	FLAG_SPACE = 1 << 4,
	FLAG_ALL = (1 << 5) - 1,
	K_CHAR = 1 << 5,
	K_STR = 1 << 6,
	K_PCT = 1 << 7,
	K_INT = 1 << 8,
	K_PTR = 1 << 9,
	K_HEX = 1 << 10,
	K_UPPER = 1 << 11,
	K_TEXT = K_CHAR | K_STR | K_PCT,
	ALLOW_TEXT = FLAG_MINUS,
	ALLOW_UINT = FLAG_MINUS | FLAG_ZERO,
	ALLOW_HEX = FLAG_MINUS | FLAG_ZERO | FLAG_HASH,
	ALLOW_INT = FLAG_MINUS | FLAG_ZERO | FLAG_PLUS | FLAG_SPACE
}	t_rule;

typedef struct s_specs
{
	int		flags;
	int		width;
	int		precision;
	char	conv;
}	t_specs;

typedef enum e_align
{
	ALIGN_LEFT,
	ALIGN_RIGHT,
	ALIGN_ZEROS
}	t_align;

typedef struct s_field
{
	const char	*sign;
	const char	*prefix;
	int			zeros;
	const char	*body;
	int			len;
	int			width;
	t_align		align;
	char		buf[DIGITS_MAX];
}	t_field;

int			ft_printf(const char *str, ...);
int			get_rule(const t_specs *s);
int			parse_spec(const char *str, int *i, t_specs *spec, va_list args);
void		format_arg(const t_specs *s, va_list arg, t_field *f);
int			conv_rules(char c);
int			print_field(const t_field *f);
void		format_number(const t_specs *s, int rule, va_list arg, t_field *f);
#endif
