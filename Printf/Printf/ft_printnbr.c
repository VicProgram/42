/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printnbr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 13:51:22 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/01/29 18:07:56 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printnbr(int n)
{
	unsigned int	l;
	int				i;

	i = 0;
	if (n < 0)
	{
		l = -n;
		write(1, "-", 1);
		i++;
	}
	else
		l = (unsigned int) n;
	if (l >= 10)
		i += ft_printnbr(l / 10);
	i += ft_printchr((l % 10) + '0');
	return (i);
}
