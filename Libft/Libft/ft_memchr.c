/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 17:29:28 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/01/23 19:33:37 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*s2;

	s2 = (unsigned char *)s;
	while (n > 0)
	{
		if ((unsigned char)c == *s2)
			return ((void *)s2);
		s2++;
		n--;
	}
	return (NULL);
}
