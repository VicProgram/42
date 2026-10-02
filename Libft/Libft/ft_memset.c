/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 13:21:54 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/01/23 19:23:14 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	value;
	unsigned char	*temp;

	temp = (unsigned char *)s;
	value = (unsigned char)c;
	while (n > 0)
	{
		*temp++ = value;
		n--;
	}
	return (s);
}
