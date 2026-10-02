/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 17:44:57 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/01/23 19:32:35 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*cdst;
	unsigned char	*csrc;
	size_t			i;

	cdst = (unsigned char *)dest;
	csrc = (unsigned char *)src;
	if (!src && !dest)
		return (NULL);
	if (csrc >= cdst)
	{
		i = 0;
		while (i < n)
		{
			cdst[i] = csrc[i];
			i++;
		}
	}
	else
	{
		i = n;
		while (i-- > 0)
			cdst[i] = csrc[i];
	}
	return (dest);
}
