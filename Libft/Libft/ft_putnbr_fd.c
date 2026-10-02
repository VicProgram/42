/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 14:05:50 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/01/23 19:31:29 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	unsigned int	l;

	if (n < 0)
	{
		l = -n;
		write(fd, "-", 1);
	}
	else
		l = n;
	if (l >= 10)
		ft_putnbr_fd(l / 10, fd);
	ft_putchar_fd((l % 10) + '0', fd);
}
