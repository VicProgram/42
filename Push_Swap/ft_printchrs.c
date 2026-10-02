/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printchrs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 14:14:13 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/03/20 14:14:16 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	ft_printchr(int c)
{
	write(2, &c, 1);
	return (1);
}

int	ft_printstr(char *s)
{
	int	i;

	if (!s)
		return (ft_printstr("(null)"));
	i = 0;
	while (s[i])
	{
		write(2, &s[i], 1);
		i++;
	}
	return (i);
}

int	ft_printnbr(int n)
{
	unsigned int	l;
	int				i;

	i = 0;
	if (n < 0)
	{
		l = -n;
		write(2, "-", 1);
		i++;
	}
	else
		l = (unsigned int) n;
	if (l >= 10)
		i += ft_printnbr(l / 10);
	i += ft_printchr((l % 10) + '0');
	return (i);
}

int	ft_printunbr(unsigned int n)
{
	int	i;

	i = 0;
	if (n >= 10)
		i += ft_printunbr(n / 10);
	i += ft_printchr((n % 10) + '0');
	return (i);
}
