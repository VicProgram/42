/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 20:11:43 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/01/23 19:30:39 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_is_in(char c, const char *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*new;
	size_t	pri;
	size_t	ult;
	size_t	i;

	pri = 0;
	ult = ft_strlen(s1);
	while (s1[pri] && ft_is_in(s1[pri], set))
		pri++;
	while (ult > pri && ft_is_in(s1[ult - 1], set))
		ult--;
	new = malloc(sizeof(char) * (ult - pri) + 1);
	if (!new)
		return (NULL);
	i = 0;
	while (ult > pri)
	{
		new[i++] = s1[pri];
		pri++;
	}
	new[i] = '\0';
	return (new);
}
