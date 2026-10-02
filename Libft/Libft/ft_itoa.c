/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:30:11 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/01/23 19:18:13 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_nb_len(long n)
{
	int	cnt;

	cnt = 1;
	while (n > 9)
	{
		cnt++;
		n = n / 10;
	}
	return (cnt);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		nb_len;
	long	n2;

	nb_len = 0;
	n2 = n;
	if (n2 < 0)
	{
		n2 *= -1;
		nb_len += 1;
	}
	nb_len += ft_nb_len(n2);
	str = malloc(sizeof(char) * nb_len + 1);
	if (!str)
		return (NULL);
	str[nb_len] = '\0';
	while (n2 > 9)
	{
		str[--nb_len] = n2 % 10 + '0';
		n2 /= 10;
	}
	str[--nb_len] = n2 % 10 + '0';
	if (n < 0)
		str[0] = '-';
	return (str);
}
