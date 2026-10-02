/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 16:54:28 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/01/23 19:25:00 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	unsigned char	*new_s;
	size_t			i;

	new_s = (unsigned char *)s;
	i = ft_strlen(s) + 1;
	while (i > 0)
	{
		i--;
		if (new_s[i] == (unsigned char)c)
			return ((char *)&new_s[i]);
	}
	return (NULL);
}
