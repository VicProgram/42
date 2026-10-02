/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 13:34:28 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/01/23 19:31:37 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	*s_new;

	s_new = (char *)s;
	if ((unsigned char)c == '\0')
	{
		while (*s_new)
			s_new++;
		return (s_new);
	}
	while (*s_new)
	{
		if (*s_new == (unsigned char)c)
			return (s_new);
		s_new++;
	}
	return (NULL);
}
