/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 18:13:34 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/01/23 19:33:33 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*s1_n;
	unsigned char	*s2_n;
	unsigned char	i;

	s1_n = (unsigned char *)s1;
	s2_n = (unsigned char *)s2;
	if (s1_n == s2_n)
		return (0);
	i = 0;
	while (i < n)
	{
		if (s1_n[i] != s2_n[i])
			return (s1_n[i] - s2_n[i]);
		i++;
	}
	return (0);
}
