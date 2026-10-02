/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printptr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 15:47:58 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/01/30 12:53:46 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printptr(void *c)
{
	unsigned long	l;
	int				len;

	len = 0;
	if (c == NULL)
		return (ft_printstr("(nil)"));
	l = (unsigned long)c;
	ft_printstr("0x");
	len += ft_printhexa((l), "0123456789abcdef");
	return (len + 2);
}
