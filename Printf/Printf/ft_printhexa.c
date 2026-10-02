/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printhexa.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 18:54:30 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/01/30 12:52:00 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printhexa(unsigned long c, const char *base)
{
	int		len;

	len = 0;
	if (c >= 16)
		len += ft_printhexa((c / 16), base);
	len += ft_printchr(base[c % 16]);
	return (len);
}
