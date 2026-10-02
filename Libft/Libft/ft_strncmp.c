/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 17:12:02 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/01/23 16:38:29 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned int	i;
	unsigned char	*s1_n;
	unsigned char	*s2_n;

	s1_n = (unsigned char *)s1;
	s2_n = (unsigned char *)s2;
	i = 0;
	while ((s1_n[i] || s2_n[i]) && i < n)
	{
		if (s1_n[i] != s2_n[i])
			return (s1_n[i] - s2_n[i]);
		i++;
	}
	return (0);
}
