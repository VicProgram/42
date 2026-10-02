/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vic <vic@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 19:27:41 by vabad-ro          #+#    #+#             */
/*   Updated: 2025/12/22 15:27:04 by vic              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putchar(char a);

void	ft_putnbr(int nb)
{
	unsigned int	l;

	if (nb < 0)
	{
		l = -nb;
		write (1, "-", 1);
	}
	else
		l = nb;
	if (l >= 10)
		ft_putnbr(l / 10);
	ft_putchar((l % 10) + '0');
}

void	ft_putchar(char a)
{
	write(1, &a, 1);
}

int	main(void)
{
	ft_putnbr(10000);
	return (0);
}
