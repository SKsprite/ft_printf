/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkoh <stkoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 19:46:15 by stkoh             #+#    #+#             */
/*   Updated: 2026/10/06 17:35:59 by stkoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	nbr(int nb, int fd)
{
	if (nb == 0)
		return ;
	nbr(nb / 10, fd);
	ft_putstr_fd((char []){'0' + nb % 10, '\0'}, fd);
}

void	ft_putnbr_fd(int n, int fd)
{
	if (n == -2147483648)
	{
		ft_putstr_fd("-2147483648", fd);
		return ;
	}
	if (n == 0)
	{
		ft_putstr_fd("0", fd);
		return ;
	}
	if (n < 0)
	{
		ft_putstr_fd("-", fd);
		n *= -1;
	}
	nbr(n, fd);
}
